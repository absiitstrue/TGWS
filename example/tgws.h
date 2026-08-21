#ifndef TGWS_H
#define TGWS_H

#include <stdint.h>
#include <stdlib.h>
//tgws.c
int tgws_close_window(void);
uint64_t get_time_us(void);
int tgws_init_window(int argc, char *argv[]);
int tgws_draw_element(void);
extern int fcl_palitra;
void fwrdisplay2(int row, int col, const char *format, ...);

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

//grandient_cor_diagonal.rs
int tgws_draw_corect_diagonal_grandient(int r1, int g1, int b1,int r2, int g2, int b2,int y, int x, int fgh, int fgw);

//grandient_horisontal.rs
int tgws_draw_horisontal_gradient(int r1, int g1, int b1,int r2, int g2, int b2,int y, int x,int fgh, int fgw);

//grandient_unc_diagonal.rs
int tgws_draw_ucorect_diagonal_grandient(int r1, int g1, int b1,int r2, int g2, int b2,int y, int x,int fgh, int fgw);

//grandient_vertical.rs
int tgws_draw_vertical_gradient(int r1, int g1, int b1,int r2, int g2, int b2,int y, int x,int fgh, int fgw);

//grandientx3.rs
int tgws_draw_grandient_3color(int r1,int g1,int b1,int rc1,int gc1,int bc1,int rc2,int gc2,int bc2,int x,int y,int fgh,int fgw);
int tgws_draw_grandient_line_to_color(int r,int g,int b, int colors[],int x, int y, int fgh,int fgw);

//label.rs
int tgws_print_label(int y, int x, char *str);
int tgws_print_label_c16(int c,int y,int x, char *str);
int tgws_print_label_c256(int c,int y,int x,char *str);

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
