int tgws_draw_border(int br,
int rr, int rg, int rb,
int y,int x,
int fgw, int fgh){
    
    if(br ==1){fastwrdisplay2c(y,x,"\u250c",rr,rg,rb);fastwrdisplay2c(y+fgw,x,"\u2514",rr,rg,rb);fastwrdisplay2c(y,x+fgh,"\u2510",rr,rg,rb);fastwrdisplay2c(y+fgw,x+fgh,"\u2518",rr,rg,rb);
    for(int i =1;i<fgw;i++){fastwrdisplay2c(y+i,x,"\u2502",rr,rg,rb);fastwrdisplay2c(y+i,x+fgh,"\u2502",rr,rg,rb);};
    for(int i =1;i<fgh;i++){fastwrdisplay2c(y,x+i,"\u2500",rr,rg,rb);fastwrdisplay2c(y+fgw,x+i,"\u2500",rr,rg,rb);};}

    if(br ==2){fastwrdisplay2c(y,x,"\u2554",rr,rg,rb);fastwrdisplay2c(y+fgw,x,"\u255A",rr,rg,rb);fastwrdisplay2c(y,x+fgh,"\u2557",rr,rg,rb);fastwrdisplay2c(y+fgw,x+fgh,"\u255D",rr,rg,rb);
    for(int i =1;i<fgw;i++){fastwrdisplay2c(y+i,x,"\u2551",rr,rg,rb);fastwrdisplay2c(y+i,x+fgh,"\u2551",rr,rg,rb);};
    for(int i =1;i<fgh;i++){fastwrdisplay2c(y,x+i,"\u2550",rr,rg,rb);fastwrdisplay2c(y+fgw,x+i,"\u2550",rr,rg,rb);};}

    if(br ==3){fastwrdisplay2c(y,x,"\u250F",rr,rg,rb);fastwrdisplay2c(y+fgw,x,"\u2517",rr,rg,rb);fastwrdisplay2c(y,x+fgh,"\u2513",rr,rg,rb);fastwrdisplay2c(y+fgw,x+fgh,"\u251B",rr,rg,rb);
    for(int i =1;i<fgw;i++){fastwrdisplay2c(y+i,x,"\u2503",rr,rg,rb);fastwrdisplay2c(y+i,x+fgh,"\u2503",rr,rg,rb);};
    for(int i =1;i<fgh;i++){fastwrdisplay2c(y,x+i,"\u2501",rr,rg,rb);fastwrdisplay2c(y+fgw,x+i,"\u2501",rr,rg,rb);};}

    return 0;
};
