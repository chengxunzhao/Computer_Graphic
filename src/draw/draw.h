#ifndef __DRAW__
#define __DRAW__
#include "tgaimage.h"
#include "geometry.h"

enum class DRAW_MODE {Line, Filler};

void line_draw(Tga_Image& image, int x1, int y1, int x2, int y2, const Tga_Color& color);

void triangle_draw(Tga_Image &image, Tga_Image &z_buffer, const int_vec3 a,const int_vec3 b,const int_vec3 c,const Tga_Color &color,const DRAW_MODE mode = DRAW_MODE::Line);

#endif // __DRAW__