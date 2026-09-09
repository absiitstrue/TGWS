int tgws_draw_gradient_super(int mode,int r1,int g1,int b1,int r2,int g2,int b2,int r3,int g3,int b3,int x,int y,int fgh,int fgw,int *colors){
if(fgh/2>h||fgw>w){printf("Error uncorect size.\n");return 1;}
if(fgh==-1){fgh=h*2-y*2;};if(fgw==-1){fgw=w-x;};
if(mode==-5){
int siz=0;for(;colors[siz]!=-1;siz++){}siz++;
float shg[siz-1];shg[siz-2]=0.0;shg[siz-3]=0.0;shg[siz-4]=0.0;
int nshg=round((float)fgw/(float)((siz-3)/3));
for(int i=0;i<siz-4;i++){shg[i]=((float)colors[i+3]-(float)colors[i])/nshg;}
int linegrand[fgw*3+10];
for(int i=0;i<fgw*3;i+=3){
int px=i/3;int seg=px/nshg;int offset=px%nshg;int base=seg*3;
linegrand[i]=round(colors[base]+shg[base]*(float)offset);
linegrand[i+1]=round(colors[base+1]+shg[base+1]*(float)offset);
linegrand[i+2]=round(colors[base+2]+shg[base+2]*(float)offset);
}
float shv[3*fgw+10];
for(int i=0;i<fgw*3;i+=3){
shv[i]=(float)(r1-linegrand[i])/(float)fgh;
shv[i+1]=(float)(g1-linegrand[i+1])/(float)fgh;
shv[i+2]=(float)(b1-linegrand[i+2])/(float)fgh;
}
for(int ch=0;ch<fgh-1;ch+=2){
for(int cw=0;cw<fgw;cw++){
int r2c=linegrand[cw*3]+shv[cw*3]*ch;int g2c=linegrand[cw*3+1]+shv[cw*3+1]*ch;int b2c=linegrand[cw*3+2]+shv[cw*3+2]*ch;
int rh=linegrand[cw*3]+shv[cw*3]*(ch+1);int gh=linegrand[cw*3+1]+shv[cw*3+1]*(ch+1);int bh=linegrand[cw*3+2]+shv[cw*3+2]*(ch+1);
int ox=x+ch/2;int oy=y+cw;
fastwrdisplay2(ox,oy,"\u2580",r2c,g2c,b2c,rh,gh,bh);
};};
return 0;
}
if(mode>0){
float ang=(float)mode*0.0174532925;
float cosv=cos(ang);float sinv=sin(ang);
for(int ch=0;ch<fgh-1;ch+=2){
for(int cw=0;cw<fgw;cw++){
float nx=(float)cw/(fgw>1?fgw-1:1);float ny=(float)ch/(fgh>1?fgh-1:1);
float rotx=nx*cosv-ny*sinv;
float r=(float)r1+rotx*(r2-r1);
float g=(float)g1+rotx*(g2-g1);
float b=(float)b1+rotx*(b2-b1);
int r2c=round(r);int rhc=round(r);int g2c=round(g);int ghc=round(g);int b2c=round(b);int bhc=round(b);
int ox=x+ch/2;int oy=y+cw;
fastwrdisplay2(ox,oy,"\u2580",r2c,g2c,b2c,rhc,ghc,bhc);
};};
return 0;
}
if(mode==-2){
float dt2[fgw*3+10];memset(dt2,0,sizeof(dt2));
float shr=(float)(r2-r1)/(fgw>1?fgw-1:1);
float shg=(float)(g2-g1)/(fgw>1?fgw-1:1);
float shb=(float)(b2-b1)/(fgw>1?fgw-1:1);
for(int cw=0;cw<fgw;cw++){
float r=(float)r1+shr*cw;float g=(float)g1+shg*cw;float b=(float)b1+shb*cw;
dt2[cw]=(float)(r3-r)/(fgh>1?fgh-1:1);
dt2[cw+fgw]=(float)(g3-g)/(fgh>1?fgh-1:1);
dt2[cw+fgw*2]=(float)(b3-b)/(fgh>1?fgh-1:1);
};
for(int ch=0;ch<fgh-1;ch+=2){
for(int cw=0;cw<fgw;cw++){
float r=(float)r1+shr*cw+dt2[cw]*ch;int r2c=round(r);int rhc=round(r+dt2[cw]);
float g=(float)g1+shg*cw+dt2[cw+fgw]*ch;int g2c=round(g);int ghc=round(g+dt2[cw+fgw]);
float b=(float)b1+shb*cw+dt2[cw+fgw*2]*ch;int b2c=round(b);int bhc=round(b+dt2[cw+fgw*2]);
int ox=x+ch/2;int oy=y+cw;
fastwrdisplay2(ox,oy,"\u2580",r2c,g2c,b2c,rhc,ghc,bhc);
};};
return 0;
}
float shr=(float)(r2-r1)/(fgw>1?fgw-1:1);
float shg=(float)(g2-g1)/(fgw>1?fgw-1:1);
float shb=(float)(b2-b1)/(fgw>1?fgw-1:1);
float shvr=(float)(r3-r1)/(fgh>1?fgh-1:1);
float shvg=(float)(g3-g1)/(fgh>1?fgh-1:1);
float shvb=(float)(b3-b1)/(fgh>1?fgh-1:1);
for(int ch=0;ch<fgh-1;ch+=2){
for(int cw=0;cw<fgw;cw++){
float r=(float)r1+shr*cw+shvr*ch;int r2c=round(r);int rhc=round(r+shvr);
float g=(float)g1+shg*cw+shvg*ch;int g2c=round(g);int ghc=round(g+shvg);
float b=(float)b1+shb*cw+shvb*ch;int b2c=round(b);int bhc=round(b+shvb);
int ox=x+ch/2;int oy=y+cw;
fastwrdisplay2(ox,oy,"\u2580",r2c,g2c,b2c,rhc,ghc,bhc);
};};
return 0;
}
