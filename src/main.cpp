#include "draw.h"
#include "model.h"
#include "log.h"
#include <iostream>

constexpr int width  = 128;
constexpr int height = 128;

constexpr Tga_Color white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr Tga_Color green   = {  0, 255,   0, 255};
constexpr Tga_Color red     = {  0,   0, 255, 255};
constexpr Tga_Color blue    = {255, 128,  64, 255};
constexpr Tga_Color yellow  = {  0, 200, 255, 255};

std::tuple<int,int> project(vertex v) 
{ 
    return { (v.x + 1.) *  width/2,  
             (v.y + 1.) * height/2 }; 
}
int main(int argc, char** argv) {
    // if (argc != 2)
    // {
    //     LOGE("Usage: {} obj/model.obj", argv[0]);
    //     return 1;
    // }
    // Model model(argv[1]);
    Tga_Image framebuffer(width, height, Tga_Image::RGB);
    
    int_vec2 a1( 7,  45);
    int_vec2 b1(35, 100);
    int_vec2 c1(45,  60);

    int_vec2 a2(120,  35);
    int_vec2 b2( 90,   5);
    int_vec2 c2( 45, 110);

    int_vec2 a3(115,  83);
    int_vec2 b3( 80,  90);
    int_vec2 c3( 85, 120);

    triangle_draw(framebuffer, a1, b1, c1,   red, DRAW_MODE::Filler);
    triangle_draw(framebuffer, a2, b2, c2, white, DRAW_MODE::Filler);
    triangle_draw(framebuffer, a3, b3, c3, green, DRAW_MODE::Filler);

    // for (int i = 0; i < model.fragment_size(); i++)
    // {
    //     auto [ax, ay] = project(model.fragment_vert(i, 0));
    //     auto [bx, by] = project(model.fragment_vert(i, 1));
    //     auto [cx, cy] = project(model.fragment_vert(i, 2));
    //     line_draw(framebuffer, ax, ay, bx, by, red);
    //     line_draw(framebuffer, bx, by, cx, cy, red);
    //     line_draw(framebuffer, cx, cy, ax, ay, red);
    // }

    // for (int i = 1; i < model.vertex_size(); i++)
    // {
    //     auto [x, y] = project(model.vetx(i));
    //     framebuffer.set_color(x, y, green);
    // }

    framebuffer.write_tga_image_file("framebuffer.tga");
    return 0;
}