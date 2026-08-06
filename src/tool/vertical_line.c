int tgws_draw_vertical_line(int rr,int r,int g,int b, int y,int x, int fgh, int fgw){

    for(int i =0;i<fgh;i++){
        wrdisplay2(y+i,x,"\033[38;2;%i;%i;%im\u2502",r,g,b);
    };
return 0;};