#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <sys/select.h>
#include <sys/time.h>

#define KEY_ESC       27
#define KEY_UP        256
#define KEY_DOWN      257
#define KEY_RIGHT     258
#define KEY_LEFT      259
#define KEY_HOME      260
#define KEY_END       261
#define KEY_INSERT    262
#define KEY_DELETE    263
#define KEY_PGUP      264
#define KEY_PGDN      265
#define KEY_F1        266
#define KEY_F2        267
#define KEY_F3        268
#define KEY_F4        269
#define KEY_F5        270
#define KEY_F6        271
#define KEY_F7        272
#define KEY_F8        273
#define KEY_F9        274
#define KEY_F10       275
#define KEY_F11       276
#define KEY_F12       277

#define MOD_NONE  0
#define MOD_SHIFT 1
#define MOD_CTRL  2
#define MOD_ALT   4

atomic_uint kbp = 0; 
atomic_uint kbb = 0; 
atomic_uint kbf = 0; 

// Имена переменных возвращены в исходный вид для совместимости с tgws.c и button.rs
atomic_int mbutton = 0;
atomic_int mpx     = 0;
atomic_int mpy     = 0;

static atomic_int io_running = 1;
static struct termios orig_termios;
static atomic_int is_cleaned_up = 0;

#define mouse_enable()  do { printf("\033[?1003h"); fflush(stdout); } while(0)
#define mouse_disable() do { printf("\033[?1003l"); fflush(stdout); } while(0)

static void cleanup_io(void) {
    int expected = 0;
    if (atomic_compare_exchange_strong(&is_cleaned_up, &expected, 1)) {
        mouse_disable();
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    }
}

static void signal_handler(int sig) {
    (void)sig;
    atomic_store(&io_running, 0);
}

static void* unified_input_thread(void* arg) {
    (void)arg;
    struct termios raw;
    
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    
    mouse_enable();

    unsigned char buf[32];
    
    while (atomic_load(&io_running)) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        
        struct timeval timeout = {.tv_sec = 0, .tv_usec = 10000}; 
        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        
        if (rv > 0) {
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n == 0) {
                break;
            }
            if (n < 0) {
                continue;
            }

            if (n >= 6 && buf[0] == '\e' && buf[1] == '[' && buf[2] == 'M') {
                int raw_btn = (int)buf[3] - 32;
                
                int clean_btn = raw_btn & ~(4 | 8 | 16);

                if((int)buf[3]==35)clean_btn=100;

                atomic_store(&mbutton, clean_btn);
                atomic_store(&mpx,     (int)buf[4] - 33);
                atomic_store(&mpy,     (int)buf[5] - 33);
            } 
            else {
                unsigned int mods = MOD_NONE;
                unsigned int target_char = 0;

                if (n == 1 && buf[0] == '\e') {
                    target_char = KEY_ESC;
                }
                else if (n == 3 && buf[0] == '\e' && buf[1] == 'O') {
                    if (buf[2] == 'P') target_char = KEY_F1;
                    else if (buf[2] == 'Q') target_char = KEY_F2;
                    else if (buf[2] == 'R') target_char = KEY_F3;
                    else if (buf[2] == 'S') target_char = KEY_F4;
                    else if (buf[2] == 'H') target_char = KEY_HOME;
                    else if (buf[2] == 'F') target_char = KEY_END;
                }
                else if (n >= 3 && buf[0] == '\e' && buf[1] == '[') {
                    if (n == 3) {
                        if (buf[2] == 'A') target_char = KEY_UP;
                        else if (buf[2] == 'B') target_char = KEY_DOWN;
                        else if (buf[2] == 'C') target_char = KEY_RIGHT;
                        else if (buf[2] == 'D') target_char = KEY_LEFT;
                        else if (buf[2] == 'H') target_char = KEY_HOME;
                        else if (buf[2] == 'F') target_char = KEY_END;
                    }
                    else if (n >= 4 && buf[n-1] == '~') {
                        int code = 0;
                        for (int i = 2; i < n - 1; i++) {
                            if (buf[i] >= '0' && buf[i] <= '9') {
                                code = code * 10 + (buf[i] - '0');
                            }
                        }
                        switch (code) {
                            case 1: target_char = KEY_HOME; break;
                            case 2: target_char = KEY_INSERT; break;
                            case 3: target_char = KEY_DELETE; break;
                            case 4: target_char = KEY_END; break;
                            case 5: target_char = KEY_PGUP; break;
                            case 6: target_char = KEY_PGDN; break;
                            case 7: target_char = KEY_HOME; break;
                            case 8: target_char = KEY_END; break;
                            case 11: target_char = KEY_F1; break;
                            case 12: target_char = KEY_F2; break;
                            case 13: target_char = KEY_F3; break;
                            case 14: target_char = KEY_F4; break;
                            case 15: target_char = KEY_F5; break;
                            case 17: target_char = KEY_F6; break;
                            case 18: target_char = KEY_F7; break;
                            case 19: target_char = KEY_F8; break;
                            case 20: target_char = KEY_F9; break;
                            case 21: target_char = KEY_F10; break;
                            case 23: target_char = KEY_F11; break;
                            case 24: target_char = KEY_F12; break;
                        }
                    }
                }
                else if (n == 2 && buf[0] == '\e') {
                    mods |= MOD_ALT;
                    target_char = buf[1];
                }
                else if (n == 1 && buf[0] <= 31 && buf[0] != '\n' && buf[0] != '\r') {
                    mods |= MOD_CTRL;
                    if (buf[0] == 0) target_char = '`';
                    else if (buf[0] >= 1 && buf[0] <= 26) target_char = buf[0] + 96;
                    else if (buf[0] == 28) target_char = '\\';
                    else if (buf[0] == 29) target_char = ']';
                    else if (buf[0] == 30) target_char = '^';
                    else if (buf[0] == 31) target_char = '_';
                }
                else if (n == 1 && buf[0] >= 'A' && buf[0] <= 'Z') {
                    mods |= MOD_SHIFT;
                    target_char = buf[0] + 32;
                }
                else if (n == 1) {
                    target_char = buf[0];
                }

                if (target_char != 0) {
                    atomic_store(&kbb, target_char);
                    atomic_store(&kbf, mods);
                    atomic_store(&kbp, 1);
                }
            }
        }
    }
    
    cleanup_io();
    return NULL;
}

void start_io(void) {
    atexit(cleanup_io);
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    pthread_t tid;
    if (pthread_create(&tid, NULL, unified_input_thread, NULL) != 0) {
        perror("pthread_create failed");
        return;
    }
    pthread_detach(tid);
}
