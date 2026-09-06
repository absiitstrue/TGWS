int tgws_draw_horisontal_gradient(int r1, int g1, int b1,
                             int r2, int g2, int b2,
                             int y, int x,
                             int fgh, int fgw){

    int dr = 0, dg = 0, db = 0;
    float shr = 0, shg = 0, shb = 0;
    if (fgh>h || fgw>w){printf("Error uncorect size.\n      |height| width |\n have | %i   | %i   |\n your | %i  | %i   |\n",h,w,fgh,fgw); return 1;}
    if (fgh ==-1){fgh = h*2-y*2;};// max
    if (fgw ==-1){fgw = w-x;};

    dr = -(r1 - r2);
    dg = -(g1 - g2);
    db = -(b1 - b2);

    shr = (float)dr / fgw;
    shg = (float)dg / fgw;
    shb = (float)db / fgw;

    for (int ch =0; ch< fgh; ch++){int cw = 0;
    for (; cw< fgw; cw++){

        int r= round((float)r1+shr*cw);
        int g= round((float)g1+shg*cw);
        int b= round((float)b1+shb*cw);
        int ox = x + ch;
        int oy = y + cw;
        fastwrdisplay2c(ox,oy,"\u2588",r,g,b    );
};cw = 0;};
    return 0;
}
