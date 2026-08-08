int tgws_draw_line_p(int r,int g,int b,int y0,int x0, int y1, int x1){
  int dx = abs(x0-x1);
  int dy = abs(y0-y1);
    
  int sx = (x0<x1)?1:-1;
  int sy = (y0<y1)?1:-1;

  int err = dx-dy,e2;
  while (1) {
    wrdisplay2(y0,x0,"\033[38;2;%i;%i;%im\u2588",r,g,b);
    if(x0==x1&&y0==y1)break;
      e2 =2*err;
      if(e2>=-dy){err -=dy;x0 +=sx;}
      if(e2<=dx){err +=dx;y0 +=sy;}
  }


   
return 0;}

int tgws_draw_line_m(int r,int g, int b,int y0,int x0,int y1, int x1){

  y0*=2;y1*=2;

  int dx = abs(x0-x1);
  int dy = abs(y0-y1);
  int pz=-10,pzx=-10;
  int sx = (x0<x1)?1:-1;
  int sy = (y0<y1)?1:-1;

  int err = dx-dy,e2;
  while (1) {

    if((pz+1==y0||pz-1==y0)&&pzx == x0){
     wrdisplay2(y0/2,x0,"\033[38;2;%i;%i;%im\u2588",r,g,b);

    }
    else if(!(y0%2)){
      wrdisplay2(y0/2,x0,"\033[38;2;%i;%i;%im\u2580",r,g,b);
    }
    else {wrdisplay2(y0/2,x0,"\033[38;2;%i;%i;%im\u2584",r,g,b);}
  pz = y0;pzx = x0;
    if(x0==x1&&y0==y1)break;
      e2 =2*err;
      if(e2>=-dy){err -=dy;x0 +=sx;}
      if(e2<=dx){err +=dx;y0 +=sy;}}


   
return 0;}

int tgws_draw_line_p_char(int r,int g,int b,int y0,int x0, int y1, int x1, char *mch){
  int dx = abs(x0-x1);
  int dy = abs(y0-y1);
    
  int sx = (x0<x1)?1:-1;
  int sy = (y0<y1)?1:-1;

  int err = dx-dy,e2;
  while (1) {
    wrdisplay2(y0,x0,"\033[38;2;%i;%i;%im%s",r,g,b,mch);
    if(x0==x1&&y0==y1)break;
      e2 =2*err;
      if(e2>=-dy){err -=dy;x0 +=sx;}
      if(e2<=dx){err +=dx;y0 +=sy;}
  }


   
return 0;}

int tgws_draw_line_m_char(int r,int g, int b,int y0,int x0,int y1, int x1,char *mch){

  y0*=2;y1*=2;

  int dx = abs(x0-x1);
  int dy = abs(y0-y1);
  int pz=-10,pzx=-10;
  int sx = (x0<x1)?1:-1;
  int sy = (y0<y1)?1:-1;

  int err = dx-dy,e2;
  while (1) {

    if((pz+1==y0||pz-1==y0)&&pzx == x0){
     wrdisplay2(y0/2,x0,"\033[38;2;%i;%i;%im%s",r,g,b,mch);

    }
    else if(!(y0%2)){
      wrdisplay2(y0/2,x0,"\033[38;2;%i;%i;%im%s",r,g,b,mch);
    }
    else {wrdisplay2(y0/2,x0,"\033[38;2;%i;%i;%im%s",r,g,b,mch);}
  pz = y0;pzx = x0;
    if(x0==x1&&y0==y1)break;
      e2 =2*err;
      if(e2>=-dy){err -=dy;x0 +=sx;}
      if(e2<=dx){err +=dx;y0 +=sy;}}


   
return 0;}

