
#include "widgets/mt_container.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Container::counter;

TOOLKIT_NAMESPACE::Container::Container(Window &window, const std::string &id, int x, int y, int w, int h) : Widget(window, id, x, y, w, h)
{
    init();
}
TOOLKIT_NAMESPACE::Container::~Container()
{
    Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");
}
void TOOLKIT_NAMESPACE::Container::init()
{
    currentBackgroundColor.hex(hexColors::White).a = 0;
    currentBorderColor = RGBA(130, 145, 144);
}

void TOOLKIT_NAMESPACE::Container::confineObject(Widget *widget)
{
    if (geometry->intercept(*widget->geometry))
    {
        widget->visible = false;
    }
    else
    {
        widget->visible = true;
        widget->geometry->confine(geometry->destR);
    }
}
