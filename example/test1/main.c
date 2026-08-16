#include "../tgws.h"
#include <math.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]){

//srand(time(NULL));
tgws_init_window(argc, argv);
int chesh[64];
  for(int i =0; i<32;i++){chesh[16+i]=0;}
  for(int i =0; i<16;i++){
    if(i<=7)chesh[8+i]=1;
    else chesh[40+i]=1;}
  chesh[0]=2;chesh[7]=2;
  chesh[1]=3;chesh[6]=3;
  chesh[2]=4;chesh[5]=4;
  chesh[3]=6;chesh[4]=5;
  

  for(int i=0;i<8;i++){
    chesh[56+i]=chesh[i];
  }

  //0 = free
  //1= peshka
  //2=ferz
  //3=kon
  //4=slon
  //5 = koroleva
  //6 = korol

  int dy[8];dy[7]=-1;
  for(int i=0;i<7;i++){
    dy[i]=i*8+8;
  }

  int dx[8];dx[7]=-1;
  for(int i=0;i<7;i++){
    dx[i]=16*i+16;
  }

  while(1){

    tgws_draw_tablr_ofs(255,255,255,1,1,8*8,8*16,dy,dx);

    for(int i=0;i<7;i++){
    char str[10];
    sprintf(str,"%i",i+1);
    tgws_print_label(2+i,2+i,str);}

    tgws_draw_element();
  }
return 0;}
