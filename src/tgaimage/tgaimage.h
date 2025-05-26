#ifndef __TGAIMAGE__
#define __TGAIMAGE__
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct Tga_Header
{
    std::uint8_t  id_length            = 0;
    std::uint8_t  color_map_type       = 0;
    std::uint8_t  image_type           = 0;
    std::uint16_t color_map_origin     = 0;
    std::uint16_t color_map_length     = 0;
    std::uint8_t  color_map_entry_size = 0;
    std::uint16_t x_origin             = 0;
    std::uint16_t y_origin             = 0;
    std::uint16_t image_width          = 0;
    std::uint16_t image_height         = 0;
    std::uint8_t  bits_per_pixel       = 0;
    std::uint8_t  image_descriptor     = 0; 
};
#pragma pack(pop)

struct Tga_Color
{
    std::uint8_t bgra[4] = {0, 0, 0, 0};
    std::uint8_t bytes_pre_pixel = 4;
    std::uint8_t operator[] (int i) { return bgra[i]; }
};

struct Tga_Image
{
    enum Tga_Format {GRAY = 1, RGB = 3, RGBA = 4};
    Tga_Image() = default;
    Tga_Image(const int width, const int height, const int bytes_pre_pixel);

    bool read_tga_image_file(const std::string filename);
    bool write_tga_image_file(const std::string filename, const bool vflip = true, const bool rle = true) const;

    Tga_Color get_color(const int x, const int y) const;
    void      set_color(const int x, const int y, const Tga_Color& color);

    void flip_horizontally();
    void flip_vertically();

    int width() const;
    int height() const;


private:
    bool   load_rle_data(std::ifstream& input);
    bool unload_rle_data(std::ofstream& output) const;

private:
    int image_width              = 0;
    int image_height             = 0;
    std::uint8_t bytes_pre_pixel = 0;
    std::vector<std::uint8_t> data;
};

#endif // __TGAIMAGE__