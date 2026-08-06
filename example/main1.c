#include "../src/tgws.c"
int main(int argc, char *argv[]) {
    srand(time(NULL));
    tgws_init_window(argc,argv);
    while(1){
        int nn =0;
        nn= tgws_draw_polzun_horisontal(255,0,0,255,10,20,2,30,1,-1);
        tgws_draw_square(nn,255,255,5,10,5,10);
        tgws_draw_element();
    }


    return 0;
}
