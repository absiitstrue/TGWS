#include "../tgws.h"
#include <stdio.h>


char form[64];


//page 0
    void dr_page0(void){
    
    }
//page 1
    void dr_page1(void){

    }
//page 2

static int truecolor[22];

void dr_page2(void){
  for(int i=0;i<16;i++){
    sprintf(form,"color=%i",i);
    tgws_print_label_c16(i,8+i,13,form);
    }
  tgws_draw_border(2,255,255,255,6,12,18,13);
  tgws_print_label(7,16,"16 bit");


  for(int i=0;i<256;i++){
    int row = i/16;
    int col = i%16;
    sprintf(form,"\033[38;5;%im\u2588",i);
    fwrdisplay2(8+row,29+col*2,form);
    fwrdisplay2(8+row,29+col*2+1,form);
  }
  tgws_draw_border(2,255,255,255,6,28,18,34);
  tgws_print_label(7,37,"256 bit color");

  tgws_call_palitra(6,64);
  tgws_print_label(40,110,"                ");
  
  tgws_print_label(42,64,"test button>>  |1|  |2|  |3|  |4|  |5|");

  int nn = tgws_set_button_classic(42,80,0,0);

  if(nn==2){
    int copyb[3];
    copyb[0]=truecolor[0];
    copyb[1]=truecolor[1];
    copyb[2]=truecolor[2];

    for(int i=0;i<=15;i++){
      truecolor[i]=truecolor[i+3];
    }

    truecolor[15]=copyb[0];
    truecolor[16]=copyb[1];
    truecolor[17]=copyb[2];
  }

  truecolor[18]=truecolor[0];
  truecolor[19]=truecolor[1];
  truecolor[20]=truecolor[2];
  nn = tgws_set_button_fix(42,85,0,0,0);
  if(nn){
    tgws_draw_border(2,255,255,255,6,12,35,117);
    tgws_draw_grandient_line_to_color(0,0,0,truecolor,7,13,34*2,116);
  }
  else {
  tgws_draw_border(2,255,255,255,25,12,17,50);
  tgws_draw_grandient_line_to_color(0,0,0,truecolor,26,13,16*2,49);
  }

}
//page 3
    void dr_page3(void){

    }
//page 4
    void dr_page4(void){

    }

int main(int argc,char *argv[]){
  tgws_init_window(argc,argv);

  truecolor[21]=-1;
  truecolor[0]  = 255; truecolor[1]  = 0; truecolor[2]  = 0;
  truecolor[3]  = 255; truecolor[4]  = 255; truecolor[5]  = 0;
  truecolor[6]  = 0; truecolor[7]  = 255; truecolor[8]  = 0;
  truecolor[9]  = 0; truecolor[10] = 255; truecolor[11] = 255;
  truecolor[12] = 0; truecolor[13] = 0; truecolor[14] = 255;
  truecolor[15] = 255; truecolor[16] = 0; truecolor[17] = 255;

  int page =2,nn=0;
  const char *spisok[]={
    "TGWS",
    "htop",
    "color",
    "statics",
    "0"
  };
    while(1){
    int nn=0;
    int y[1],x[1];
      y[0]=2,x[0]=-1;
    tgws_draw_tablr_ofs(2,255,255,255,3,10,40,120,y,x);
    tgws_print_label(3,55,"[TGWS demo]");
    
    //button page
    tgws_print_label(4,11,"<<");
    tgws_print_label(4,128,">>");
    sprintf(form,"page:%i; it's %s",page,spisok[page]);
    tgws_print_label(4,55,form);
    nn = tgws_set_button_classic(4,11,0,1);
    if(nn==2){page--;nn=0;}
    nn = tgws_set_button_classic(4,128,0,1);
    if(nn==2){page++;nn=0;}
    
    if(page<=-1)page=4;
    if(page>=5)page=0;

    void (*pages[])(void)={
      dr_page0,
      dr_page1,
      dr_page2,
      dr_page3,
      dr_page4

    };
      pages[page]();


    tgws_draw_element();
  }
  tgws_close_window();
return 0;}
