#include "draw.h"

void line_draw(Tga_Image& image, int x1, int y1, int x2, int y2, const Tga_Color& color)
{
    const bool change_coordinate = abs(y2 - y1) > abs(x2 - x1);
    if (change_coordinate)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int y = y1;
    int err_cont = 0;
    for (int x = x1; x <= x2; x++)
    {
        if (std::abs(err_cont) > std::abs(x2 - x1))
        {
            y += y2 > y1 ? 1 : -1;
            err_cont -= 2 * (x2 - x1);
        }
        change_coordinate ? image.set_color(y, x, color) : image.set_color(x, y, color);
        err_cont += 2 * std::abs(y2 - y1);
    }
}