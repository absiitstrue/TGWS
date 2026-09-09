#ifndef TGWS_H
#define TGWS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
//tgws.c
int tgws_close_window(void);
uint64_t get_time_us(void);
int tgws_init_window(int argc, char *argv[]);
int tgws_draw_element(void);
extern int fcl_palitra;
void fwrdisplay2(int y,int x, char *world,int r,int g,int b,int fr, int fg,int fb);
extern atomic_uint kbb;extern atomic_uint kbp;
extern atomic_uint mx;
extern atomic_uint my;
extern atomic_uint mbtn;
extern int refp;
//palitra.rs 
int* tgws_call_palitra(int y, int x);

// border.rs
int tgws_draw_border(int br,int rr, int rg, int rb,int y,int x,int fgw, int fgh);

// button.rs
int tgws_set_button_classic(int y, int x, int fgh, int fgw);
int tgws_set_button_radio(int *b, int numb);
int tgws_set_button_fix(int y, int x, int fgh, int fgw, int numb);

// copy_mm.rs
int tgws_copy_mm(int y ,int x,int fgh,int fgw,char b[][64]);

//grandient_multi.c
int tgws_draw_gradient_super(int mode, int r1, int g1, int b1, int r2, int g2, int b2, int r3, int g3, int b3, int x, int y, int fgh, int fgw, int *colors);

#define tgws_draw_vertical_gradient(r1,g1,b1,r2,g2,b2,y,x,fgh,fgw) \
    tgws_draw_gradient_super(-1,r1,g1,b1,r1,g1,b1,r2,g2,b2,y,x,fgh,fgw,NULL)

#define tgws_draw_horisontal_gradient(r1,g1,b1,r2,g2,b2,y,x,fgh,fgw) \
    tgws_draw_gradient_super(-1,r1,g1,b1,r2,g2,b2,r1,g1,b1,y,x,fgh,fgw,NULL)

#define tgws_draw_gradient_with_angle(r1,g1,b1,r2,g2,b2,angle,y,x,fgh,fgw) \
    tgws_draw_gradient_super(angle,r1,g1,b1,r2,g2,b2,r2,g2,b2,y,x,fgh,fgw,NULL)

#define tgws_draw_grandient_3color(r1,g1,b1,rc1,gc1,bc1,rc2,gc2,bc2,x,y,fgh,fgw) \
    tgws_draw_gradient_super(-2,r1,g1,b1,rc1,gc1,bc1,rc2,gc2,bc2,x,y,fgh,fgw,NULL)

#define tgws_draw_grandient_3c_auto(r1,g1,b1,r2,g2,b2,r3,g3,b3,x,y,fgh,fgw) \
    tgws_draw_gradient_super(-1,r1,g1,b1,r2,g2,b2,r3,g3,b3,x,y,fgh,fgw,NULL)

#define tgws_draw_grandient_line_to_color(r1,g1,b1,colors,x,y,fgh,fgw) \
    tgws_draw_gradient_super(-5,r1,g1,b1,0,0,0,0,0,0,x,y,fgh,fgw,colors)

//label.c
int tgws_print_label_fc(int y, int x, char *str,int r,int g, int b,int fr, int fg,int fb);
#define tgws_print_label(y,x,str)\
        tgws_print_label_fc(y,x,str,255,255,255,0,0,0)
#define tgws_print_label_c(y,x,str,r,g,b)\
        tgws_print_label_fc(y,x,str,r,g,b,0,0,0)
#define tgws_print_label_f(y,x,str,fr,fg,fb)\
        tgws_print_label_fc(y,x,str,255,255,255,fr,fg,fb)
int tgws_print_txt_fc(int y, int x, char *str,int r,int g, int b,int fr, int fg,int fb);
#define tgws_print_txt(y,x,str)\
        tgws_print_txt_fc(y,x,str,255,255,255,0,0,0)
#define tgws_print_txt_c(y,x,str,r,g,b)\
        tgws_print_txt_fc(y,x,str,r,g,b,0,0,0)
#define tgws_print_txt_f(y,x,str,fr,fg,fb)\
        tgws_print_txt_fc(y,x,str,255,255,255,fr,fg,fb)

// line.rs
int tgws_draw_line_p(int r, int g, int b, int y0, int x0, int y1, int x1);
int tgws_draw_line_m(int r, int g, int b, int y0, int x0, int y1, int x1);
int tgws_draw_line_p_char(int r, int g, int b, int y0, int x0, int y1, int x1, char *mch);
int tgws_draw_line_m_char(int r, int g, int b, int y0, int x0, int y1, int x1, char *mch);

// pic.rs
int tgws_put_pic(int y, int x, int fgh, int fgw, const char* fn);

// polzun.rs
int tgws_draw_polzun_vertical(int r, int g, int b, int max, int y, int x, int fgh, int fgw, int numb, int inint);
int tgws_draw_polzun_horisontal(int r, int g, int b, int max, int y, int x, int fgh, int fgw, int numb, int inint);

// square.rs
int tgws_draw_square(int r1, int g1, int b1, int y, int x, int fgh, int fgw);

// tablr.rs
int tgws_draw_tablr_place(int r, int g, int b, int y, int x, int fgh, int fgw, int *xy);
int tgws_draw_tablr_ofs(int rr,int r, int g, int b, int y, int x, int fgh, int fgw, int *cy, int *cx);

// vertical_line.rs
int tgws_draw_vertical_line(int rr, int r, int g, int b, int y, int x, int fgh, int fgw);

//graf.rs
int tgws_draw_grafic(int rg,int colors[],int pound[],int y,int x, int fgh, int fgw);

#endif // TGWS_H
