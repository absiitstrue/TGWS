#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define wrdisplay3(row, col, ...) \
    snprintf(display3[(row) * w + (col)], 64, __VA_ARGS__)
char (*display3)[64] = NULL;

int html(void);

#include "../src/tgws.c"

int html() {
  int  wrtd(int r,int g,int b,int size){
int fkd = mx %2;

int draw_p(int y,int x){
    wrdisplay3(y,x,"\033[38;2;%d;%d;%dm\u2588", r, g, b);wrdisplay3(y,x+1,"\033[38;2;%d;%d;%dm\u2588", r, g, b);return 0;}

switch(size){
    case 1:
    if(fkd)draw_p(my-6,mx-1);else draw_p(my-6,mx);break;

    case 2:
    if(fkd && mx+1<=w&& mx-1!=0&&my+1<=h){              draw_p(my-5,mx-1);
        draw_p(my-6,mx+1);draw_p(my-6,mx-1);draw_p(my-6,mx-3);
                          draw_p(my-7,mx-1);}

    else if(mx+1<=w&& mx-1!=0&&my+1<=h){              draw_p(my-5,mx);
    draw_p(my-6,mx+2);draw_p(my-6,mx);draw_p(my-6,mx-2);
                      draw_p(my-7,mx);}break;

    case 3:
    if(fkd&&mx+1<=w&& mx-1!=0&&my+1<=h){draw_p(my-5,mx+1);draw_p(my-5,mx-1);draw_p(my-5,mx-3);
            draw_p(my-6,mx+1);draw_p(my-6,mx-1);draw_p(my-6,mx-3);
            draw_p(my-7,mx+1);draw_p(my-7,mx-1);draw_p(my-7,mx-3);}

    else if(!fkd&& mx+1<=w&& mx-1!=0&&my+1<=h){
        draw_p(my-5,mx+2);draw_p(my-5,mx);draw_p(my-5,mx-2);
        draw_p(my-6,mx+2);draw_p(my-6,mx);draw_p(my-6,mx-2);
        draw_p(my-7,mx+2);draw_p(my-7,mx);draw_p(my-7,mx-2);}
  }}

static int aaa ,kj;
if(__builtin_expect(aaa ==0,1)){
    display3 = calloc((h-6) * (w-1), 64);aaa=1;
   for(int i =0;i<(h-7);i++){
    for(int ii=0;ii<w;ii++){
        wrdisplay3(i,ii,"\033[0m ");
    }
   }
}
char txt[150];
static int ffps ,sfps,size =1;
ffps++;if(__builtin_expect(ffps>=refp*2,0)){ffps =0;sfps =refp;}
sprintf(txt,"TGWS paint || TGWS version : 0.0.1e || paint version : 0.0.1w|| FPS (want):%3i || FPS (realy):%4i || FPS (slow):%4i",fpsw,refp,sfps);
label(0,0,txt);
border(1,255,255,255,1,0,4,w-1);

nn = button_classic(2,1,3,5,0);
static int color[3]={255};
square(color[0],color[1],color[2],2,1,2,5);label(3,1,"color");
vertical_line(0,255,255,255,2,7,3,0);vertical_line(0,255,255,255,2,12,3,0);
label(2,8,"size");char mtxt[20];sprintf(mtxt,"%2i",size);
label(3,8,mtxt);label(4,8,"+||-");
int szp = button_classic(4,8,0,0,200);if(__builtin_expect(szp==2,0)){size++;}
int szm =button_classic(4,11,0,0,199);if(__builtin_expect(szm==2,0)){size--;}
if(__builtin_expect(size <=0,0))size =0;if(__builtin_expect(size >=5,0))size =4;


if(my > 6&&(mbtn == 0||mbtn == 32)&& fcl_palitra!=1){
    if(__builtin_expect(kj==0,1))wrtd(color[0],color[1],color[2],size);else kj--; 
};

copy_mm(6,0,h-6,w,display3);

if(nn==2||fcl_palitra==1){
    int *scolor= call_palitra(10,10);
    if(scolor[0]!=-1){
        nn=0;
        color[1]=scolor[1];
        color[2]=scolor[2];
        color[0]=scolor[0];
        kj =20;}}
return 0;}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    printf("start\n");
    draw_window(argc,argv);   
    return 0;
}
 