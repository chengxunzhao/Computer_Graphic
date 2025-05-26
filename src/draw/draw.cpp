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

void triangle_draw(Tga_Image &image,const int_vec2 a,const int_vec2 b,const int_vec2 c,const Tga_Color &color,const DRAW_MODE mode)
{
    line_draw(image, a.x, a.y, b.x, b.y, color);
    line_draw(image, b.x, b.y, c.x, c.y, color);
    line_draw(image, c.x, c.y, a.x, a.y, color);
    if (DRAW_MODE::Line == mode)
    {
        return;
    }
    if (DRAW_MODE::Filler == mode)
    {
        LOGW("[ {} {} ]", a.x, a.y);
        LOGW("[ {} {} ]", b.x, b.y);
        LOGW("[ {} {} ]", c.x, c.y);

        int_vec2 vect_a_b_ = b - a;
        int_vec2 vect_b_c_ = c - b;
        int_vec2 vect_c_a_ = a - c;
        LOGI("ab -> [{} {}]", vect_a_b_.x, vect_a_b_.y);
        LOGI("bc -> [{} {}]", vect_b_c_.x, vect_b_c_.y);
        LOGI("ca -> [{} {}]", vect_c_a_.x, vect_c_a_.y);

        int begin_x = std::min(std::min(a.x, b.x), c.x);
        int end_x   = std::max(std::max(a.x, b.x), c.x);
        int begin_y = std::min(std::min(a.y, b.y), c.y);
        int end_y   = std::max(std::max(a.y, b.y), c.y);

        for (int x = begin_x; x <= end_x; x++)
        {
            for (int y = begin_y; y <= end_y; y++)
            {
                int_vec2 point(x, y);
                // LOGI("point x {} y {}", point.x, point.y);
                int_vec2 vect_a_p = point - a;
                int_vec2 vect_b_p = point - b;
                int_vec2 vect_c_p = point - c;
                // LOGI("ap -> [{} {}]", vect_a_p.x, vect_a_p.y);
                // LOGI("bp -> [{} {}]", vect_b_p.x, vect_b_p.y);
                // LOGI("cp -> [{} {}]", vect_c_p.x, vect_c_p.y);
                int p1 = vect_a_b_ ^ vect_a_p;
                int p2 = vect_b_c_ ^ vect_b_p;
                int p3 = vect_c_a_ ^ vect_c_p;
                LOGI("[ap * ab {}] | [bp *bc {}] | [cp * ca {}]", p1, p2, p3);
                if ((p1 > 0 && p2 > 0 && p3 > 0) || (p1 < 0 && p2 < 0 && p3 < 0))
                {
                    image.set_color(point.x, point.y, color);
                    LOGI("{} {}", point.x, point.y);
                }
            }
        }

    }
}