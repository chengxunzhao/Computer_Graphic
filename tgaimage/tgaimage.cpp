#include "tgaimage.h"
#include <iostream>

Tga_Image::Tga_Image(const int width, const int height, const int bytes_pre_pixel) :
image_width(width), image_height(height), bytes_pre_pixel(bytes_pre_pixel),
data(image_width * image_height * bytes_pre_pixel, 0)
{
}

bool Tga_Image::read_tga_image_file(const std::string filename)
{
    bool ret = false;
    std::ifstream input;
    do
    {
        input.open(filename, std::ios::binary);
        if (!input.is_open())
        {
            std::cerr << "Open file failed, filename: " << filename << std::endl;
            break; 
        }
        Tga_Header header;
        input.read(reinterpret_cast<char*>(&header), sizeof(header));
        if (!input.good())
        {
            std::cerr << "Reading header failed, filename: " << filename << std::endl;
            break;
        }
        image_width  = header.image_width;
        image_height = header.image_height;
        bytes_pre_pixel = header.bits_per_pixel >> 3;
        if ((image_width < 0 || image_height < 0)
         || (bytes_pre_pixel != GRAY && bytes_pre_pixel != RGB && bytes_pre_pixel != RGBA))
        {
            std::cerr << "image: "<< filename << " value(width or height or bytes_pre_pixel) is failed" << std::endl;
            break;
        }
        size_t sum_bytes = image_width * image_height * bytes_pre_pixel;
        data = std::vector<std::uint8_t>(sum_bytes, 0);
        if (2 == header.image_type || 3 == header.image_type)
        {
            input.read(reinterpret_cast<char*>(data.data()), sum_bytes);
            if (!input.good())
            {
                std::cerr << "Reading data failed, filename: " << filename << std::endl;
                break;
            }
        }
        else if (10 == header.image_type || 11 == header.image_type)
        {
            if (!load_rle_data(input))
            {
                std::cerr << "Reading rle data failed, filename: " << filename << std::endl;
                break;
            }
        }
        else
        {
            std::cerr << filename << " unknown file format: " << (int)header.image_type << std::endl;
            break; 
        }
        if (!(header.image_descriptor & 0x20))
        {
            flip_vertically();
        }
        if (header.image_descriptor & 0x10)
        {
            flip_horizontally();
        }
        ret = true;
    } while (0);
    if (ret)
    {
        std::cerr << "Read tga image file: " << filename
                  << " success, width: "     << image_width
                  << " height: "             << image_height
                  << " bytes_pre_pixel: "    << bytes_pre_pixel << std::endl;
    }
    return ret;
}

bool Tga_Image::load_rle_data(std::ifstream &input)
{
    bool ret = false;
    size_t pixel_count = image_width * image_height;
    size_t pixel_current = 0;
    size_t bytes_current = 0;
    Tga_Color color_buffer;
    do
    {
       std::uint8_t chunk_header = 0;
       chunk_header = input.get();
       if (chunk_header < 128)
       {
            chunk_header++;
            for (int i = 0; i < chunk_header; i++)
            {
                input.read(reinterpret_cast<char*>(color_buffer.bgra), bytes_pre_pixel);
                if (!input.good())
                {
                    std::cerr << "Reading tga color buffer failed, current pixel:" << pixel_current << std::endl;
                    return ret;
                }
                for (int t = 0; t < bytes_pre_pixel; t++)
                {
                    data[bytes_current++] = color_buffer.bgra[t];
                }
                pixel_current++;
                if (pixel_current > pixel_count)
                {
                    std::cerr << "Too many pixel read" << std::endl;
                    return ret;
                }
            }
       }
       else
       {
            chunk_header -= 127;
            input.read(reinterpret_cast<char*>(color_buffer.bgra), bytes_pre_pixel);
            if (!input.good())
            {
                std::cerr << "Reading from rle data tga color buffer failed, current bytes: " << bytes_current << std::endl;
                return ret;
            }
            for (int i = 0; i < chunk_header; i++)
            {
                for (int t = 0; t < bytes_pre_pixel; t++)
                {
                    data[bytes_current++] = color_buffer.bgra[t];
                }
                pixel_current++;
                if (pixel_current > pixel_count)
                {
                    std::cerr << "Too many pixel read" << std::endl;
                    return ret;
                }
            }
       }
    } while (pixel_current < pixel_count);
    ret = true;
    return ret;
}

bool Tga_Image::write_tga_image_file(const std::string filename, const bool vflip, const bool rle) const
{
    constexpr std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    bool ret = false;
    do
    {
        std::ofstream output;
        output.open(filename, std::ios::binary);
        if (!output.is_open())
        {
            std::cerr << "Open file: " << filename << " failed" << std::endl;
            break;
        }
        Tga_Header header = {};
        header.image_width      = image_width;
        header.image_height     = image_height;
        header.bits_per_pixel   = bytes_pre_pixel << 3;
        header.image_type       = (bytes_pre_pixel == GRAY) ? (rle ? 11 : 3) : (rle ? 10 : 2);
        header.image_descriptor = vflip ? 0x00 : 0x20;
        output.write(reinterpret_cast<const char*>(&header), sizeof(header));
        if (!output.good())
        {
            std::cerr << "Writing header failed, filename: " << filename << std::endl;
            break;
        }
        if (!rle)
        {
            output.write(reinterpret_cast<const char*>(data.data()), image_width * image_height * bytes_pre_pixel);
            if (!output.good())
            {
                std::cerr << "Writing data failed, filename: " << filename << std::endl;
                break;
            }
        }
        else if(!unload_rle_data(output))
        {
            std::cerr << "unload rle data failed, filename: " << filename << std::endl;
            break;
        }
        output.write(reinterpret_cast<const char*>(developer_area_ref), sizeof(developer_area_ref));
        if (!output.good())
        {
            std::cerr << "Writing developer area ref failed, filename: " << filename << std::endl;
            break;
        }
        output.write(reinterpret_cast<const char*>(extension_area_ref), sizeof(extension_area_ref));
        if (!output.good())
        {
            std::cerr << "Writing extension area ref failed, filename: " << filename << std::endl;
            break;  
        }
        output.write(reinterpret_cast<const char*>(footer), sizeof(footer));
        if (!output.good())
        {
            std::cerr << "Writing footer failed, filename: " << filename << std::endl;
            break;
        }
        ret = true;
    } while (0);
    
    return ret;
}

bool Tga_Image::unload_rle_data(std::ofstream &output) const
{
    bool ret = false;
    const std::uint8_t max_chunk_length = 128;
    size_t pixels_count = image_width * image_height;
    size_t pixels_current = 0;
    while(pixels_current < pixels_count)
    {
        size_t chunk_start   = pixels_current * bytes_pre_pixel;
        size_t bytes_current = pixels_current * bytes_pre_pixel;
        std::uint8_t run_length = 1;
        bool raw = true;
        while(pixels_current < pixels_count && run_length < max_chunk_length)
        {
            bool succ_eq = true;
            for (int t = 0; succ_eq && t < bytes_pre_pixel; t++)
            {
                succ_eq = (data[bytes_current + t] == data[bytes_current + t + bytes_pre_pixel]);
            }
            bytes_current += bytes_pre_pixel;
            if (1 == run_length)
            {
                raw = !succ_eq;
            }
            if (raw && succ_eq)
            {
                run_length--;
                break;
            }
            if (!raw && !succ_eq)
            {
                break;
            }
            run_length++;
        }
        pixels_current += run_length;
        output.put(raw ? run_length - 1 : run_length + 127);
        if (!output.good())
        {
            std::cerr << "Writing run length failed" << std::endl;
            return ret;
        }
        output.write(reinterpret_cast<const char*>(data.data() + chunk_start), (raw ? run_length * bytes_pre_pixel : bytes_pre_pixel));
        if (!output.good())
        {
            std::cerr << "Writing run length data failed" << std::endl;
            return ret;
        }
    }
    ret = true;
    return ret;
}

Tga_Color Tga_Image::get_color(const int x, const int y) const
{
    if (!data.size() || x < 0 || y < 0 || x >= image_width || y >= image_height)
    {
        return {};
    }
    Tga_Color color_ret = {0, 0, 0, bytes_pre_pixel};
    for (int i = 0; i < bytes_pre_pixel; i++)
    {
        color_ret.bgra[i] = data[(x + y * image_width) * bytes_pre_pixel + i]; 
    }
    
    return color_ret;
}

void Tga_Image::set_color(const int x, const int y,const Tga_Color& color)
{
    if (!data.size() || x < 0 || y < 0 || x >= image_width || y >= image_height)
    {
        return;
    }
    for(int t = 0; t < bytes_pre_pixel; t++)
    {
        data[(x + y * image_width) * bytes_pre_pixel + t] = color.bgra[t];
    }
}

void Tga_Image::flip_horizontally()
{
    for (int i = 0; i < image_width / 2; i++)
    {
        for (int j = 0; j < image_height; j++)
        {
            for (int k = 0; k < bytes_pre_pixel; k++)
            {
                std::swap(data[(i + j * image_width) * bytes_pre_pixel + k], data[(image_width - 1 - i + j * image_width) * bytes_pre_pixel + k]);
            }
        }
    }
}

void Tga_Image::flip_vertically()
{
    for (int i = 0; i < image_width; i++)
    {
        for (int j = 0; j < image_height / 2; j++)
        {
            for (int k = 0; k < bytes_pre_pixel; k++)
            {
                std::swap(data[(i + j * image_width) * bytes_pre_pixel + k], data[(i + (image_height - 1 - j) * image_width) * bytes_pre_pixel + k]);
            }
        }
    }
}

int Tga_Image::width() const
{
    return image_width;
}

int Tga_Image::height() const
{
    return image_height;
}