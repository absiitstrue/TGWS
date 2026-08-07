#include "../../src/tgws.c"
#define deg90 1.57


void unicode_to_utf8(uint32_t codepoint, char *out_str) {
    if (codepoint <= 0x7F) {
        out_str[0] = (char)codepoint;
        out_str[1] = 0;
    } else if (codepoint <= 0x7FF) {
        out_str[0] = (char)(0xC0 | (codepoint >> 6));
        out_str[1] = (char)(0x80 | (codepoint & 0x3F));
        out_str[2] = 0;
    } else if (codepoint <= 0xFFFF) {
        out_str[0] = (char)(0xE0 | (codepoint >> 12));
        out_str[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out_str[2] = (char)(0x80 | (codepoint & 0x3F));
        out_str[3] = 0;
    } else {
        out_str[0] = (char)(0xF0 | (codepoint >> 18));
        out_str[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        out_str[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out_str[3] = (char)(0x80 | (codepoint & 0x3F));
        out_str[4] = 0;
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    tgws_init_window(argc,argv);
  

  int sy=0,sx=0;
  float sech = (float)6.28/60;
  float houh = (float)6.28/12;
  int nn =0;
  int sumblsec=183,sumblmin=8226,sumblhou=9679;
char form[30];
  int colorss[18];for(int i=0;i<18;i++){colorss[i]=0;
  if(i>8)colorss[i]=255;}
  int kss =0,kk=0,seth=0;

  colorss[0]=255;colorss[4]=255;;colorss[8]=255;

    for(int i =0; i<=100000;i+=0){
    time_t rawtime;
    time(&rawtime);

    struct tm *timeinfo = localtime(&rawtime);

    int hour = timeinfo->tm_hour;
    int minu = timeinfo->tm_min;
    int secu = timeinfo->tm_sec;
for(float i =0.00;i<=6.28;i+=sech){
      sy = roundf(sin(i)*32+40);
      sx = roundf(cos(i)*32+40);
      if(!(sy%2==0))fwrdisplay2(sy/2,sx,"\033[38;2;%i;%i;%im\u2804",colorss[9],colorss[10],colorss[11]);
      else fwrdisplay2(sy/2,sx,"\033[38;2;%i;%i;%im\u2802",colorss[9],colorss[10],colorss[11]);
    }
    
for(float i =0.00;i<=6.28;i+=houh){
      sy = roundf(sin(i)*32+40);
      sx = roundf(cos(i)*32+40);
      if(!(sy%2==0))fwrdisplay2(sy/2,sx,"\033[38;2;%i;%i;%im\u2584",colorss[12],colorss[13],colorss[14]);
      else fwrdisplay2(sy/2,sx,"\033[38;2;%i;%i;%im\u2580",colorss[12],colorss[13],colorss[14]);
    }
    nn = tgws_set_button_fix(4,62,0,10,0);

    if(nn==0){
    if(seth==0)tgws_print_label(4,64,"Setting>>");
      else tgws_print_label(4,72,">");
    tgws_draw_border(1,colorss[15],colorss[16],colorss[17],3,7,34,67);

    }
    else {
    if(seth==0)tgws_print_label(4,64,"Setting<<");
      else tgws_print_label(4,72,"<");

      int borders[3];borders[2]=-1;
      borders[0]=3;borders[1]=74;
    tgws_draw_tablr(colorss[15],colorss[16],colorss[17],3,7,34,78,borders);
      tgws_print_label(4,75,"set symbol");
      //module open secund

      sprintf(form,"\\u-%04X S",sumblsec);
      tgws_print_label(5,75,form);
      tgws_print_label(6,75," |+|+|+|+|");
      tgws_print_label(7,75," |-|-|-|-|");
      for(int i =0;i<4;i++){
        int kk = tgws_set_button_classic(6,77+i*2,0,0);
        if(kk==2)sumblsec+=1<<(4*(3-i));
      }for(int i =0;i<4;i++){
        int kk = tgws_set_button_classic(7,77+i*2,0,0);
        if(kk==2)sumblsec-=1<<(4*(3-i));
      }
      tgws_print_label(8,75,"----------");

      //module close secund and open minut
       sprintf(form,"\\u-%04X M",sumblmin);
      tgws_print_label(9,75,form);
      tgws_print_label(10,75," |+|+|+|+|");
      tgws_print_label(11,75," |-|-|-|-|");
      for(int i =0;i<4;i++){
        int kk = tgws_set_button_classic(10,77+i*2,0,0);
        if(kk==2)sumblmin+=1<<(4*(3-i));
      }for(int i =0;i<4;i++){
        int kk = tgws_set_button_classic(11,77+i*2,0,0);
        if(kk==2)sumblmin-=1<<(4*(3-i));
      }
      tgws_print_label(12,75,"----------");


      //module close minut and open hour
       sprintf(form,"\\u-%04X H",sumblhou);
      tgws_print_label(13,75,form);
      tgws_print_label(14,75," |+|+|+|+|");
      tgws_print_label(15,75," |-|-|-|-|");
      for(int i =0;i<4;i++){
        int kk = tgws_set_button_classic(14,77+i*2,0,0);
        if(kk==2)sumblhou+=1<<(4*(3-i));
      }for(int i =0;i<4;i++){
        int kk = tgws_set_button_classic(15,77+i*2,0,0);
        if(kk==2)sumblhou-=1<<(4*(3-i));
      }
      tgws_print_label(16,75,"----------");


      //module close hour
//color
      tgws_print_label(17,75,"set color");
      tgws_print_label(18,75,"hour --");
      tgws_print_label(21,75,"minute --");
      tgws_print_label(24,75,"seconds --");
      tgws_print_label(27,75,"round --");
      tgws_print_label(30,75,"hour round");
      tgws_print_label(33,75,"border");

      if(seth==0)tgws_print_label(36,75,"hide sett");
      else tgws_print_label(36,75,"how sett");

          seth = tgws_set_button_fix(36,75,0,10,2);
      if(fcl_palitra!=1)for(int i=0;i<6;i++){
        int ot = i*3;
        tgws_draw_square(colorss[ot],colorss[ot+1],colorss[ot+2],19+ot,75,1,9);
        kss = tgws_set_button_classic(18+ot,75,2,9);
        if(kss==2){kk=i;break;}
      }
 if(kss==2||fcl_palitra==1){
     int *scolor= tgws_call_palitra(10,10);
     if(scolor[0]!=-1){
         kss=0;
         colorss[kk*3+1]=scolor[1];
         colorss[kk*3+2]=scolor[2];
         colorss[kk*3]=scolor[0];}
      tgws_draw_element();continue;}                                                                                                                                                                                                
      //close color
    }
    
unicode_to_utf8(sumblhou,form);sy = (int)(sin(houh*hour-deg90)*8)+20;sx = (int)(cos(hour*houh-deg90)*16)+40;tgws_draw_line_m_char(colorss[0],colorss[1],colorss[2],20,40,sy,sx,form);// hour
unicode_to_utf8(sumblmin,form);sy = (int)(sin(sech*minu-deg90)*12)+20;sx = (int)(cos(sech*minu-deg90)*24)+40;tgws_draw_line_m_char(colorss[3],colorss[4],colorss[5],20,40,sy,sx,form);// minut
unicode_to_utf8(sumblsec,form);sy = (int)(sin(sech*secu-deg90)*15)+20;sx = (int)(cos(sech*secu-deg90)*30)+40;tgws_draw_line_m_char(colorss[6],colorss[7],colorss[8],20,40,sy,sx,form);// secunds
        tgws_draw_element();
    }
tgws_close_window();

    return 0;
}

