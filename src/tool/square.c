int tgws_draw_square(int r1, int g1, int b1,
int y, int x,
int fgh, int fgw){

for(int i =0;i<fgh;i++){
    for(int ii =0; ii<fgw;ii++){
        fastwrdisplay2c(y+i,x+ii,"\u2588",r1,g1,b1);
    };
};

return 0;}
