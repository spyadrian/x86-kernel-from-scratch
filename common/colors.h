#ifndef _COLORS_H_
#define _COLORS_H_

// Convert a 24-bit color to a 16-bit one (RGB565)
#define RGB(r,g,b) (((r & 0x1f) << 11) | ((g & 0x3f) << 5) | ((b & 0x1f) << 0))

#define BLACK        RGB(0,0,0)
#define WHITE        RGB(255,255,255)
#define RED          RGB(255,0,0)
#define BLUE         RGB(0,0,255)
#define GREEN        RGB(0,255,0)
#define YELLOW       RGB(255,255,0)
#define CYAN         RGB(0,255,255)
#define GREY         RGB(150,150,150)
#define LIGHT_GREY   RGB(220,220,220)
#define LIGHT_BLUE   RGB(150,150,220)
#define LIGHT_GREEN  RGB(150,220,150)
#define LIGHT_RED    RGB(255,200,200)
#define LIGHT_YELLOW RGB(220,220,150)
#define DARK_GREY    RGB(80,80,80)
#define DARK_BLUE    RGB(0,0,100)
#define DARK_GREEN   RGB(0,100,0)
#define DARK_RED     RGB(100,0,0)
#define DARK_YELLOW  RGB(100,100,0)

#endif
