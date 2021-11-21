#include <stdio.h>

#ifndef   TERMINAL_COLORS_H
#define   TERMINAL_COLORS_H

#define   TERMINAL_RESET  0

enum terminal_colors
{
    TM_COLOR_BLACK = 30,
    TM_COLOR_RED,
    TM_COLOR_GREEN,
    TM_COLOR_YELLOW,
    TM_COLOR_BLUE,
    TM_COLOR_MAGENTA,
    TM_COLOR_CYAN,
    TM_COLOR_WHITE
};

void set_tmcolor (int color)
{
    printf ("\033[%dm", color);
}

void set_back_tmcolor (int color)
{
    set_tmcolor (color + 10);
}

void reset_tmcolor ()
{
    set_tmcolor (0);
}

#define $fbc  set_tmcolor (TM_COLOR_BLACK);
#define $frd  set_tmcolor (TM_COLOR_RED);
#define $fgr  set_tmcolor (TM_COLOR_GREEN);
#define $fyl  set_tmcolor (TM_COLOR_YELLOW);
#define $fbl  set_tmcolor (TM_COLOR_BLUE);
#define $fmg  set_tmcolor (TM_COLOR_MAGENTA);
#define $fcn  set_tmcolor (TM_COLOR_CYAN);
#define $fwt  set_tmcolor (TM_COLOR_WHITE);

#define $bbc  set_back_tmcolor (TM_COLOR_BLACK);
#define $brd  set_back_tmcolor (TM_COLOR_RED);
#define $bgr  set_back_tmcolor (TM_COLOR_GREEN);
#define $byl  set_back_tmcolor (TM_COLOR_YELLOW);
#define $bbl  set_back_tmcolor (TM_COLOR_BLUE);
#define $bmg  set_back_tmcolor (TM_COLOR_MAGENTA);
#define $bcn  set_back_tmcolor (TM_COLOR_CYAN);
#define $bwt  set_back_tmcolor (TM_COLOR_WHITE);

#define $def  reset_tmcolor ();

#endif // TERMINAL_COLORS_H
