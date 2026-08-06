#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

int tgws_draw_ucorect_diagonal_grandient(int r1, int g1, int b1,
                             int r2, int g2, int b2,
                            int y, int x,
                             int fgh, int fgw){

    int dr = 0, dg = 0, db = 0;
    float shr = 0, shg = 0, shb = 0;
    if (fgh/2>h || fgw>w){printf("Error uncorect size.\n      |height| width |\n have | %i   | %i   |\n your | %i  | %i   |\n",h,w,fgh,fgw); return 1;}
    if (fgh ==-1){fgh = h*2;};// max
    if (fgw ==-1){fgw = w;};

    dr = -(r1 - r2);
    dg = -(g1 - g2);
    db = -(b1 - b2);

    shr = (float)dr / fgw;
    shg = (float)dg / fgw;
    shb = (float)db / fgw;

    for (int ch =0; ch< fgh; ch+=2){int cw = 0;
    for (; cw< fgw; cw++){

        int r= round((float)r1+shr*(0.5*ch+cw));
        int g= round((float)g1+shg*(0.5*ch+cw));
        int b= round((float)b1+shb*(0.5*ch+cw));

        int rt= round((float)r1+shr*(0.5*ch+cw+1));
        int gt= round((float)g1+shg*(0.5*ch+cw+1));
        int bt= round((float)b1+shb*(0.5*ch+cw+1));

int ox = x + ch;
        int oy = y + cw;

        wrdisplay2(ox,oy,"\033[38;2;%i;%i;%im\033[48;2;%i;%i;%im\u2580"
        ,r
        ,g
        ,b
        ,rt
        ,gt
        ,bt    );
};cw = 0;};
    return 0;
}
    