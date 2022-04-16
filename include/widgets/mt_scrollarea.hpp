#ifndef BFB770F5_2D1B_41B9_991B_2BAB3168F72D
#define BFB770F5_2D1B_41B9_991B_2BAB3168F72D

#include "mt_box.hpp"
#include "mt_button.hpp"

namespace TOOLKIT_NAMESPACE
{
    class Scrollarea : public Box
    {
    private:
        SDL_Rect scroll;
        float progress = 0.f;

        Button *up = nullptr;
        Button *down = nullptr;
        Button *scrollbar = nullptr;

        static const short scrollButtonWidth = 15;
        static const short scrollButtonHeight = 17;
        static const short padding = 2;
        SDL_Rect rect;

        Scrollarea(Window &window, int x, int y, int w, int h, int scroll_w, int scroll_h);
        Scrollarea(const Scrollarea &) = delete;

    public:
        static Scrollarea &create(Window &window, int x, int y, int w, int h, int scroll_w, int scroll_h);
        ~Scrollarea();

        void handleEvent() override;
        void update() override;
        void draw() override;
    };
}

#endif /* BFB770F5_2D1B_41B9_991B_2BAB3168F72D */
