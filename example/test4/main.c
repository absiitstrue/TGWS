#include <math.h>
#include <stdio.h>
#include "../tgws.h"

int main(int argc, char *argv[]){

  tgws_init_window(argc,argv);
  int see = 20;
  float seer = (float)see/180*3.14;
char str[64];
  int vrx=20,vry=10,plx=86,ply=40;
int dd=0;

  while(1){


for(int i =see-20;i<=see+20;i++){
      float g =i;
      g = g/180*3.14;
      int sy = round(sin(g)*20+10);
      int sx = round(cos(g)*40+20);
      tgws_draw_line_p_char(255,255,255,10,20,sy,sx,"."); 
      tgws_print_label(sy,sx,".");
    }

    tgws_print_label(vry,vrx,"@");
    tgws_print_label(ply,plx,"#");

    tgws_print_label(vry+sin(seer)*20,vrx+cos(seer)*40,"0");

    float vecvpy =(float)ply-vry;
    float vecvpx =(float)(plx-vrx)/2.0;
    float rustplvr = sqrt(vecvpx*vecvpx+vecvpy*vecvpy);


    sprintf(str,"rustoaynie -- %f,see--%i",rustplvr,dd);
    tgws_print_label(0,0,str);
   
    dd=0;
    float norm[4];
      norm[0]= (float)vecvpx/rustplvr;
      norm[1]= (float)vecvpy/rustplvr;
      norm[2]= cosf(seer);
      norm[3]= sinf(seer);

      float dot = norm[0]*norm[2]+norm[1]*norm[3];
      if(dot>0.9300 && rustplvr<=20){
      dd=1;  
      }

    if(kbp==1){
        if(kbb=='w')ply--;
        else if(kbb=='s')ply++;
        else if(kbb=='a')plx--;
        else if(kbb=='d')plx++;
      }

    

tgws_draw_element();
  }
tgws_close_window();
return 0;}
