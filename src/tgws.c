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
#define export __attribute__((visibility("default")))

int fore =0,pos =0, prx =-1,pry=-1;
int h,w,mx,my,mbtn,fpsw=120,refp=0;
char (*display2)[10] = NULL;
char (*display1)[10] = NULL;

char *bufer = NULL;
int bufersize,prew=-1;
int fcl_palitra = 0;
static int nn=0,nnn=0;

#define fastwrdisplay2(row, col,chars,r,g,b,fr,fg,fb) \
if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display2[(row) * w + (col)],r,g,b,fr,fg,fb,chars)

#define fastwrdisplay2c(row, col,chars,r,g,b) \
if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display2[(row) * w + (col)],r,g,b,0,0,0,chars)

#define fastwrdisplay2w(row, col,chars) \
if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display2[(row) * w + (col)],255,255,255,0,0,0,chars)

#define fastwrdisplay1(row, col,chars,r,g,b,fr,fg,fb) \
if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display1[(row) * w + (col)],r,g,b,fr,fg,fb,chars)

#define fastwrdisplay1c(row, col,chars,r,g,b) \
if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display1[(row) * w + (col)],r,g,b,0,0,0,chars)

#define fastwrdisplay1w(row, col,chars) \
if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display1[(row) * w + (col)],0,0,0,0,0,0,chars)

typedef struct {
    uint32_t str_bytes;
    uint8_t len;
} FastItoaLUT;

static const FastItoaLUT FAST_ITOA_255[256] = {
    { 0x00000030, 1 }, { 0x00000031, 1 }, { 0x00000032, 1 }, { 0x00000033, 1 }, { 0x00000034, 1 },
    { 0x00000035, 1 }, { 0x00000036, 1 }, { 0x00000037, 1 }, { 0x00000038, 1 }, { 0x00000039, 1 },
    { 0x00003031, 2 }, { 0x00003131, 2 }, { 0x00003231, 2 }, { 0x00003331, 2 }, { 0x00003431, 2 },
    { 0x00003531, 2 }, { 0x00003631, 2 }, { 0x00003731, 2 }, { 0x00003831, 2 }, { 0x00003931, 2 },
    { 0x00003032, 2 }, { 0x00003132, 2 }, { 0x00003232, 2 }, { 0x00003332, 2 }, { 0x00003432, 2 },
    { 0x00003532, 2 }, { 0x00003632, 2 }, { 0x00003732, 2 }, { 0x00003832, 2 }, { 0x00003932, 2 },
    { 0x00003033, 2 }, { 0x00003133, 2 }, { 0x00003233, 2 }, { 0x00003333, 2 }, { 0x00003433, 2 },
    { 0x00003533, 2 }, { 0x00003633, 2 }, { 0x00003733, 2 }, { 0x00003833, 2 }, { 0x00003933, 2 },
    { 0x00003034, 2 }, { 0x00003134, 2 }, { 0x00003234, 2 }, { 0x00003334, 2 }, { 0x00003434, 2 },
    { 0x00003534, 2 }, { 0x00003634, 2 }, { 0x00003734, 2 }, { 0x00003834, 2 }, { 0x00003934, 2 },
    { 0x00003035, 2 }, { 0x00003135, 2 }, { 0x00003235, 2 }, { 0x00003335, 2 }, { 0x00003435, 2 },
    { 0x00003535, 2 }, { 0x00003635, 2 }, { 0x00003735, 2 }, { 0x00003835, 2 }, { 0x00003935, 2 },
    { 0x00003036, 2 }, { 0x00003136, 2 }, { 0x00003236, 2 }, { 0x00003336, 2 }, { 0x00003436, 2 },
    { 0x00003536, 2 }, { 0x00003636, 2 }, { 0x00003736, 2 }, { 0x00003836, 2 }, { 0x00003936, 2 },
    { 0x00003037, 2 }, { 0x00003137, 2 }, { 0x00003237, 2 }, { 0x00003337, 2 }, { 0x00003437, 2 },
    { 0x00003537, 2 }, { 0x00003637, 2 }, { 0x00003737, 2 }, { 0x00003837, 2 }, { 0x00003937, 2 },
    { 0x00003038, 2 }, { 0x00003138, 2 }, { 0x00003238, 2 }, { 0x00003338, 2 }, { 0x00003438, 2 },
    { 0x00003538, 2 }, { 0x00003638, 2 }, { 0x00003738, 2 }, { 0x00003838, 2 }, { 0x00003938, 2 },
    { 0x00003039, 2 }, { 0x00003139, 2 }, { 0x00003239, 2 }, { 0x00003339, 2 }, { 0x00003439, 2 },
    { 0x00003539, 2 }, { 0x00003639, 2 }, { 0x00003739, 2 }, { 0x00003839, 2 }, { 0x00003939, 2 },
    { 0x00303031, 3 }, { 0x00313031, 3 }, { 0x00323031, 3 }, { 0x00333031, 3 }, { 0x00343031, 3 },
    { 0x00353031, 3 }, { 0x00363031, 3 }, { 0x00373031, 3 }, { 0x00383031, 3 }, { 0x00393031, 3 },
    { 0x00303131, 3 }, { 0x00313131, 3 }, { 0x00323131, 3 }, { 0x00333131, 3 }, { 0x00343131, 3 },
    { 0x00353131, 3 }, { 0x00363131, 3 }, { 0x00373131, 3 }, { 0x00383131, 3 }, { 0x00393131, 3 },
    { 0x00303231, 3 }, { 0x00313231, 3 }, { 0x00323231, 3 }, { 0x00333231, 3 }, { 0x00343231, 3 },
    { 0x00353231, 3 }, { 0x00363231, 3 }, { 0x00373231, 3 }, { 0x00383231, 3 }, { 0x00393231, 3 },
    { 0x00303331, 3 }, { 0x00313331, 3 }, { 0x00323331, 3 }, { 0x00333331, 3 }, { 0x00343331, 3 },
    { 0x00353331, 3 }, { 0x00363331, 3 }, { 0x00373331, 3 }, { 0x00383331, 3 }, { 0x00393331, 3 },
    { 0x00303431, 3 }, { 0x00313431, 3 }, { 0x00323431, 3 }, { 0x00333431, 3 }, { 0x00343431, 3 },
    { 0x00353431, 3 }, { 0x00363431, 3 }, { 0x00373431, 3 }, { 0x00383431, 3 }, { 0x00393431, 3 },
    { 0x00303531, 3 }, { 0x00313531, 3 }, { 0x00323531, 3 }, { 0x00333531, 3 }, { 0x00343531, 3 },
    { 0x00353531, 3 }, { 0x00363531, 3 }, { 0x00373531, 3 }, { 0x00383531, 3 }, { 0x00393531, 3 },
    { 0x00303631, 3 }, { 0x00313631, 3 }, { 0x00323631, 3 }, { 0x00333631, 3 }, { 0x00343631, 3 },
    { 0x00353631, 3 }, { 0x00363631, 3 }, { 0x00373631, 3 }, { 0x00383631, 3 }, { 0x00393631, 3 },
    { 0x00303731, 3 }, { 0x00313731, 3 }, { 0x00323731, 3 }, { 0x00333731, 3 }, { 0x00343731, 3 },
    { 0x00353731, 3 }, { 0x00363731, 3 }, { 0x00373731, 3 }, { 0x00383731, 3 }, { 0x00393731, 3 },
    { 0x00303831, 3 }, { 0x00313831, 3 }, { 0x00323831, 3 }, { 0x00333831, 3 }, { 0x00343831, 3 },
    { 0x00353831, 3 }, { 0x00363831, 3 }, { 0x00373831, 3 }, { 0x00383831, 3 }, { 0x00393831, 3 },
    { 0x00303931, 3 }, { 0x00313931, 3 }, { 0x00323931, 3 }, { 0x00333931, 3 }, { 0x00343931, 3 },
    { 0x00353931, 3 }, { 0x00363931, 3 }, { 0x00373931, 3 }, { 0x00383931, 3 }, { 0x00393931, 3 },
    { 0x00303032, 3 }, { 0x00313032, 3 }, { 0x00323032, 3 }, { 0x00333032, 3 }, { 0x00343032, 3 },
    { 0x00353032, 3 }, { 0x00363032, 3 }, { 0x00373032, 3 }, { 0x00383032, 3 }, { 0x00393032, 3 },
    { 0x00303132, 3 }, { 0x00313132, 3 }, { 0x00323132, 3 }, { 0x00333132, 3 }, { 0x00343132, 3 },
    { 0x00353132, 3 }, { 0x00363132, 3 }, { 0x00373132, 3 }, { 0x00383132, 3 }, { 0x00393132, 3 },
    { 0x00303232, 3 }, { 0x00313232, 3 }, { 0x00323232, 3 }, { 0x00333232, 3 }, { 0x00343232, 3 },
    { 0x00353232, 3 }, { 0x00363232, 3 }, { 0x00373232, 3 }, { 0x00383232, 3 }, { 0x00393232, 3 },
    { 0x00303332, 3 }, { 0x00313332, 3 }, { 0x00323332, 3 }, { 0x00333332, 3 }, { 0x00343332, 3 },
    { 0x00353332, 3 }, { 0x00363332, 3 }, { 0x00373332, 3 }, { 0x00383332, 3 }, { 0x00393332, 3 },
    { 0x00303432, 3 }, { 0x00313432, 3 }, { 0x00323432, 3 }, { 0x00333432, 3 }, { 0x00343432, 3 },
    { 0x00353432, 3 }, { 0x00363432, 3 }, { 0x00373432, 3 }, { 0x00383432, 3 }, { 0x00393432, 3 },
    { 0x00303532, 3 }, { 0x00313532, 3 }, { 0x00323532, 3 }, { 0x00333532, 3 }, { 0x00343532, 3 },
    { 0x00353532, 3 }
};

static inline char* itoa(char *str, int a) {
    uint8_t val = (uint8_t)a;
    *(uint32_t*)str = FAST_ITOA_255[val].str_bytes;
    return str + FAST_ITOA_255[val].len;
}

int cursor_mov(char *wr,int y,int x){
  char *p=wr;
  *p++='\033';
  *p++='[';
   p=itoa(p,y);*p++=';';
   p=itoa(p,x);*p++='H';
return (int)(p-wr);}

int fastsnprintfb(char *disp2,unsigned char *inf){
char *p = disp2;
    *p++='\033';*p++='[';*p++='3';
    *p++='8';*p++=';';*p++='2';
    *p++=';';
    p=itoa(p,inf[0]);*p++=';';
    p=itoa(p,inf[1]);*p++=';';
    p=itoa(p,inf[2]);*p++='m';

    *p++='\033';*p++='[';*p++='4';
    *p++='8';*p++=';';*p++='2';
    *p++=';';
    p=itoa(p,inf[3]);*p++=';';
    p=itoa(p,inf[4]);*p++=';';
    p=itoa(p,inf[5]);*p++='m';

  const char *chars = (const char*)&inf[6];
  while(*chars){
    *p++=*chars++;
  }


  
  *p='\0';
return (int)(p-disp2);}

static inline void fastsnprintf(char *mass,int r,int g,int b,int fr,int fg,int fb,const char *chars){
char *p= mass;
  *p++=r;
  *p++=g;
  *p++=b;
  *p++=fr;
  *p++=fg;
  *p++=fb;
  while(*chars){
    *p++=*chars++;
  }
  *p='\0';
}

void fwrdisplay2(int row, int col,char *world, int r,int g,int b,int fr,int fg,int fb ){
  if(h>(unsigned)(row)&&w>(unsigned)(col))fastsnprintf(display2[(row) * w + (col)],r,g,b,fr,fg,fb,world);}

#include "./include/includetools.c"
#include "./include/includesys.c"
#include "./include/call_level2.c"

    int tgws_close_window(){printf("\033[?1049l");system("tput init");
   printf("\033[?25h"); 
  free(display2);free(display1);free(bufer);
    return 0;}

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

    display2 = calloc(h * w, 16);if(!display2) exit(1);
for(int y =0;y<h;y++){for(int x=0;x<w;x++){fastwrdisplay2w(y,x," ");};};
    display1 = calloc(h * w, 16);if(!display1) exit(1);
    bufer = calloc(h*w,64);if(!bufer) exit(1);

    for(int y =0;y<h;y++){for(int x=0;x<w;x++){fastwrdisplay1w(y,x," ");printf("%s",display1[y*w+x]);};};
    start_io();}

int tgws_draw_element(){

  prew = mbtn;
    uint64_t stt = get_time_us();

        
        mx = atomic_load(&mpx);
        my = atomic_load(&mpy);
        mbtn = atomic_load(&mbutton);
        unsigned int current_kbf = atomic_load(&kbf);
        unsigned int current_kbb = atomic_load(&kbb);

        if(kbb==27){tgws_close_window();exit(0);}       
       
  for(int oy =1;oy<=h;oy++){for(int ox=1;ox<=w;ox++){
    fore = (oy-1)*w+(ox-1);

        uint64_t f_f  = *(uint64_t*)&display1[fore];
        uint16_t f_s = *(uint16_t*)&display1[fore][8];

        uint64_t s_f  = *(uint64_t*)&display2[fore]; 
        uint16_t s_s = *(uint16_t*)&display2[fore][8];


            if (f_f!=s_f||s_s!=f_s){

                if (prx==ox-1 && pry ==oy){
        pos+= fastsnprintfb(bufer+pos,(unsigned char*)display2[fore]);
      }
                else {pos+=cursor_mov(bufer+pos,oy,ox);pos += fastsnprintfb(bufer+pos,(unsigned char*)display2[fore]);};

      for(int spf=0;spf<10;spf++){
        display1[fore][spf]=display2[fore][spf];}
      prx = ox;pry =oy;};};};

write(STDOUT_FILENO, bufer, pos);pos =0;prx =-1;pry=-1;
int times = 1000000/fpsw;usleep(times);
uint64_t edt = get_time_us();
uint64_t dt = edt - stt;refp = (dt > 0) ? (int)(1000000ULL / dt) : 0;

for(int y =0;y<h;y++){for(int x=0;x<w;x++){fastwrdisplay2w(y,x," ");};};
  return 0;}

