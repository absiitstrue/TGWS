    int tgws_print_label_c16(int c,int y , int x,char *str){
    int wwstr = strlen(str);
    if(c<8)c+=30;
    else c+=82;
    for(int i = 0; i<wwstr;i++){
        char ms[2]={c,'\0'};
        //wrdisplay2(y,x+i,"\033[%im%c",c,str[i]);
    };
 
    
    return 0;}


    int tgws_print_label_c256(int c,int y , int x,char *str){
    int wwstr = strlen(str);
    for(int i = 0; i<wwstr;i++){
        //wrdisplay2(y,x+i,"\033[38;5;%im%c",c,str[i]);
    };
 
    
    return 0;}

int tgws_print_label_fc(int y , int x,char *str,int r,int g,int b,int fr,int fg,int fb){
    for(int i = 0;str[i]!='\0';i++){
        char ms[2]={str[i],'\0'};
        fastwrdisplay2(y,x+i,ms,r,g,b,fr,fg,fb);
    };
    return 0;}

int tgws_print_txt_fc(int y , int x,char *str,int r,int g,int b,int fr,int fg,int fb){
    int ii =0;
    for(int i = 0;str[i]!='\0';i++){
        ii++;
        if(str[i]=='\n'){
            y++;
            ii=0;
            continue;

        }
        char ms[2]={str[i],'\0'};
        fastwrdisplay2(y,x+ii,ms,r,g,b,fr,fg,fb);
    };
    return 0;}
