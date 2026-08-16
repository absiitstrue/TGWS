int tgws_draw_tablr_place(int r,int g,int b, int y, int x, int fgh, int fgw ,int *xy){

    wrdisplay2(y,x,"\033[38;2;%i;%i;%im\u250c",r,g,b);wrdisplay2(y+fgh,x,"\033[38;2;%i;%i;%im\u2514",r,g,b);wrdisplay2(y,x+fgw,"\033[38;2;%i;%i;%im\u2510",r,g,b);wrdisplay2(y+fgh,x+fgw,"\033[38;2;%i;%i;%im\u2518",r,g,b);
for(int i =1;i<fgh;i++){wrdisplay2(y+i,x,"\033[38;2;%i;%i;%im\u2502",r,g,b);wrdisplay2(y+i,x+fgw,"\033[38;2;%i;%i;%im\u2502",r,g,b);};
for(int ii =1;ii<fgw;ii++){wrdisplay2(y,x+ii,"\033[38;2;%i;%i;%im\u2500",r,g,b);wrdisplay2(y+fgh,x+ii,"\033[38;2;%i;%i;%im\u2500",r,g,b);};

    for(int i =0;xy[i]!=-1;i+=2){
int lx = xy[i+1], ly =xy[i];

if(ly == y){wrdisplay2(ly,lx,"\033[38;2;%i;%i;%im\u252C",r,g,b);wrdisplay2(ly+fgh,lx,"\033[38;2;%i;%i;%im\u2534",r,g,b);
for(int z =1;z<fgh;z++){wrdisplay2(y+z,lx,"\033[38;2;%i;%i;%im\u2502",r,g,b);};
}
if(lx == x){wrdisplay2(ly,lx,"\033[38;2;%i;%i;%im\u251C",r,g,b);wrdisplay2(ly,lx+fgw,"\033[38;2;%i;%i;%im\u2524",r,g,b);
for(int z =1;z<fgw;z++){wrdisplay2(ly,x+z,"\033[38;2;%i;%i;%im\u2500",r,g,b);};


};};
    for(int i =0;xy[i]!=-1;i+=2){
        if(xy[i+1]==x){for(int j =0;xy[j]!=-1;j+=2){
            if(xy[j]==y){
                wrdisplay2(xy[i],xy[j+1],"\033[38;2;%i;%i;%im\u253C", r, g, b);
            }
        }}
    }


    return 0;
}

int tgws_draw_tablr_ofs(int rr,int r,int g,int b, int y, int x, int fgh, int fgw ,int *cy,int *cx){

wrdisplay2(y,x,"\033[38;2;%i;%i;%im\u250c",r,g,b);wrdisplay2(y+fgh,x,"\033[38;2;%i;%i;%im\u2514",r,g,b);wrdisplay2(y,x+fgw,"\033[38;2;%i;%i;%im\u2510",r,g,b);wrdisplay2(y+fgh,x+fgw,"\033[38;2;%i;%i;%im\u2518",r,g,b);
for(int i =1;i<fgh;i++){wrdisplay2(y+i,x,"\033[38;2;%i;%i;%im\u2502",r,g,b);wrdisplay2(y+i,x+fgw,"\033[38;2;%i;%i;%im\u2502",r,g,b);}
for(int ii =1;ii<fgw;ii++){wrdisplay2(y,x+ii,"\033[38;2;%i;%i;%im\u2500",r,g,b);wrdisplay2(y+fgh,x+ii,"\033[38;2;%i;%i;%im\u2500",r,g,b);}

for(int i= 0;cx[i]!=-1;i++){

    wrdisplay2(y,x+cx[i],"\033[38;2;%i;%i;%im\u252C",r,g,b);wrdisplay2(y+fgh,x+cx[i],"\033[38;2;%i;%i;%im\u2534",r,g,b);
for(int z =1;z<fgh;z++){wrdisplay2(y+z,x+cx[i],"\033[38;2;%i;%i;%im\u2502",r,g,b);}}

for(int i=0;cy[i]!=-1;i++){
    wrdisplay2(cy[i]+y,x,"\033[38;2;%i;%i;%im\u251C",r,g,b);wrdisplay2(cy[i]+y,x+fgw,"\033[38;2;%i;%i;%im\u2524",r,g,b);
for(int z =1;z<fgw;z++){wrdisplay2(y+cy[i],x+z,"\033[38;2;%i;%i;%im\u2500",r,g,b);}}

for(int i=0;cx[i]!=-1;i++){
    for(int ii=0;cy[ii]!=-1;ii++){
                wrdisplay2(cy[ii]+y,cx[i]+x,"\033[38;2;%i;%i;%im\u253C", r, g, b);}}

return 0;}

