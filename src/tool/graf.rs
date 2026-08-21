int tgws_draw_grafic(int rg,int colors[],int pounds[],int y,int x, int fgh, int fgw){
    int r1=rg%10; int r10=(rg/10)%10; int r100=(rg/100)%10; int r1000=(rg/1000)%10;

    for(int gy=y;gy<y+fgh;gy++){
        for(int gx=x;gx<x+fgw;gx++){
            if(r100==3||r100==4) wrdisplay2(gy,gx,".");
            else wrdisplay2(gy,gx," ");
        }
    }

    int colp=0; for(;pounds[colp]!=-1;colp++){};
    int colc=0; for(;colors[colc]!=-1;colc++){}; colc/=3; if(colc==0) colc=1;
    
    int off_x=(r100==1||r100==2||r100==3)?6:0;
    int work_w=fgw-off_x; if(work_w<1) work_w=1;
    float shag=(float)work_w/colp;
    
    int minp=pounds[0]; int maxp=pounds[0];
    for(int i=0;i<colp;i++){minp=(minp>pounds[i])?pounds[i]:minp; maxp=(maxp<pounds[i])?pounds[i]:maxp;};
    float delt=(float)(maxp-minp); if(delt==0) delt=1;
    int vis=fgh-2; if(vis<1) vis=1;

    if(r100==1||r100==2||r100==3){
        int steps=4;
        for(int s=0;s<=steps;s++){
            int val=minp+(int)((maxp-minp)*(float)s/steps);
            int ly=y+fgh-2-(int)((vis-1)*(float)s/steps);
            if(ly>=y&&ly<y+fgh){
                char buf[16]; snprintf(buf,sizeof(buf),"%d-",val);
                for(int k=0;buf[k];k++){if(x+k<x+fgw) wrdisplay2(ly,x+k,"%c",buf[k]);};
            };
        };
    }

    if(r100==2){
        for(int i=0;i<colp;i++){
            int norm=(int)(((pounds[i]-minp)*(float)(vis-1))/delt);
            if(r10==6){int norm8=(int)(((pounds[i]-minp)*(float)(vis-1)*8.0)/delt); norm=norm8/8;};
            int ely=y+fgh-2-norm; int elx_start=x+off_x+(int)(i*shag);
            for(int dx=x+off_x;dx<elx_start;dx++){if(dx<x+fgw) wrdisplay2(ely,dx,".");};
        };
    }

    int nc=(r1000>=5)?r1000-3:(r1000>=2?r1000:1); int glob=(r1000>=5)?1:0;
    int mode=(r1>4)?(r1-4):r1;

    for(int i=0;i<colp;i++){
        int norm=(int)(((pounds[i]-minp)*(float)(vis-1))/delt);
        int norm8=(int)(((pounds[i]-minp)*(float)(vis-1)*8.0)/delt);
        int suby=norm8%8;
        int target_ely=(r10==6)?(y+fgh-2-(norm8/8)):(y+fgh-2-norm);
        int b_c=i*nc;
        
        int elx_start=x+off_x+(int)(i*shag);
        int elx_end=(r1>=5)?((i==colp-1)?(x+fgw):(x+off_x+(int)((i+1)*shag))):(elx_start+1);
        if(elx_end>x+fgw) elx_end=x+fgw;

        for(int elx=elx_start;elx<elx_end;elx++){
            if(r10==1||r10==2){wrdisplay2(y+fgh-1,elx,"^");}
            if(r10==1||r10==3){if(target_ely-1>=y) wrdisplay2(target_ely-1,elx,"v");}
            if(r10==4 && elx==elx_start){
                if(target_ely-1>=y){
                    char buf[16]; snprintf(buf,sizeof(buf),"%d",pounds[i]);
                    for(int k=0;buf[k];k++){if(elx+k<x+fgw) wrdisplay2(target_ely-1,elx+k,"%c",buf[k]);};
                }
            }
            if(r10==5 && elx==elx_start){
                char buf[16]; snprintf(buf,sizeof(buf),"%d",pounds[i]);
                for(int k=0;buf[k];k++){if(elx+k<x+fgw) wrdisplay2(y+fgh-1,elx+k,"%c",buf[k]);};
            }

            int sy,ey;
            if(mode==3){sy=y+1; ey=target_ely;}else if(mode==4){sy=target_ely; ey=y+fgh-2;}else{sy=target_ely; ey=target_ely;}
            if(sy>ey){int tmp=sy; sy=ey; ey=tmp;}

            for(int cy=sy;cy<=ey;cy++){
                int r,g,b;
                if(nc==1){
                    int ci=(b_c%colc)*3;
                    if(colors[ci]>255){r=(colors[ci]>>16)&255; g=(colors[ci]>>8)&255; b=colors[ci]&255;}
                    else{r=colors[ci]; g=colors[ci+1]; b=colors[ci+2];}
                }else{
                    float t;
                    if(glob){t=(float)(y+fgh-2-cy)/(float)(vis-1>0?vis-1:1);}else{
                        int ht_local=(mode==3)?(target_ely-(y+1)):(y+fgh-2-target_ely); if(ht_local<=0) ht_local=1;
                        if(mode==3){t=(float)(cy-(y+1))/ht_local;}else{t=(float)(y+fgh-2-cy)/ht_local;}
                    }
                    if(t<0) t=0; if(t>1) t=1;
                    float sc=t*(nc-1); int sg=(int)sc; if(sg>=nc-1){sg=nc-2; sc=(float)(nc-1);} float fr=sc-sg;
                    int i1=((b_c+sg)%colc)*3; int i2=((b_c+sg+1)%colc)*3; int r1c,g1c,b1c,r2c,g2c,b2c;
                    if(colors[i1]>255){r1c=(colors[i1]>>16)&255; g1c=(colors[i1]>>8)&255; b1c=colors[i1]&255;}
                    else{r1c=colors[i1]; g1c=colors[i1+1]; b1c=colors[i1+2];}
                    if(colors[i2]>255){r2c=(colors[i2]>>16)&255; g2c=(colors[i2]>>8)&255; b2c=colors[i2]&255;}
                    else{r2c=colors[i2]; g2c=colors[i2+1]; b2c=colors[i2+2];}
                    r=r1c+(r2c-r1c)*fr; g=g1c+(g2c-g1c)*fr; b=b1c+(b2c-b1c)*fr;
                }
                char* sym=".";
                if(mode==1) sym="."; else if(mode==2||mode==3||mode==4) sym="\u2588"; 
                if(r10==6&&cy==target_ely){
                    if(suby==0) sym="\u2581"; else if(suby==1) sym="\u2582";
                    else if(suby==2) sym="\u2583"; else if(suby==3) sym="\u2584";
                    else if(suby==4) sym="\u2585"; else if(suby==5) sym="\u2586";
                    else if(suby==6) sym="\u2587"; else if(suby==7) sym="\u2588";
                }
                wrdisplay2(cy,elx,"\033[38;2;%d;%d;%dm%s\033[0m",r,g,b,sym);
            }
        }
    }
    return 0;
}
