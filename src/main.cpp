#include "tgaimage.h"
#include <iostream>

constexpr Tga_Color white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr Tga_Color green   = {  0, 255,   0, 255};
constexpr Tga_Color red     = {  0,   0, 255, 255};
constexpr Tga_Color blue    = {255, 128,  64, 255};
constexpr Tga_Color yellow  = {  0, 200, 255, 255};

void line_draw(Tga_Image& image, int x1, int y1, int x2, int y2, const Tga_Color& color)
{
    const bool change_coordinate = abs(y2 - y1) > abs(x2 - x1);
    if (change_coordinate)
    {
        x1 = x1 ^ y1;
        y1 = y1 ^ x1;
        x1 = y1 ^ x1;

        x2 = x2 ^ y2;
        y2 = y2 ^ x2;
        x2 = x2 ^ y2;
    }
    if (x1 > x2)
    {
        x1 = x1 ^ x2;
        x2 = x2 ^ x1;
        x1 = x2 ^ x1;

        y1 = y1 ^ y2;
        y2 = y2 ^ y1;
        y1 = y2 ^ y1;
    }
    const double err = 1.0 * (y1 - y2) / (x1 - x2);
    // std::cout << y1 << y2 << x1 << x2 << std::endl;
    // std::cout << "err = " << err << std::endl;
    double err_cont = 0;
    for (int x = x1; x <= x2; x++)
    {
        // std::cout << err_cont << std::endl;
        if (err_cont < -0.5 || err_cont > 0.5)
        {
            // std::cout << err_cont << std::endl;
            y1 += err > 0.0 ? 1 : -1;
            err_cont -= 1;
        }
        change_coordinate ? image.set_color(y1, x, color) : image.set_color(x, y1, color);
        err_cont += err;
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    Tga_Image framebuffer(width, height, Tga_Image::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    framebuffer.set_color(ax, ay, white);
    framebuffer.set_color(bx, by, white);
    framebuffer.set_color(cx, cy, white);
    std::cout << "point 1: x = " << ax << " y = " << ay << std::endl;
    line_draw(framebuffer, ax, ay, bx, by, green);
    std::cout << "point 2: x = " << bx << " y = " << by << std::endl;
    line_draw(framebuffer, bx, by, cx, cy, yellow);
    std::cout << "point 3: x = " << cx << " y = " << cy << std::endl;
    line_draw(framebuffer, cx, cy, ax, ay, blue);

    framebuffer.write_tga_image_file("framebuffer.tga");
    return 0;
}