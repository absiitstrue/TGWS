int tgws_print_label(int y , int x,char *str){
    int wwstr = strlen(str);
    for(int i = 0; i<wwstr;i++){
        wrdisplay2(y,x+i,"\033[38;2;255;255;255m%c",str[i]);
    };
    return 0;}

    int tgws_print_label_c16(int c,int y , int x,char *str){
    int wwstr = strlen(str);
    if(c<8)c+=30;
    else c+=82;
    for(int i = 0; i<wwstr;i++){
        wrdisplay2(y,x+i,"\033[%im%c",c,str[i]);
    };
 
    
    return 0;}


    int tgws_print_label_c256(int c,int y , int x,char *str){
    int wwstr = strlen(str);
    for(int i = 0; i<wwstr;i++){
        wrdisplay2(y,x+i,"\033[38;5;%im%c",c,str[i]);
    };
 
    
    return 0;}
