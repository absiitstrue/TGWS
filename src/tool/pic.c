int tgws_put_pic(int y,int x, int fgh, int fgw, const char* fn){
    FILE *file = fopen(fn,"rb");
    if(!file){tgws_print_label(y,x,"not open");return 0;};

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
            wrdisplay2(y+i,x+ii,"\033[38;2;%u;%u;%um\033[48;2;%u;%u;%um\u2580",fr,fg,fb,br,bg,bb);
        };
    };

fclose(file);return 0;}