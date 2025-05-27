#include "draw.h"
#include "model.h"
#include "log.h"
#include <iostream>

constexpr int width  = 800;
constexpr int height = 800;
    

constexpr Tga_Color white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr Tga_Color green   = {  0, 255,   0, 255};
constexpr Tga_Color red     = {  0,   0, 255, 255};
constexpr Tga_Color blue    = {255, 128,  64, 255};
constexpr Tga_Color yellow  = {  0, 200, 255, 255};
constexpr Tga_Color pink    = {255, 102, 255, 255};

int_vec3 project(double_vec3 v) 
{ 
    return { static_cast<int>((v.x + 1.) *  width / 2),  
             static_cast<int>((v.y + 1.) * height / 2),
             static_cast<int>((v.z + 1.) *   255. / 2) }; 
}
int main(int argc, char** argv) {
    if (argc != 2)
    {
        LOGE("Usage: {} obj/model.obj", argv[0]);
        return 1;
    }
    Model model(argv[1]);
    Tga_Image framebuffer(width, height, Tga_Image::RGB);
    Tga_Image z_buffer(width, height, Tga_Image::GRAY);

    double alpha = M_PI/6;
    double_mat3 Ry = {{{std::cos(alpha), 0, std::sin(alpha)}, {0,1,0}, {-std::sin(alpha), 0, std::cos(alpha)}}};

    for (int i = 0; i < model.fragment_size(); i++)
    {
        int_vec3 a = project(Ry * model.fragment_vert(i, 0));
        int_vec3 b = project(Ry * model.fragment_vert(i, 1));
        int_vec3 c = project(Ry * model.fragment_vert(i, 2));
        Tga_Color rnd;
        for (int col = 0; col < 3; col++)
        {
            rnd[col] = std::rand() % 255;
        }
        triangle_draw(framebuffer, z_buffer, a, b, c, rnd, DRAW_MODE::Filler);
    }

    framebuffer.write_tga_image_file("framebuffer.tga");
    z_buffer.write_tga_image_file("z_buffer.tga");
    return 0;
}