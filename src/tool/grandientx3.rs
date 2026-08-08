int tgws_draw_grandient_3color(int r1,int g1,int b1,int rc1,int gc1,int bc1,int rc2,int gc2,int bc2,int x,int y,int fgh,int fgw){
if(fgh/2>h||fgw>w){printf("Error uncorect size.\n");return 1;}
if(fgh==-1){fgh=h*2;};if(fgw==-1){fgw=w;};
float dt2[fgw*3+10];memset(dt2,0,sizeof(dt2));

float shr=(float)(rc1-r1)/(fgw>1?fgw-1:1);
float shg=(float)(gc1-g1)/(fgw>1?fgw-1:1);
float shb=(float)(bc1-b1)/(fgw>1?fgw-1:1);

for(int cw=0;cw<fgw;cw++){

float r=(float)r1+shr*cw;float g=(float)g1+shg*cw;float b=(float)b1+shb*cw;
dt2[cw]=(float)(rc2-r)/(fgh>1?fgh-1:1);
dt2[cw+fgw]=(float)(gc2-g)/(fgh>1?fgh-1:1);
dt2[cw+fgw*2]=(float)(bc2-b)/(fgh>1?fgh-1:1);
};
for(int ch=0;ch<fgh-1;ch+=2){
for(int cw=0;cw<fgw;cw++){

float r=(float)r1+shr*cw+dt2[cw]*ch;int r2=round(r);int rh=round(r+dt2[cw]);
float g=(float)g1+shg*cw+dt2[cw+fgw]*ch;int g2=round(g);int gh=round(g+dt2[cw+fgw]);
float b=(float)b1+shb*cw+dt2[cw+fgw*2]*ch;int b2=round(b);int bh=round(b+dt2[cw+fgw*2]);



int ox=x+ch/2;int oy=y+cw;
wrdisplay2(ox,oy,"\033[38;2;%i;%i;%im\033[48;2;%i;%i;%im\u2580\033[0m"
    ,r2,g2,b2,rh,gh,bh);
};
};
return 0;
}