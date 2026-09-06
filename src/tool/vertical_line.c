int tgws_draw_vertical_line(int rr,int r,int g,int b, int y,int x, int fgh, int fgw){

    for(int i =0;i<fgh;i++){
        fastwrdisplay2c(y+i,x,"\u2502",r,g,b);
    };
return 0;};
