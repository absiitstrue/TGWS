int tgws_draw_tablr_place(int r,int g,int b, int y, int x, int fgh, int fgw ,int *xy){

    fastwrdisplay2c(y,x,"\u250c",r,g,b);fastwrdisplay2c(y+fgh,x,"\u2514",r,g,b);fastwrdisplay2c(y,x+fgw,"\u2510",r,g,b);fastwrdisplay2c(y+fgh,x+fgw,"\u2518",r,g,b);
for(int i =1;i<fgh;i++){fastwrdisplay2c(y+i,x,"\u2502",r,g,b);fastwrdisplay2c(y+i,x+fgw,"\u2502",r,g,b);};
for(int ii =1;ii<fgw;ii++){fastwrdisplay2c(y,x+ii,"\u2500",r,g,b);fastwrdisplay2c(y+fgh,x+ii,"\u2500",r,g,b);};

    for(int i =0;xy[i]!=-1;i+=2){
int lx = xy[i+1], ly =xy[i];

if(ly == y){fastwrdisplay2c(ly,lx,"\u252C",r,g,b);fastwrdisplay2c(ly+fgh,lx,"\u2534",r,g,b);
for(int z =1;z<fgh;z++){fastwrdisplay2c(y+z,lx,"\u2502",r,g,b);};
}
if(lx == x){fastwrdisplay2c(ly,lx,"\u251C",r,g,b);fastwrdisplay2c(ly,lx+fgw,"\u2524",r,g,b);
for(int z =1;z<fgw;z++){fastwrdisplay2c(ly,x+z,"\u2500",r,g,b);};


};};
    for(int i =0;xy[i]!=-1;i+=2){
        if(xy[i+1]==x){for(int j =0;xy[j]!=-1;j+=2){
            if(xy[j]==y){
                fastwrdisplay2c(xy[i],xy[j+1],"\u253C", r, g, b);
            }
        }}
    }


    return 0;
}

int tgws_draw_tablr_ofs(int rr,int r,int g,int b, int y, int x, int fgh, int fgw ,int *cy,int *cx){

fastwrdisplay2c(y,x,"\u250c",r,g,b);fastwrdisplay2c(y+fgh,x,"\u2514",r,g,b);fastwrdisplay2c(y,x+fgw,"\u2510",r,g,b);fastwrdisplay2c(y+fgh,x+fgw,"\u2518",r,g,b);
for(int i =1;i<fgh;i++){fastwrdisplay2c(y+i,x,"\u2502",r,g,b);fastwrdisplay2c(y+i,x+fgw,"\u2502",r,g,b);}
for(int ii =1;ii<fgw;ii++){fastwrdisplay2c(y,x+ii,"\u2500",r,g,b);fastwrdisplay2c(y+fgh,x+ii,"\u2500",r,g,b);}

for(int i= 0;cx[i]!=-1;i++){

    fastwrdisplay2c(y,x+cx[i],"\u252C",r,g,b);fastwrdisplay2c(y+fgh,x+cx[i],"\u2534",r,g,b);
for(int z =1;z<fgh;z++){fastwrdisplay2c(y+z,x+cx[i],"\u2502",r,g,b);}}

for(int i=0;cy[i]!=-1;i++){
    fastwrdisplay2c(cy[i]+y,x,"\u251C",r,g,b);fastwrdisplay2c(cy[i]+y,x+fgw,"\u2524",r,g,b);
for(int z =1;z<fgw;z++){fastwrdisplay2c(y+cy[i],x+z,"\u2500",r,g,b);}}

for(int i=0;cx[i]!=-1;i++){
    for(int ii=0;cy[ii]!=-1;ii++){
                fastwrdisplay2c(cy[ii]+y,cx[i]+x,"\u253C", r, g, b);}}

return 0;}

