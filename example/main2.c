int html(void);

#include "../src/tgws.c"

int html() {

    square(255,255,255,10,10,5,10);
    square(255,255,255,16,10,5,10);
    square(255,255,255,22,10,5,10);
    square(255,255,255,28,10,5,10);

    int aa[17]={0};aa[16]=-1;
    aa[0]=10; aa[1]=10; aa[2]=5; aa[3]=10;
    aa[4]=16; aa[5]=10; aa[6]=5; aa[7]=10;
    aa[8]=22; aa[9]=10; aa[10]=5; aa[11]=10;
    aa[12]=28; aa[13]=10; aa[14]=5; aa[15]=10;    
    int vv = button_radio(aa,1);char kk[20];sprintf(kk,"%i",vv);label(8,12,kk);
return 0;}
int main(int argc, char *argv[]) {
    srand(time(NULL));
    printf("start\n");
    draw_window(argc,argv);   
    return 0;
}
 