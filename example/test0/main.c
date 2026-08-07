#include "../../src/tgws.c"
int main(int argc, char *argv[]) {
    srand(time(NULL));
    tgws_init_window(argc,argv);
    int i=30,sy=0,sx=0;
  float r = 0.00;
    while(1){
    r+=0.1;if(r>=6.28)r=0.00;
  
      sy = (int)(sin(r)*15)+20;
      sx = (int)(cos(r)*30)+40;

    
    tgws_draw_line_m(255,0,0,20,40,sy,sx);
        tgws_draw_element();
    }


    return 0;
}

