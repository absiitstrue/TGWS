#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <ctype.h>

static int metrics[80];
static char sys_info_buf[1024];

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} CpuTime;

static CpuTime prev_cores[32];
static CpuTime prev_total;
static double prev_cpu_time = 0.0;
static double prev_net_time = 0.0;
static double prev_disk_time = 0.0;

static unsigned long long prev_rx_bytes = 0;
static unsigned long long prev_tx_bytes = 0;
static unsigned long long prev_disk_read_bytes = 0;
static unsigned long long prev_disk_write_bytes = 0;

static double get_time_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void read_file_string(const char *path, char *buf, size_t size) {
    buf[0] = '\0';
    int fd = open(path, O_RDONLY);
    if (fd < 0) return;
    ssize_t bytes = read(fd, buf, size - 1);
    if (bytes > 0) {
        buf[bytes] = '\0';
        char *newline = strchr(buf, '\n');
        if (newline) *newline = '\0';
    }
    close(fd);
}

static long read_file_long(const char *path) {
    char buf[64];
    read_file_string(path, buf, sizeof(buf));
    if (buf[0] == '\0') return 0;
    return atol(buf);
}

static int get_cpu_temp(void) {
    char path[256];
    char name[64];

    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "/sys/class/hwmon/hwmon%d/name", i);
        read_file_string(path, name, sizeof(name));

        if (strstr(name, "coretemp") || strstr(name, "k10temp") || strstr(name, "zenpower")) {
            for (int j = 1; j <= 5; j++) {
                snprintf(path, sizeof(path), "/sys/class/hwmon/hwmon%d/temp%d_input", i, j);
                long t = read_file_long(path);
                if (t > 0 && t < 150000) return (int)(t / 1000);
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "/sys/class/thermal/thermal_zone%d/temp", i);
        long t = read_file_long(path);
        if (t > 0 && t != 20000 && t != 27800 && t < 150000) return (int)(t / 1000);
    }
    return 0;
}

/*
 * ИСПРАВЛЕНИЕ (главный баг файла):
 * В оригинале core_count инкрементировался внутри блока `if (is_update)`,
 * поэтому при первом вызове (dt < 0.1, is_update == 0) core_count оставался 0,
 * и metrics[32]/metrics[33] (число ядер) обнулялись на первом кадре и могли
 * "мигать" при коротких интервалах между вызовами. Строку `core_count++`
 * вынесено из `if (is_update)`, так как подсчёт ядер не должен зависеть
 * от того, обновляем мы дельту загрузки или нет — ядро видно в /proc/stat
 * независимо от таймера.
 */
static void get_cpu_data(void) {
    double current_time = get_time_sec();
    double dt = current_time - prev_cpu_time;

    FILE *f = fopen("/proc/stat", "r");
    if (!f) return;

    char line[256];
    int core_count = 0;
    int is_update = (prev_cpu_time == 0.0) || (dt >= 0.1);

    while (fgets(line, sizeof(line), f)) {
        char tag[16];
        CpuTime cur = {0};

        if (sscanf(line, "%15s %llu %llu %llu %llu %llu %llu %llu %llu",
                   tag, &cur.user, &cur.nice, &cur.system, &cur.idle,
                   &cur.iowait, &cur.irq, &cur.softirq, &cur.steal) < 5) {
            continue;
        }

        if (strcmp(tag, "cpu") == 0) {
            if (is_update) {
                unsigned long long prev_busy = prev_total.user + prev_total.nice + prev_total.system + prev_total.irq + prev_total.softirq + prev_total.steal;
                unsigned long long cur_busy = cur.user + cur.nice + cur.system + cur.irq + cur.softirq + cur.steal;
                unsigned long long prev_idle = prev_total.idle + prev_total.iowait;
                unsigned long long cur_idle = cur.idle + cur.iowait;
                unsigned long long total_diff = (cur_busy + cur_idle) - (prev_busy + prev_idle);

                if (total_diff > 0) {
                    metrics[34] = (int)(((cur_busy - prev_busy) * 100.0) / total_diff);
                    metrics[35] = (int)((((cur.user + cur.nice) - (prev_total.user + prev_total.nice)) * 100.0) / total_diff);
                    metrics[36] = (int)((((cur.system + cur.irq + cur.softirq) - (prev_total.system + prev_total.irq + prev_total.softirq)) * 100.0) / total_diff);
                    metrics[37] = (int)(((cur_idle - prev_idle) * 100.0) / total_diff);

                    /* защита от значений вне диапазона из-за счётчиков jiffies,
                       переполнения или гонок при чтении /proc/stat */
                    if (metrics[34] < 0) metrics[34] = 0;
                    if (metrics[34] > 100) metrics[34] = 100;
                    if (metrics[35] < 0) metrics[35] = 0;
                    if (metrics[35] > 100) metrics[35] = 100;
                    if (metrics[36] < 0) metrics[36] = 0;
                    if (metrics[36] > 100) metrics[36] = 100;
                    if (metrics[37] < 0) metrics[37] = 0;
                    if (metrics[37] > 100) metrics[37] = 100;
                }
                prev_total = cur;
            }
        } else if (strncmp(tag, "cpu", 3) == 0 && isdigit((unsigned char)tag[3])) {
            int core_idx = atoi(tag + 3);
            if (core_idx >= 0 && core_idx < 32) {
                if (is_update) {
                    unsigned long long prev_busy = prev_cores[core_idx].user + prev_cores[core_idx].nice + prev_cores[core_idx].system + prev_cores[core_idx].irq + prev_cores[core_idx].softirq + prev_cores[core_idx].steal;
                    unsigned long long cur_busy = cur.user + cur.nice + cur.system + cur.irq + cur.softirq + cur.steal;
                    unsigned long long prev_idle = prev_cores[core_idx].idle + prev_cores[core_idx].iowait;
                    unsigned long long cur_idle = cur.idle + cur.iowait;
                    unsigned long long total_diff = (cur_busy + cur_idle) - (prev_busy + prev_idle);

                    if (total_diff > 0) {
                        int v = (int)(((cur_busy - prev_busy) * 100.0) / total_diff);
                        if (v < 0) v = 0;
                        if (v > 100) v = 100;
                        metrics[core_idx] = v;
                    }
                    prev_cores[core_idx] = cur;
                }
                /* core_count теперь считается всегда, а не только при is_update,
                   чтобы число ядер (metrics[32]/[33]) не было 0 на первом вызове */
                core_count++;
            }
        }
    }
    fclose(f);

    if (is_update) prev_cpu_time = current_time;

    metrics[32] = core_count;
    metrics[33] = core_count;

    /*
     * ИСПРАВЛЕНИЕ: путь cpu0/cpufreq доступен не на всех системах
     * (например, при отключённом cpufreq-драйвере или virtio).
     * Раньше при freq <= 0 после обеих попыток metrics[39] тихо
     * получал отрицательное/мусорное значение из-за деления
     * несчитанного long. Теперь явно обнуляем, если частоту
     * прочитать не удалось, вместо публикации мусора.
     */
    long freq = read_file_long("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    if (freq <= 0) freq = read_file_long("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq");
    metrics[39] = (freq > 0) ? (int)(freq / 1000) : 0;
    metrics[38] = get_cpu_temp();
}

/*
 * ИСПРАВЛЕНИЕ: формула used = mem_total - mem_free - cached - buffers - sreclaimable + shmem
 * при определённых состояниях памяти (например, сразу после старта системы,
 * когда cached+buffers+sreclaimable может из-за округления ядра быть чуть
 * больше mem_total - mem_free) даёт ОТРИЦАТЕЛЬНОЕ unsigned-число, которое
 * заворачивается в огромное положительное (переполнение unsigned long).
 * Это приводило к тому, что "used RAM" и "% used" могли внезапно показывать
 * гигантские/некорректные значения. Добавлена защита через знаковую
 * арифметику с последующим клампом в [0, mem_total].
 */
static void get_memory_data(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) return;

    char line[128];
    unsigned long mem_total = 0, mem_free = 0, mem_avail = 0, buffers = 0, cached = 0, sreclaimable = 0, shmem = 0;
    unsigned long swap_total = 0, swap_free = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemTotal:", 9) == 0) sscanf(line + 9, "%lu", &mem_total);
        else if (strncmp(line, "MemFree:", 8) == 0) sscanf(line + 8, "%lu", &mem_free);
        else if (strncmp(line, "MemAvailable:", 13) == 0) sscanf(line + 13, "%lu", &mem_avail);
        else if (strncmp(line, "Buffers:", 8) == 0) sscanf(line + 8, "%lu", &buffers);
        else if (strncmp(line, "Cached:", 7) == 0) sscanf(line + 7, "%lu", &cached);
        else if (strncmp(line, "SReclaimable:", 13) == 0) sscanf(line + 13, "%lu", &sreclaimable);
        else if (strncmp(line, "Shmem:", 6) == 0) sscanf(line + 6, "%lu", &shmem);
        else if (strncmp(line, "SwapTotal:", 10) == 0) sscanf(line + 10, "%lu", &swap_total);
        else if (strncmp(line, "SwapFree:", 9) == 0) sscanf(line + 9, "%lu", &swap_free);
    }
    fclose(f);

    long long used_signed = (long long)mem_total - (long long)mem_free
                           - (long long)cached - (long long)buffers
                           - (long long)sreclaimable + (long long)shmem;
    if (used_signed < 0) used_signed = 0;
    if (used_signed > (long long)mem_total) used_signed = (long long)mem_total;
    unsigned long used = (unsigned long)used_signed;

    metrics[40] = (int)(mem_total / 1024);
    metrics[41] = (int)(used / 1024);
    metrics[42] = mem_avail > 0 ? (int)(mem_avail / 1024) : (int)((mem_free + cached + buffers) / 1024);
    metrics[43] = mem_total > 0 ? (int)((used * 100.0) / mem_total) : 0;

    /* ИСПРАВЛЕНИЕ: аналогичная защита от переполнения unsigned для swap,
       если swap_free по какой-то причине окажется больше swap_total
       (гонка чтения /proc/meminfo между строками). */
    long long swap_used_signed = (long long)swap_total - (long long)swap_free;
    if (swap_used_signed < 0) swap_used_signed = 0;
    unsigned long swap_used = (unsigned long)swap_used_signed;

    metrics[44] = (int)(swap_total / 1024);
    metrics[45] = (int)(swap_used / 1024);
    metrics[46] = swap_total > 0 ? (int)((swap_used * 100.0) / swap_total) : 0;
}

static void get_disk_data(void) {
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        unsigned long long total = (unsigned long long)vfs.f_blocks * vfs.f_frsize;
        unsigned long long free = (unsigned long long)vfs.f_bfree * vfs.f_frsize;
        unsigned long long used = total - free;

        metrics[52] = (int)(total / (1024 * 1024 * 1024));
        metrics[53] = (int)(used / (1024 * 1024 * 1024));
        metrics[54] = total > 0 ? (int)((used * 100.0) / total) : 0;
    }

    FILE *f = fopen("/proc/diskstats", "r");
    if (!f) return;

    char line[256];
    unsigned long long cur_read_sectors = 0, cur_write_sectors = 0;

    while (fgets(line, sizeof(line), f)) {
        int major, minor;
        char dev_name[32];
        unsigned long r_ios, r_merges, r_sectors, r_ticks, w_ios, w_merges, w_sectors;

        if (sscanf(line, "%d %d %31s %lu %lu %lu %lu %lu %lu %lu",
                   &major, &minor, dev_name, &r_ios, &r_merges, &r_sectors, &r_ticks, &w_ios, &w_merges, &w_sectors) == 10) {

            if ((strncmp(dev_name, "sd", 2) == 0 && strlen(dev_name) == 3) ||
                (strncmp(dev_name, "nvme", 4) == 0 && strchr(dev_name, 'n') && !strchr(dev_name, 'p'))) {
                cur_read_sectors += r_sectors;
                cur_write_sectors += w_sectors;
            }
        }
    }
    fclose(f);

    unsigned long long cur_read_bytes = cur_read_sectors * 512;
    unsigned long long cur_write_bytes = cur_write_sectors * 512;

    double current_time = get_time_sec();
    double dt = current_time - prev_disk_time;

    if (dt >= 0.1) {
        if (prev_disk_time > 0.0) {
            metrics[50] = (int)(((cur_read_bytes - prev_disk_read_bytes) / 1024.0) / dt);
            metrics[51] = (int)(((cur_write_bytes - prev_disk_write_bytes) / 1024.0) / dt);
            /* ИСПРАВЛЕНИЕ: при сбросе счётчиков ядром (пересоздание устройства,
               контейнер и т.п.) cur_* может стать МЕНЬШЕ prev_*, что даёт
               огромное отрицательное значение из-за unsigned-вычитания выше
               (uint wrap), проявляющееся как гигантский всплеск I/O.
               Клампим отрицательные скорости в 0. */
            if (metrics[50] < 0) metrics[50] = 0;
            if (metrics[51] < 0) metrics[51] = 0;
        }
        prev_disk_read_bytes = cur_read_bytes;
        prev_disk_write_bytes = cur_write_bytes;
        prev_disk_time = current_time;
    }
}

static void get_network_data(void) {
    FILE *f = fopen("/proc/net/dev", "r");
    if (!f) return;

    char line[256];
    unsigned long long cur_rx = 0, cur_tx = 0;

    while (fgets(line, sizeof(line), f)) {
        char *colon = strchr(line, ':');
        if (colon) {
            char iface[32];
            sscanf(line, " %31[^:]", iface);
            if (strcmp(iface, "lo") != 0) {
                unsigned long long rx, tx, dummy;
                sscanf(colon + 1, "%llu %llu %llu %llu %llu %llu %llu %llu %llu",
                       &rx, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &tx);
                cur_rx += rx;
                cur_tx += tx;
            }
        }
    }
    fclose(f);

    metrics[62] = (int)(cur_rx / (1024 * 1024));
    metrics[63] = (int)(cur_tx / (1024 * 1024));

    double current_time = get_time_sec();
    double dt = current_time - prev_net_time;

    if (dt >= 0.1) {
        if (prev_net_time > 0.0) {
            metrics[60] = (int)(((cur_rx - prev_rx_bytes) / 1024.0) / dt);
            metrics[61] = (int)(((cur_tx - prev_tx_bytes) / 1024.0) / dt);
            /* ИСПРАВЛЕНИЕ: та же защита от wrap-around при сбросе счётчиков
               интерфейса (переподключение адаптера, смена iface и т.д.). */
            if (metrics[60] < 0) metrics[60] = 0;
            if (metrics[61] < 0) metrics[61] = 0;
        }
        prev_rx_bytes = cur_rx;
        prev_tx_bytes = cur_tx;
        prev_net_time = current_time;
    }
}

/*
 * ИСПРАВЛЕНИЕ: в оригинале Intel-ветка при успешном чтении freq делала
 * `return;` из get_gpu_data() ДО того, как для AMD-карты (если она вообще
 * присутствует на индексах i+1..3) выполнялась проверка — это в целом ок
 * по логике "нашли — выходим", НО настоящая проблема в другом:
 * при полном отсутствии GPU (card0..card3 не существуют или vendor
 * не читается) все метрики [70..72] у нас уже проинициализированы
 * нулями/temp — это верно и сохранено. Реальный баг был в том,
 * что переменная freq объявлена как `long`, а `metrics[72]` — `int`;
 * на системах, где gt_act_freq_mhz возвращает 0 из-за спящего GPU,
 * функция читала второй путь, но если ОБА пути вернули <= 0,
 * код всё равно не делал `return`, и цикл переходил к следующей
 * карте, что нормально. Тем не менее ветка AMD ниже не сбрасывала
 * metrics[70]/[71] в 0 перед чтением gpu_busy_percent, если чтение
 * не удавалось (возвращает -1 при отсутствии файла из read_file_long,
 * т.к. atol("") == 0 — ОК), но temp1_input индекс hwmon почти
 * никогда не совпадает с индексом карты (i), что давало неверную
 * температуру GPU. Добавлен реальный поиск нужного hwmon по имени,
 * с fallback на прежнее поведение, если по имени не нашли.
 */
static int find_hwmon_by_name_hint(const char *hint) {
    for (int i = 0; i < 10; i++) {
        char path[256], name[64];
        snprintf(path, sizeof(path), "/sys/class/hwmon/hwmon%d/name", i);
        read_file_string(path, name, sizeof(name));
        if (name[0] != '\0' && strstr(name, hint)) return i;
    }
    return -1;
}

static void get_gpu_data(void) {
    metrics[70] = 0;
    metrics[71] = metrics[38];
    metrics[72] = 0;

    for (int i = 0; i < 4; i++) {
        char path[256], vendor[32];
        snprintf(path, sizeof(path), "/sys/class/drm/card%d/device/vendor", i);
        read_file_string(path, vendor, sizeof(vendor));

        if (vendor[0] == '\0') continue;

        if (strstr(vendor, "0x8086")) {
            snprintf(path, sizeof(path), "/sys/class/drm/card%d/gt_act_freq_mhz", i);
            long freq = read_file_long(path);
            if (freq <= 0) {
                snprintf(path, sizeof(path), "/sys/class/drm/card%d/gt/gt0/rps_act_freq_mhz", i);
                freq = read_file_long(path);
            }
            if (freq > 0) {
                metrics[72] = (int)freq;
                return;
            }
        } else if (strstr(vendor, "0x1002")) {
            snprintf(path, sizeof(path), "/sys/class/drm/card%d/device/gpu_busy_percent", i);
            long busy = read_file_long(path);
            if (busy < 0) busy = 0;
            if (busy > 100) busy = 100;
            metrics[70] = (int)busy;

            int hw = find_hwmon_by_name_hint("amdgpu");
            if (hw >= 0) {
                snprintf(path, sizeof(path), "/sys/class/hwmon/hwmon%d/temp1_input", hw);
            } else {
                snprintf(path, sizeof(path), "/sys/class/hwmon/hwmon%d/temp1_input", i);
            }
            long temp = read_file_long(path);
            if (temp > 0) metrics[71] = (int)(temp / 1000);
            return;
        }
    }
}

int* get_system_metrics(void) {
    get_cpu_data();
    get_memory_data();
    get_disk_data();
    get_network_data();
    get_gpu_data();
    return metrics;
}

/*
 * ИСПРАВЛЕНИЕ: sys_info_buf — статический буфер, который переиспользуется
 * при каждом вызове sys_name(). Раньше os_name/cpu_name/gpu_name были
 * локальными char[] с дефолтными значениями "Unknown ..." — это верно,
 * но при повторных вызовах uname() не сбрасывался, а также при
 * PRETTY_NAME длиннее буфера strncpy не гарантировал null-terminator
 * (классический баг strncpy). Добавлена явная терминация после каждого
 * strncpy, чтобы избежать чтения мусора за пределами буфера.
 */
const char* sys_name(void) {
    struct utsname uts;
    char os_name[128] = "Linux";
    char cpu_name[256] = "Unknown CPU";
    char gpu_name[256] = "Unknown GPU";

    uname(&uts);

    FILE *f_os = fopen("/etc/os-release", "r");
    if (f_os) {
        char line[256];
        while (fgets(line, sizeof(line), f_os)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *start = line + 12;
                if (*start == '"') start++;
                char *end = strchr(start, '"');
                if (!end) end = strchr(start, '\n');
                if (end) *end = '\0';
                strncpy(os_name, start, sizeof(os_name) - 1);
                os_name[sizeof(os_name) - 1] = '\0';
                break;
            }
        }
        fclose(f_os);
    }

    FILE *f_cpu = fopen("/proc/cpuinfo", "r");
    if (f_cpu) {
        char line[256];
        while (fgets(line, sizeof(line), f_cpu)) {
            if (strncmp(line, "model name", 10) == 0) {
                char *colon = strchr(line, ':');
                if (colon) {
                    colon++;
                    while (*colon == ' ' || *colon == '\t') colon++;
                    char *newline = strchr(colon, '\n');
                    if (newline) *newline = '\0';
                    strncpy(cpu_name, colon, sizeof(cpu_name) - 1);
                    cpu_name[sizeof(cpu_name) - 1] = '\0';
                    break;
                }
            }
        }
        fclose(f_cpu);
    }

    for (int i = 0; i < 4; i++) {
        char path[256], vendor[32];
        snprintf(path, sizeof(path), "/sys/class/drm/card%d/device/vendor", i);
        read_file_string(path, vendor, sizeof(vendor));

        if (vendor[0] == '\0') continue;

        if (strstr(vendor, "0x8086")) {
            strncpy(gpu_name, "Intel HD/UHD/Iris Graphics", sizeof(gpu_name) - 1);
            gpu_name[sizeof(gpu_name) - 1] = '\0';
            break;
        } else if (strstr(vendor, "0x1002")) {
            strncpy(gpu_name, "AMD Radeon Graphics", sizeof(gpu_name) - 1);
            gpu_name[sizeof(gpu_name) - 1] = '\0';
            break;
        } else if (strstr(vendor, "0x10de")) {
            strncpy(gpu_name, "NVIDIA Corporation", sizeof(gpu_name) - 1);
            gpu_name[sizeof(gpu_name) - 1] = '\0';
            break;
        }
    }

    snprintf(sys_info_buf, sizeof(sys_info_buf),
             "OS: %s | Kernel: %s | CPU: %s | GPU: %s",
             os_name, uts.release, cpu_name, gpu_name);

    return sys_info_buf;
}
