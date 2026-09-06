int tgws_copy_mm(int y ,int x,int fgh,int fgw,char b[][64]){

    for(int i =0;i<fgh;i++){
        for(int ii =0;ii<fgw;ii++){
            wrdisplay2(y+i,x+ii,b[i*fgw+ii]);
        }
    }

return 0;}
