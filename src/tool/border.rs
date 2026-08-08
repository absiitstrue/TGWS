int tgws_draw_border(int br,
int rr, int rg, int rb,
int y,int x,
int fgw, int fgh){
    
    if(br ==1){wrdisplay2(y,x,"\033[38;2;%i;%i;%im\u250c",rr,rg,rb);wrdisplay2(y+fgw,x,"\033[38;2;%i;%i;%im\u2514",rr,rg,rb);wrdisplay2(y,x+fgh,"\033[38;2;%i;%i;%im\u2510",rr,rg,rb);wrdisplay2(y+fgw,x+fgh,"\033[38;2;%i;%i;%im\u2518",rr,rg,rb);
    for(int i =1;i<fgw;i++){wrdisplay2(y+i,x,"\033[38;2;%i;%i;%im\u2502",rr,rg,rb);wrdisplay2(y+i,x+fgh,"\033[38;2;%i;%i;%im\u2502",rr,rg,rb);};
    for(int i =1;i<fgh;i++){wrdisplay2(y,x+i,"\033[38;2;%i;%i;%im\u2500",rr,rg,rb);wrdisplay2(y+fgw,x+i,"\033[38;2;%i;%i;%im\u2500",rr,rg,rb);};}

    if(br ==2){wrdisplay2(y,x,"\033[38;2;%i;%i;%im\u2554",rr,rg,rb);wrdisplay2(y+fgw,x,"\033[38;2;%i;%i;%im\u255A",rr,rg,rb);wrdisplay2(y,x+fgh,"\033[38;2;%i;%i;%im\u2557",rr,rg,rb);wrdisplay2(y+fgw,x+fgh,"\033[38;2;%i;%i;%im\u255D",rr,rg,rb);
    for(int i =1;i<fgw;i++){wrdisplay2(y+i,x,"\033[38;2;%i;%i;%im\u2551",rr,rg,rb);wrdisplay2(y+i,x+fgh,"\033[38;2;%i;%i;%im\u2551",rr,rg,rb);};
    for(int i =1;i<fgh;i++){wrdisplay2(y,x+i,"\033[38;2;%i;%i;%im\u2550",rr,rg,rb);wrdisplay2(y+fgw,x+i,"\033[38;2;%i;%i;%im\u2550",rr,rg,rb);};}

    if(br ==3){wrdisplay2(y,x,"\033[38;2;%i;%i;%im\u250F",rr,rg,rb);wrdisplay2(y+fgw,x,"\033[38;2;%i;%i;%im\u2517",rr,rg,rb);wrdisplay2(y,x+fgh,"\033[38;2;%i;%i;%im\u2513",rr,rg,rb);wrdisplay2(y+fgw,x+fgh,"\033[38;2;%i;%i;%im\u251B",rr,rg,rb);
    for(int i =1;i<fgw;i++){wrdisplay2(y+i,x,"\033[38;2;%i;%i;%im\u2503",rr,rg,rb);wrdisplay2(y+i,x+fgh,"\033[38;2;%i;%i;%im\u2503",rr,rg,rb);};
    for(int i =1;i<fgh;i++){wrdisplay2(y,x+i,"\033[38;2;%i;%i;%im\u2501",rr,rg,rb);wrdisplay2(y+fgw,x+i,"\033[38;2;%i;%i;%im\u2501",rr,rg,rb);};}

    return 0;
};