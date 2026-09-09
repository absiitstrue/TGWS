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
  //  tgws_print_label_c16(i,8+i,13,form);
    }
  tgws_draw_border(2,255,255,255,6,12,18,13);
  tgws_print_label(7,16,"16 bit");


  for(int i=0;i<256;i++){
    int row = i/16;
    int col = i%16;
    sprintf(form,"\033[38;5;%im\u2588",i);
    //fwrdisplay2(8+row,29+col*2,form);
    //fwrdisplay2(8+row,29+col*2+1,form);
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
int conf=0;
int red=0;
     int colors[13];
  int pound[11];
void dr_page3(void){

  pound[10]=-1;colors[12]=-1;

  sprintf(form,"its real config-- @/@1000 @/@100 @/@10 @/@1;conf=%03i",conf);
tgws_print_label(42,12,form);

int nn = tgws_set_button_classic(42,30,0,0);tgws_print_label(42,30,"+");if(nn==2)conf+=1000;
    nn = tgws_set_button_classic(42,38,0,0);tgws_print_label(42,38,"+");if(nn==2)conf+=100;
    nn = tgws_set_button_classic(42,45,0,0);tgws_print_label(42,45,"+");if(nn==2)conf+=10;
    nn = tgws_set_button_classic(42,51,0,0);tgws_print_label(42,51,"+");if(nn==2)conf+=1;

    nn = tgws_set_button_classic(42,32,0,0);tgws_print_label(42,32,"-");if(nn==2)conf-=1000;
    nn = tgws_set_button_classic(42,40,0,0);tgws_print_label(42,40,"-");if(nn==2)conf-=100;
    nn = tgws_set_button_classic(42,47,0,0);tgws_print_label(42,47,"-");if(nn==2)conf-=10;
    nn = tgws_set_button_classic(42,53,0,0);tgws_print_label(42,53,"-");if(nn==2)conf-=1;

  sprintf(form,"remove number in [] re--%i",red);
  tgws_print_label(42,67,form);
    nn = tgws_set_button_classic(42,93,0,0);tgws_print_label(42,93,"+/");if(nn==2)red++;
    nn = tgws_set_button_classic(42,95,0,0);tgws_print_label(42,95,"-");if(nn==2)red--;

  if(red<0)red=9;
   if(red>9)red=0;


    nn = tgws_set_button_classic(42,97,0,0);tgws_print_label(42,97,"+/");if(nn==2)pound[red]++;
    nn = tgws_set_button_classic(42,99,0,0);tgws_print_label(42,99,"-");if(nn==2)pound[red]--;
  
  tgws_draw_grafic(conf,colors,pound,6,12,35,117);

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

  colors[1]=0;
  colors[2]=0;
  colors[3]=175;
  colors[4]=0;
  colors[5]=200;
  colors[0]=255;
  colors[6]=0;
  colors[7]=200;
  colors[8]=200;
  colors[9]=0;
  colors[10]=0;
  colors[11]=255;

  pound[0]=10;
  pound[1]=12;
  pound[2]=19;
  pound[3]=8;
  pound[4]=6;
  pound[5]=10;
  pound[6]=13;
  pound[7]=18;
  pound[8]=20;
  pound[9]=19;


  int page =3,nn=0;
  const char *spisok[]={
    "I couldn't be bothered.",
    "I couldn't be bothered.",
    "color",
    "statics",
    "I couldn't be bothered."
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
