#ifndef CDD91408_80F0_4F9A_B500_C1CF5377C5E5
#define CDD91408_80F0_4F9A_B500_C1CF5377C5E5

#include "mt_core.hpp"
#include "mt_color.hpp"

namespace TOOLKIT_NAMESPACE
{
    class Pixelgrid
    {
    protected:
        SDL_Surface *surface = nullptr;
        RGBA **data = nullptr;
        size_t w = 0, h = 0;

        void updateSurface();

        Pixelgrid();

        void init();

    public:
        Pixelgrid(size_t w, size_t h);
        ~Pixelgrid();

        virtual RGBA &at(size_t x, size_t y);
        virtual const RGBA &get(size_t x, size_t y) const;

        virtual void alloc(size_t width, size_t height);
        virtual void erase();

        size_t width() const noexcept;
        size_t height() const noexcept;

        void savePPMA(const std::string &file);
        void savePPMB(const std::string &file);
        void saveJPG(const std::string &file);
        void savePNG(const std::string &file);
        void saveBMP(const std::string &file);
    };
}

#endif /* CDD91408_80F0_4F9A_B500_C1CF5377C5E5 */
