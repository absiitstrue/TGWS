int tgws_put_pic(int y,int x, int fgh, int fgw, const char* fn){
    FILE *file = fopen(fn,"rb");
    if(!file){tgws_print_label_fc(y,x,"not open",255,255,255,0,0,0);return 0;};

    rewind(file);
    uint8_t ph = fgetc(file);
    uint8_t pw = fgetc(file);

    for(int i =0;i<ph/2;i++){
        for(int ii=0;ii<pw;ii++){

            uint8_t fr= (uint8_t)fgetc(file); 
            uint8_t fg= (uint8_t)fgetc(file); 
            uint8_t fb= (uint8_t)fgetc(file); 
            uint8_t br= (uint8_t)fgetc(file); 
            uint8_t bg= (uint8_t)fgetc(file); 
            uint8_t bb= (uint8_t)fgetc(file); 
            fastwrdisplay2(y+i,x+ii,"\u2580",fr,fg,fb,br,bg,bb);
        };
    };

fclose(file);return 0;}
