#include "tgaimage.h"

constexpr Tga_Color white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr Tga_Color green   = {  0, 255,   0, 255};
constexpr Tga_Color red     = {  0,   0, 255, 255};
constexpr Tga_Color blue    = {255, 128,  64, 255};
constexpr Tga_Color yellow  = {  0, 200, 255, 255};

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

    framebuffer.write_tga_image_file("framebuffer.tga");
    return 0;
}