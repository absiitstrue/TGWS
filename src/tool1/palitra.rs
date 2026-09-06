static int opr=0, opg=0, opb=0, plh=0, pls=0, plv=0,rret=0;
static int colorsel[3]={-1};
void hsv_to_rgb(){
    
    float r, g, b, s = (float)pls / 100.0f, v = (float)plv / 100.0f;
    if(s <= 0.0f){ r = g = b = v; }else{
        float h = (float)plh;
        if(h >= 360.0f) h = 0.0f;
        h /= 60.0f;
        int i = (int)h;
        float f = h - i, p = v * (1.0f - s), q = v * (1.0f - s * f), t = v * (1.0f - s * (1.0f - f));
        switch(i){
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default: r = v; g = p; b = q; break;
        }
    }
    opr = (int)(r * 255.0f); opg = (int)(g * 255.0f); opb = (int)(b * 255.0f);
}

void rgb_to_hsv(){
    float fr = (float)opr / 255.0f, fg = (float)opg / 255.0f, fb = (float)opb / 255.0f;
    float max = fr > fg ? (fr > fb ? fr : fb) : (fg > fb ? fg : fb);
    float min = fr < fg ? (fr < fb ? fr : fb) : (fg < fb ? fg : fb);
    float delta = max - min;
    plv = (int)(max * 100.0f);
    if(max <= 0.0f){ pls = 0; plh = 0; }else{
        pls = (int)((delta / max) * 100.0f);
        if(delta <= 0.0f){ plh = 0; }else{
            if(max == fr) plh = (int)(60.0f * ((fg - fb) / delta + (fg < fb ? 6.0f : 0.0f)));
            else if(max == fg) plh = (int)(60.0f * ((fb - fr) / delta + 2.0f));
            else plh = (int)(60.0f * ((fr - fg) / delta + 4.0f));
        }
    }
}

int* tgws_call_palitra(int y, int x){
    fcl_palitra =1;
    rret=1;
    for(int m =0;m<=36;m++){for(int mm=0;mm<=63;mm++){fastwrdisplay2w(y+m,x+mm," ");};};
    int rgbcr= tgws_draw_polzun_vertical(255, 0, 0, 255, y + 2, x + 1,32, 1, 0, opr);
    int rgbcg= tgws_draw_polzun_vertical(0, 255, 0, 255, y + 2, x + 3,32, 1, 1, opg);
    int rgbcb= tgws_draw_polzun_vertical(0, 0, 255, 255, y + 2, x + 5,32, 1, 2, opb);
    int hsvh = tgws_draw_polzun_vertical(255, 80, 0, 360, y + 2, x + 8,32, 1, 3, plh);
    int hsvs = tgws_draw_polzun_vertical(255, 255, 255, 100, y + 2, x + 10,32, 1, 4, pls);
    int hsvv = tgws_draw_polzun_vertical(150, 150, 150, 100, y + 2, x + 12,32, 1, 5, plv);
    
    tgws_print_label_fc(y+34,x+49,"[OK]",255,255,255,0,0,0);
    tgws_print_label_fc(y+34,x+55,"[CLOSE]",255,255,255,0,0,0);
    nnn=tgws_set_button_classic(y+34,x+49,1,4); if(nnn==2){fcl_palitra=0;nnn=0;rret=0;};
    nnn=tgws_set_button_classic(y+34,x+55,1,7); if(nnn==2){fcl_palitra=0;nnn=0;rret=1;};
    tgws_print_label_fc(y + 1, x + 1, "R G B |H S V",255,255,255,0,0,0);
    tgws_draw_grandient_3color(opr, opg, opb, 255, 255, 255, 0, 0, 0, y + 1, x + 17, 45, 45);
    tgws_draw_border(1, 255, 255, 255, y, x + 15,35, 47);
    tgws_draw_border(1, 255, 255, 255, y + 23, x + 15, 12, 47);
    tgws_draw_vertical_line(1, 255, 255, 255, y + 1, x + 7,34, 1);
    tgws_draw_border(1, 255, 255, 255, y, x,35, 62);

    char dt[20]={0};
    sprintf(dt,"%-3iR",opr);tgws_print_label_fc(y+24,x+16,dt,255,255,255,0,0,0);
    sprintf(dt,"%-3iG",opg);tgws_print_label_fc(y+25,x+16,dt,255,255,255,0,0,0);
    sprintf(dt,"%-3iB",opb);tgws_print_label_fc(y+26,x+16,dt,255,255,255,0,0,0);
    sprintf(dt,"%-3iH",plh);tgws_print_label_fc(y+27,x+16,dt,255,255,255,0,0,0);
    sprintf(dt,"%-3iS",pls);tgws_print_label_fc(y+28,x+16,dt,255,255,255,0,0,0);
    sprintf(dt,"%-3iV",plv);tgws_print_label_fc(y+29,x+16,dt,255,255,255,0,0,0);


    fastwrdisplay2w(y,x+7,"\u252C");
    fastwrdisplay2w(y+35,x+7,"\u2534");
    fastwrdisplay2w(y,x+15,"\u252C");
    
    fastwrdisplay2w(y+23,x+62,"\u2524");
    fastwrdisplay2w(y+23,x+15,"\u251C");

    fastwrdisplay2w(y+35,x+15,"\u2534");
    
    fastwrdisplay2w(y+23,x+22,"\u252C");
    fastwrdisplay2w(y+35,x+22,"\u2534");

    tgws_draw_square(opr,opg,opb,y+30,x+16,4,5);
    tgws_draw_vertical_line(1, 255, 255, 255, y + 24, x + 22, 11, 1);
    
    int f_rgb = 0, f_hsv = 0;
    if(rgbcr != opr){ opr = rgbcr; f_rgb = 1; }
    if(rgbcg != opg){ opg = rgbcg; f_rgb = 1; }
    if(rgbcb != opb){ opb = rgbcb; f_rgb = 1; }
    
    if(hsvh != plh){ plh = hsvh; f_hsv = 1; }
    if(hsvs != pls){ pls = hsvs; f_hsv = 1; }
    if(hsvv != plv){ plv = hsvv; f_hsv = 1; }

    if(f_rgb){ rgb_to_hsv(); }
    else if(f_hsv){ hsv_to_rgb(); }

    colorsel[0] = opr;
    colorsel[1] = opg;
    colorsel[2] = opb;
    if(rret==1){colorsel[0]=-1;};
    return colorsel;
}
