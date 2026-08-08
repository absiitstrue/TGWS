#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <stdatomic.h>
#include <signal.h>
#include <pthread.h>
#include <stdarg.h>

int fore =0,pos =0, prx =-1,pry=-1;
int h,w,mx,my,mbtn,fpsw=120,refp=0;
char (*display2)[64] = NULL;
char (*display1)[64] = NULL;
char *bufer = NULL;
int bufersize,prew=-1;
__attribute__((visibility("default"))) int fcl_palitra = 0;
static int nn=0,nnn=0;

#define wrdisplay2(row, col, ...) \
    snprintf(display2[(row) * w + (col)], 64, __VA_ARGS__)
#define wrdisplay1(row, col, ...) \
    snprintf(display1[(row) * w + (col)], 64, __VA_ARGS__)


void fwrdisplay2(int row, int col, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(display2[row * w + col], 64, format, args);
    va_end(args);
}
#include "./include/includetools.rs"
#include "./include/includesys.rs"
#include "./include/call_level2.rs"

    int tgws_close_window(){printf("\033[?1049l");system("tput init");
   printf("\033[?25h"); 
    exit(1);}

    uint64_t get_time_us(void) {
    struct timespec ts;             
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)(ts.tv_nsec / 1000);
}

int tgws_init_window(int argc, char *argv[]){

    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-f")==0){fpsw = atoi(argv[i+1]);if(fpsw ==-1)fpsw=1000000;}
    }

    struct winsize ws;if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {perror("ioctl have error");return 1;} h = ws.ws_row;  w = ws.ws_col;

    setbuf(stdout, NULL); 
    printf("\033[?1049h");
   printf("\033[?25l"); 
    bufersize =h*w*64;

    display2 = calloc(h * w, 64);if(!display2) exit(1);
for(int y =0;y<h;y++){for(int x=0;x<w;x++){wrdisplay2(y,x,"\033[0m ");};};
    display1 = calloc(h * w, 64);if(!display1) exit(1);
    bufer = calloc(h*w,64);if(!bufer) exit(1);

    for(int y =0;y<h;y++){for(int x=0;x<w;x++){wrdisplay1(y,x," ");printf("%s",display1[y*w+x]);};};
    start_io();}

int tgws_draw_element(){

  prew = mbtn;
    uint64_t stt = get_time_us();

        
        mx = atomic_load(&mpx);
        my = atomic_load(&mpy);
        mbtn = atomic_load(&mbutton);
        unsigned int current_kbf = atomic_load(&kbf);
        unsigned int current_kbb = atomic_load(&kbb);

        if(kbb==27)tgws_close_window();       
       


        for(int oy =1;oy<=h;oy++){for(int ox=1;ox<=w;ox++){

            fore = (oy-1)*w+(ox-1);
            if (strcmp(display1[fore],display2[fore])!=0){

                if (prx==ox-1 && pry ==oy){pos += snprintf(bufer +pos ,bufersize-pos, "%s",display2[fore]);}
                else {pos += snprintf(bufer +pos ,bufersize-pos, "\033[%i;%iH%s",oy,ox,display2[fore]);};

                strcpy(display1[fore],display2[fore]);prx = ox;pry =oy;};};};

write(STDOUT_FILENO, bufer, pos);pos =0;prx =-1;pry=-1;
int times = 1000000/fpsw;usleep(times);
uint64_t edt = get_time_us();
uint64_t dt = edt - stt;refp = (dt > 0) ? (int)(1000000ULL / dt) : 0;   

for(int y =0;y<h;y++){for(int x=0;x<w;x++){wrdisplay2(y,x,"\033[0m ");};};
  return 0;}

