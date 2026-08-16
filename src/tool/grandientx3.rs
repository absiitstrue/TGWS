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

int tgws_draw_grandient_line_to_color(int r1,int g1,int b1,int colors[],int x,int y,int fgh,int fgw){
if(fgh/2>h||fgw>w){printf("Error uncorect size.\n");return 1;}
if(fgh==-1){fgh=h*2;};if(fgw==-1){fgw=w;};

int siz=0;
for(;colors[siz]!=-1;siz++){}siz++;
float shg[siz-1];shg[siz-2]=0.0;shg[siz-3]=0.0;shg[siz-4]=0.0;
int nshg=round((float)fgw/(float)((siz-3)/3));
for(int i=0;i<siz-4;i++){
shg[i]=((float)colors[i+3]-(float)colors[i])/nshg;
}
int linegrand[fgw*3+10];
for(int i=0;i<fgw*3;i+=3){
int px =i/3;
int seg=px/nshg;
int offset = px % nshg;

    int base = seg *3;

linegrand[i] = round(colors[base] + shg[base] * (float)offset);
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
int r2 = linegrand[cw*3]+shv[cw*3]*ch;
int g2 = linegrand[cw*3+1]+shv[cw*3+1]*ch;
int b2 = linegrand[cw*3+2]+shv[cw*3+2]*ch;


int rh = linegrand[cw*3]+shv[cw*3]*(ch+1);
int gh = linegrand[cw*3+1]+shv[cw*3+1]*(ch+1);
int bh = linegrand[cw*3+2]+shv[cw*3+2]*(ch+1);

int ox=x+ch/2;int oy=y+cw;
wrdisplay2(ox,oy,"\033[38;2;%i;%i;%im\033[48;2;%i;%i;%im\u2580\033[0m"
    ,r2,g2,b2,rh,gh,bh);
};
};
return 0;
}
