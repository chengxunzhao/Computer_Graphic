#include "draw.h"
#include "log.h"

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

void triangle_draw(Tga_Image &image,Tga_Image &z_buffer, const int_vec3 a,const int_vec3 b,const int_vec3 c,const Tga_Color &color,const DRAW_MODE mode)
{
    if (DRAW_MODE::Line == mode)
    {
        line_draw(image, a.x, a.y, b.x, b.y, color);
        line_draw(image, b.x, b.y, c.x, c.y, color);
        line_draw(image, c.x, c.y, a.x, a.y, color);
        return;
    }
    if (DRAW_MODE::Filler == mode)
    {
        int_vec2 vect_a_b_(b.x - a.x, b.y - a.y);
        int_vec2 vect_b_c_(c.x - b.x, c.y - b.y);
        int_vec2 vect_c_a_(a.x - c.x, a.y - c.y);
        int_vec2 vect_c_b_(c.x - b.x, c.y - b.y);
        int      area_s    = std::abs(vect_c_a_ ^ vect_c_b_); 
        if (area_s < 1)
        {
            return;
        }
        int begin_x = std::min(std::min(a.x, b.x), c.x);
        int end_x   = std::max(std::max(a.x, b.x), c.x);
        int begin_y = std::min(std::min(a.y, b.y), c.y);
        int end_y   = std::max(std::max(a.y, b.y), c.y);

        for (int x = begin_x; x <= end_x; x++)
        {
            for (int y = begin_y; y <= end_y; y++)
            {
                int_vec2 vect_a_p(x - a.x, y - a.y);
                int_vec2 vect_b_p(x - b.x, y - b.y);
                int_vec2 vect_c_p(x - c.x, y - c.y);
                int p1 = vect_a_b_ ^ vect_a_p;
                int p2 = vect_b_c_ ^ vect_b_p;
                int p3 = vect_c_a_ ^ vect_c_p;
                if ((p1 >= 0 && p2 >= 0 && p3 >= 0))
                {
                    double alpha = static_cast<double>(p2) / area_s;
                    double beta = static_cast<double>(p3) / area_s;
                    double gamma = static_cast<double>(p1) / area_s;
                    unsigned char z = static_cast<unsigned char>(alpha * a.z + beta * b.z + gamma * c.z);
                    if (z < z_buffer.get_color(x, y)[0])
                    {
                        continue;
                    }
                    image.set_color(x, y, color);
                    z_buffer.set_color(x, y, {z});
                }
            }
        }

    }
}