int tgws_draw_square(int r1, int g1, int b1,
int y, int x,
int fgh, int fgw){

for(int i =0;i<fgh;i++){
    for(int ii =0; ii<fgw;ii++){
        wrdisplay2(y+i,x+ii,"\033[38;2;%i;%i;%im\u2588",r1,g1,b1);
    };
};

return 0;}
