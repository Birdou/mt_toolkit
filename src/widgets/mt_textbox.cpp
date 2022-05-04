
#include "widgets/mt_textbox.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Textbox::counter;

TOOLKIT_NAMESPACE::Textbox::Textbox(Widget &widget) : Widget(widget, getClassId())
{
    init();
}
TOOLKIT_NAMESPACE::Textbox::Textbox(Window &window, int x, int y, int w, int h) : Widget(window, getClassId(), x, y, w, h)
{
    init();
}
TOOLKIT_NAMESPACE::Textbox &TOOLKIT_NAMESPACE::Textbox::create(Widget &widget)
{
    TOOLKIT_NAMESPACE::Textbox *tbox = new TOOLKIT_NAMESPACE::Textbox(widget);
    // widget.window.widgets.emplace_back(tbox);
    return *tbox;
}
TOOLKIT_NAMESPACE::Textbox &TOOLKIT_NAMESPACE::Textbox::create(Window &window, int x, int y, int w, int h)
{
    TOOLKIT_NAMESPACE::Textbox *tbox = new TOOLKIT_NAMESPACE::Textbox(window, x, y, w, h);
    // window.widgets.emplace_back(tbox);:
    window.add(*tbox);
    return *tbox;
}

void TOOLKIT_NAMESPACE::Textbox::init()
{
    caret = &Caret::create(*this);

    input = &Label::create(*this);
    input->autoupdate = false;

    scheme = UI_TEXTINPUT_COLOR_SCHEME;

    currentBackgroundColor = scheme.background.normal;
    currentBorderColor = scheme.border.normal;
}

void TOOLKIT_NAMESPACE::Textbox::updateCaretPosition()
{
    if (caretPos_x > 0)
    {
        if (!password)
        {
            int tlen = window.renderer->substrWidth(input->font->getFont(), input->text, 0, caretPos_x);
            caret->geometry->destR.x = geometry->destR.x + tlen + text_x;
        }
        else
        {
            int tlen = window.renderer->substrWidth(input->font->getFont(), "\u25CF");
            caret->geometry->destR.x = geometry->destR.x + (tlen * caretPos_x);
        }
    }
    else
    {
        caret->geometry->destR.x = geometry->destR.x + 5;
    }

    int vspace = input->font->getH();
    caret->geometry->destR.y = input->geometry->destR.y - input->geometry->srcR.y;
    if (caret->geometry->destR.y + vspace <= geometry->destR.y || caret->geometry->destR.y >= geometry->destR.y + geometry->destR.h ||
        caret->geometry->destR.x + caret->geometry->getW() <= geometry->destR.x || caret->geometry->destR.x >= geometry->destR.x + geometry->destR.w)
    {
        caret->visible = false;
    }
    else
    {
        caret->visible = true;
        caret->geometry->confineX(geometry->destR);
        caret->geometry->confineY(geometry->destR);
    }
}

void TOOLKIT_NAMESPACE::Textbox::pointCursor()
{
    updateCaretPosition();

    int xw = geometry->destR.x + geometry->destR.w;
    int cxw = caret->geometry->destR.x + caret->geometry->destR.w;
    if (input->geometry->getW() > geometry->destR.w && cxw > xw)
    {
        text_x -= cxw - xw;
    }
    else if (caret->geometry->destR.x - 25 < geometry->destR.x && text_x < 0)
    {
        text_x += caret->geometry->destR.x - geometry->destR.x;
    }
    text_x = std::min(0, text_x);

    updateCaretPosition();
}

TOOLKIT_NAMESPACE::Textbox::~Textbox()
{
    Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

    if (caret)
        delete caret;
    if (input)
        delete input;
}

std::string TOOLKIT_NAMESPACE::Textbox::str() const
{
    return input->text;
}
void TOOLKIT_NAMESPACE::Textbox::str(const std::string &text)
{
    input->text = text;

    caretPos_x = input->text.length();

    onTextModified();
}

void TOOLKIT_NAMESPACE::Textbox::handleMouse()
{
    hoverScroll = false;
    // if (window.hovering == this)
    // window.hovering = nullptr;

    if (Point::mousePos().intercept(geometry->destR))
    {
        onHover();
        hoverScroll = true;
        // if (window.hovering == nullptr)
        // {
        // window.hovering = this;
        SetCursor(SDL_SYSTEM_CURSOR_IBEAM);
        fadeToHover();
        // }
        if (!focused)
        {
            if (window.event.type == SDL_MOUSEBUTTONDOWN)
            {
                switch (window.event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    onMouseDown();
                    // if (window.hovering == this)
                    // {
                    onFocus();
                    if (!SDL_IsTextInputActive())
                    {
                        Debug("Start text input");
                        SDL_StartTextInput();
                    }
                    fadeToFocused();
                    focused = true;
                    released = false;
                    // }
                    break;
                default:
                    break;
                }
            }
        }
    }
    else if (window.event.type == SDL_MOUSEBUTTONDOWN && released && focused)
    {
        SetCursor(SDL_SYSTEM_CURSOR_ARROW);
        switch (window.event.button.button)
        {
        case SDL_BUTTON_LEFT:
            onLostFocus();
            // if (!dynamic_cast<TOOLKIT_NAMESPACE::Textbox *>(window.hovering))
            // {
            Debug("Stop text input");
            SDL_StopTextInput();
            // }
            fadeToNormal();
            focused = false;
            break;
        }
    }
    else if (!focused)
    {
        SetCursor(SDL_SYSTEM_CURSOR_ARROW);
        onMouseLeave();
        // if (window.hovering == this)
        // {
        fadeToNormal();
        // window.hovering = nullptr;
        // }
    }
    else if (released)
    {
        SetCursor(SDL_SYSTEM_CURSOR_ARROW);
    }

    if (window.event.type == SDL_MOUSEBUTTONUP)
    {
        switch (window.event.button.button)
        {
        case SDL_BUTTON_LEFT:
            onMouseUp();
            released = true;
            break;
        default:
            break;
        }
    }
}

void TOOLKIT_NAMESPACE::Textbox::handleEvent()
{
    HANDLE_WINDOW_EVENTS;

    return_if(!visible);

    if (/*window.hovering == this &&*/ window.event.type == SDL_MOUSEWHEEL)
        wheel();

    if (focused)
    {
        switch (window.event.type)
        {
        case SDL_KEYDOWN:
            switch (window.event.key.keysym.sym)
            {
            case SDLK_c:
                copy();
                break;
            }
            break;
        }

        if (editable)
        {
            switch (window.event.type)
            {
            case SDL_KEYDOWN:
                switch (window.event.key.keysym.sym)
                {
                case SDLK_UP:
                    up();
                    break;
                case SDLK_LEFT:
                    left();
                    break;
                case SDLK_DOWN:
                    down();
                    break;
                case SDLK_RIGHT:
                    right();
                    break;
                case SDLK_HOME:
                    home();
                    break;
                case SDLK_END:
                    end();
                    break;
                case SDLK_BACKSPACE:
                    backspace();
                    break;
                case SDLK_DELETE:
                    del();
                    break;
                case SDLK_RETURN:
                    enter();
                    break;
                case SDLK_TAB:
                    tab();
                    break;
                case SDLK_v:
                    paste();
                    break;
                }
                break;
            case SDL_TEXTINPUT:
                textInput();
                break;
            }
        }
    }
}

void TOOLKIT_NAMESPACE::Textbox::update()
{
    return_if(!visible);

    handleMouse();
    if (password)
    {
        std::string text = input->text;
        std::stringstream stream;
        for (size_t i = 0; i < text.length(); i++)
            stream << "\u25CF";

        str(stream.str());
        input->update();
        str(text);
    }
    else
    {
        input->update();
    }

    if (focused && editable)
        caret->update();

    input->geometry->srcR.x = -text_x;

    input->geometry->destR.x = geometry->destR.x;
    input->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + ((geometry->getH() - input->geometry->getH()) / 2);

    input->geometry->srcR.w = input->geometry->destR.w = std::min(text_x + input->geometry->getW(), geometry->destR.w);
    if (input->geometry->destR.w > 0)
    {
        input->visible = true;
        input->geometry->confineY(geometry->destR);
    }

    updateCaretPosition();
}

void TOOLKIT_NAMESPACE::Textbox::draw()
{
    return_if(!visible);

    window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

    input->draw();

    if (focused && editable)
        caret->draw();
    window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}