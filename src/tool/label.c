int tgws_print_label(int y , int x,char *str){
    int wwstr = strlen(str);
    for(int i = 0; i<wwstr;i++){
        wrdisplay2(y,x+i,"\033[38;2;255;255;255m%c",str[i]);
    };
    
    return 0;};