#ifndef TGWS_H
#define TGWS_H

#include <stdint.h>
#include <stdarg.h>

// Основные функции окна и рендеринга
int tgws_close_window(void);
uint64_t get_time_us(void);
int tgws_init_window(int argc, char *argv[]);
int tgws_draw_element(void);
extern int fcl_palitra;
// Функция форматированного вывода на дисплей
void fwrdisplay2(int row, int col, const char *format, ...);

// Палитра
int* tgws_call_palitra(int y, int x);

// Кнопки
int tgws_set_button_classic(int y, int x, int fgh, int fgw);
int tgws_set_button_radio(int *b, int numb);
int tgws_set_button_fix(int y, int x, int fgh, int fgw, int numb);

// Элементы рисования
int tgws_draw_border(int br, int rr, int rg, int rb, int y, int x, int fgw, int fgh);
int tgws_draw_square(int r1, int g1, int b1, int y, int x, int fgh, int fgw);
int tgws_draw_tablr(int r, int g, int b, int y, int x, int fgh, int fgw, int *xy);
int tgws_print_label(int y, int x, char *str);
int tgws_draw_line_m_char(int r,int g,int b, int y0,int x0, int y1, int x1, char *ss);

#endif // TGWS_H
