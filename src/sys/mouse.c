atomic_uint kbp = 0;
atomic_uint kbn = 0;
atomic_uint kbf = 0;
atomic_uint kbb = 0;

atomic_int mbutton = 0;
atomic_int mpx = 0;
atomic_int mpy = 0;

static volatile int io_running = 1;
static struct termios orig_termios;

// Макросы для мыши
#define mouse_enable()  printf("\033[?1003h")
#define mouse_disable() printf("\033[?1003l")

static void cleanup_io() {
    mouse_disable();
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

static void signal_handler(int sig) {
    (void)sig;
    io_running = 0;
}

// Объединенный поток обработки клавиатуры и мыши
static void* unified_input_thread(void* arg) {
    (void)arg;
    struct termios raw;
    
    // Настройка терминала в raw mode
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    
    mouse_enable();
    setbuf(stdout, NULL);

    unsigned char buf[32];
    
    while (io_running) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // Таймаут 50мс
        
        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        
        if (rv > 0) {
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n > 0) {
                // Проверка на эскейп-последовательность мыши (X11)
                if (n >= 6 && buf[0] == '\e' && buf[1] == '[' && buf[2] == 'M') {
                    atomic_store(&mbutton, (int)buf[3] - 32);
                    atomic_store(&mpx,     (int)buf[4] - 33);
                    atomic_store(&mpy,     (int)buf[5] - 33);
                } 
                // Обработка клавиатуры
                else {
                    unsigned int mods = 0;
                    unsigned char target_char = buf[0];

                    // Распознавание Alt (Escape + буква, n == 2)
                    if (n == 2 && buf[0] == '\e') {
                        mods |= 4; // Устанавливаем флаг Alt
                        target_char = buf[1];
                    }
                    // Распознавание Ctrl (коды 1-26)
                    else if (n == 1 && buf[0] > 0 && buf[0] < 27 && buf[0] != '\n' && buf[0] != '\r') {
                        mods |= 2; // Устанавливаем флаг Ctrl
                        target_char = buf[0] + 96;
                    }

                    atomic_store(&kbb, target_char);
                    atomic_store(&kbf, mods);
                    atomic_store(&kbp, 1);
                }
            }
        } else if (rv == 0) {
            // Сбрасываем флаг нажатия клавиши при отсутствии ввода
            atomic_store(&kbp, 0);
        }
    }
    
    cleanup_io();
    return NULL;
}

// Новая функция инициализации вместо seekey() и seemous()
void start_io() {
    signal(SIGINT, signal_handler);
    pthread_t tid;
    if (pthread_create(&tid, NULL, unified_input_thread, NULL) != 0) {
        perror("pthread_create failed");
        return;
    }
    pthread_detach(tid);
}