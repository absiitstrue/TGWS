int tgws_draw_polzun_vertical(int r,int g,int b,int max,int y,int x,int fgh,int fgw,int numb,int inint){
static int sp[300] ={[100 ... 199]=-1}; 

if(__builtin_expect(sp[numb+100] == -1,0)){
    sp[numb+100] =y+fgh;
};

float sh;
sh = (float)max/fgh;
if(mx>=x && my>=y && mx<=fgw+x && my<=y+fgh&&(mbtn == 32||mbtn==0)){sp[numb] = round((my-y)*sh);sp[numb] = max-sp[numb];sp[numb+100] = my;}
else if(inint !=-1&&inint !=sp[numb+200]){sp[numb+100]=(y+fgh)-round(inint/sh);sp[numb]=inint;sp[numb+200]=inint;};
int xx = (x+x+fgw)/2;
tgws_draw_square(r,g,b,sp[numb+100],x,fgh-(sp[numb+100]-y),fgw);
fastwrdisplay2w(y,xx,"^");
fastwrdisplay2w(y+fgh,xx,"v");
return sp[numb];};


int tgws_draw_polzun_horisontal(int r,int g,int b,int max,int y,int x,int fgh,int fgw,int numb,int inint){
static int sp1[300] ={[100 ... 199]=-1}; 

if(__builtin_expect(sp1[numb+100] == -1,0)){
    sp1[numb+100] =x;
};

float sh;
sh = (float)max/fgw;

if(mx>=x && my>=y && mx<=fgw+x && my<=y+fgh&&(mbtn == 32||mbtn==0)){sp1[numb] = round((mx-x)*sh);sp1[numb+100] = mx;}
else if(inint !=-1&&inint !=sp1[numb+200]){sp1[numb+100]=x+round(inint/sh);sp1[numb]=inint;sp1[numb+200]=inint;};

int yy = (y+y+fgh)/2;

tgws_draw_square(r,g,b,y,x,fgh,(sp1[numb+100]-x));
fastwrdisplay2w(yy,x,">");
fastwrdisplay2w(yy,fgw+x,"<");
return sp1[numb];};
