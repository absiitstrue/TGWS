int tgws_set_button_classic(
int y,int x,
int fgh, int fgw){
int g =0;
    if(mx>=x && my>=y && mx<=fgw+x && my<=y+fgh){g=1;
    if(mbtn ==0&&prew!=0){g=2;}
    else if(mbtn ==2&&prew!=2){g=3;}}
    return g;
}

int tgws_set_button_radio(int *b,int numb){
    static int ss[100]={-1};
    for(int i =0;b[i]!=-1;i+=4){
        if( my>=b[i] && mx>=b[i+1] && my<=b[i]+b[i+2] && mx<=b[i+1] + b[i+3] && mbtn ==0){
            ss[numb] = 1 + (i +1)/4;
        };
    };

    return ss[numb];
}
int tgws_set_button_fix(int y,int x, int fgh, int fgw,int numb){
static int bb[300],ba[300];
if(mx>=x && my>=y && mx<=fgw+x && my<=y+fgh && mbtn==0 &&ba[numb]!=0){
    if(bb[numb]==0)bb[numb]=1;
    else bb[numb]=0;
}

ba[numb]=mbtn;return bb[numb];}
