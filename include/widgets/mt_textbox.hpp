#ifndef E1D38D69_904C_4A24_A328_320F8885D04E
#define E1D38D69_904C_4A24_A328_320F8885D04E

#include "mt_widget.hpp"
#include "mt_caret.hpp"
#include "mt_label.hpp"

#include <functional>

template <typename T>
using Function = std::function<T>;

namespace TOOLKIT_NAMESPACE
{
    class Textbox : public Widget
    {
    private:
        WIDGET_CLASS("TEXTBOX");

    protected:
        Label *input = nullptr;
        Caret *caret = nullptr;

        size_t caretPos_x = 0;
        size_t caretPos_y = 0;
        int text_x = 0;
        int text_y = 0;

        Textbox(Widget &widget);
        Textbox(Window &window, int x, int y, int w, int h);
        Textbox(const Textbox &) = delete;

        void init();

        virtual void updateCaretPosition();

        virtual void pointCursor();

    public:
        static Textbox &create(Widget &widget);
        static Textbox &create(Window &window, int x, int y, int w, int h);

        bool editable = true;
        bool password = false;

        virtual ~Textbox();

        virtual std::string str() const;
        virtual void str(const std::string &text);

        void handleMouse();

        Function<void()> copy = [this]()
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                SDL_SetClipboardText(str().c_str());
            }
        };
        Function<void()> paste = [this]()
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                const char *text = SDL_GetClipboardText();
                std::string ftext(text);
                replaceAll(ftext, "\t", "    ");
                str(ftext);
                delete[] text;

                onTextModified();
                pointCursor();
            }
        };
        Function<void()> wheel = [this]() {};
        Function<void()> enter = [this]() {};
        Function<void()> backspace = [this]()
        {
            if (input->text.size() > 0 && caretPos_x > 0)
            {
                caretPos_x--;
                input->geometry->srcR.x -= window.renderer->substrWidth(font->getFont(), input->text, caretPos_x, 1);
                if (input->geometry->srcR.x < 0)
                    input->geometry->srcR.x = 0;
                input->text.erase(input->text.begin() + caretPos_x);

                // Handling accents
                if (caretPos_x > 0)
                    if (input->text[caretPos_x - 1] == -61)
                    {
                        caretPos_x--;
                        input->text.erase(input->text.begin() + caretPos_x);
                    }

                onTextModified();
                pointCursor();
            }
        };
        Function<void()> del = [this]()
        {
            if (input->text.size() > 0 && caretPos_x < input->text.length())
            {

                if (input->text[caretPos_x] == -61)
                {
                    input->text.erase(input->text.begin() + caretPos_x);
                    input->text.erase(input->text.begin() + caretPos_x);
                }
                else
                {
                    input->text.erase(input->text.begin() + caretPos_x);
                }

                onTextModified();
                pointCursor();
            }
        };
        Function<void()> up = [this]() {};
        Function<void()> left = [this]()
        {
            if (caretPos_x > 0)
            {
                caretPos_x--;
            }
            pointCursor();
        };
        Function<void()> right = [this]()
        {
            if (caretPos_x + 1 <= input->text.length())
            {
                caretPos_x++;
            }
            pointCursor();
        };
        Function<void()> down = [this]() {};
        Function<void()> end = [this]()
        {
            caretPos_x = input->text.length();
            if (input->geometry->destR.w >= geometry->destR.w)
                text_x = input->geometry->destR.w - geometry->destR.w;
            pointCursor();
        };
        Function<void()> home = [this]()
        {
            caretPos_x = 0;
            text_x = 0;
            pointCursor();
        };
        Function<void()> tab = [this]() {};
        Function<void()> textInput = [this]()
        {
            std::string text(window.event.text.text);
            if (!(SDL_GetModState() & KMOD_CTRL && (text[0] == 'c' || text[0] == 'C' || text[0] == 'v' || text[0] == 'V')))
            {
                input->text.insert(caretPos_x, text);
                input->update();

                caretPos_x += text.length();

                onTextModified();
                pointCursor();
            }
        };

        void handleEvent() override;
        void update() override;
        void draw() override;
    };
}
#endif /* E1D38D69_904C_4A24_A328_320F8885D04E */
