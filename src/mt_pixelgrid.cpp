
#include "mt_pixelgrid.hpp"

#include <fstream>

void TOOLKIT_NAMESPACE::Pixelgrid::updateSurface()
{
    unsigned char *pixels = (unsigned char *)surface->pixels;
    for (size_t y = 0; y < h; ++y)
    {
        for (size_t x = 0; x < w; ++x)
        {
            pixels[4 * (y * surface->w + x) + 2] = data[y][x].r; // 2103
            pixels[4 * (y * surface->w + x) + 1] = data[y][x].g; // 2103
            pixels[4 * (y * surface->w + x) + 0] = data[y][x].b; // 2103
            pixels[4 * (y * surface->w + x) + 3] = 255;          // 2103
        }
    }
}

TOOLKIT_NAMESPACE::Pixelgrid::Pixelgrid()
{
}

void TOOLKIT_NAMESPACE::Pixelgrid::init()
{
}
TOOLKIT_NAMESPACE::Pixelgrid::Pixelgrid(size_t w, size_t h)
{
    alloc(w, h);
}
TOOLKIT_NAMESPACE::Pixelgrid::~Pixelgrid()
{
    erase();
}
TOOLKIT_NAMESPACE::RGBA &TOOLKIT_NAMESPACE::Pixelgrid::at(size_t x, size_t y)
{
    return data[y][x];
}
const TOOLKIT_NAMESPACE::RGBA &TOOLKIT_NAMESPACE::Pixelgrid::get(size_t x, size_t y) const
{
    return data[y][x];
}

void TOOLKIT_NAMESPACE::Pixelgrid::alloc(size_t width, size_t height)
{
    erase();
    w = width;
    h = height;

    data = new TOOLKIT_NAMESPACE::RGBA *[height];
    for (size_t i = 0; i < height; ++i)
        data[i] = new TOOLKIT_NAMESPACE::RGBA[width];
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    if (surface == nullptr)
    {
        SDL_PrintError(Error);
    }
}

void TOOLKIT_NAMESPACE::Pixelgrid::erase()
{
    if (w == 0 || h == 0)
        return;

    for (size_t i = 0; i < h; ++i)
        delete[] data[i];
    delete[] data;
    SDL_FreeSurface(surface);

    w = 0;
    h = 0;

    data = nullptr;
}

size_t TOOLKIT_NAMESPACE::Pixelgrid::width() const noexcept
{
    return w;
}
size_t TOOLKIT_NAMESPACE::Pixelgrid::height() const noexcept
{
    return h;
}

void TOOLKIT_NAMESPACE::Pixelgrid::savePPMA(const std::string &file)
{
    std::ofstream save;
    save.open(file);

    save << "P3\r";
    save << w << " " << h << "\r";
    save << "255\r";

    for (size_t i = 0; i < h; ++i) // o laço superior é relacionado à altura devido a ordem de gravação.
        for (size_t j = 0; j < w; ++j)
            save << data[i][j].r << " " << data[i][j].g << " " << data[i][j].b << "\r";

    save.close();
}
void TOOLKIT_NAMESPACE::Pixelgrid::savePPMB(const std::string &file)
{
    std::ofstream save;
    save.open(file, std::ios_base::binary);

    save << "P6\r";
    save << w << " " << h << "\r";
    save << "255\r";

    for (size_t i = 0; i < h; ++i) // o laço superior é relacionado à altura devido a ordem de gravação.
        for (size_t j = 0; j < w; ++j)
            save << (unsigned char)data[i][j].r << (unsigned char)data[i][j].g << (unsigned char)data[i][j].b;

    save.close();
}
void TOOLKIT_NAMESPACE::Pixelgrid::saveJPG(const std::string &file)
{
    updateSurface();
    if (IMG_SaveJPG(surface, file.c_str(), 100) < 0)
    {
        SDL_PrintError(Error);
    }
}
void TOOLKIT_NAMESPACE::Pixelgrid::savePNG(const std::string &file)
{
    updateSurface();
    if (IMG_SavePNG(surface, file.c_str()) < 0)
    {
        SDL_PrintError(Error);
    }
}
void TOOLKIT_NAMESPACE::Pixelgrid::saveBMP(const std::string &file)
{
    updateSurface();
    if (SDL_SaveBMP(surface, file.c_str()) < 0)
    {
        SDL_PrintError(Error);
    }
}