
#include "widgets/mt_box.hpp"

#include "mt_point.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Box::counter;

TOOLKIT_NAMESPACE::Box::Box(Window &window, int x, int y, int w, int h) : Container(window, x, y, w, h)
{

    init();
}
TOOLKIT_NAMESPACE::Box &TOOLKIT_NAMESPACE::Box::create(Window &window, int x, int y, int w, int h)
{
    Box *scroll = new Box(window, x, y, w, h);
    window.add(*scroll);
    return *scroll;
}
TOOLKIT_NAMESPACE::Box::~Box()
{
    Debug("Destroying " << this->id) << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")";
}

void TOOLKIT_NAMESPACE::Box::handleEvent()
{
    HANDLE_WINDOW_EVENTS;

    for (auto widget : widgets)
        widget->handleEvent();
}
void TOOLKIT_NAMESPACE::Box::update()
{
    const int difX = geometry->destR.x - geometry->srcR.x;
    const int difY = geometry->destR.y - geometry->srcR.y;

    anyhover = false;
    for (auto widget : widgets)
    {
        widget->geometry->destR.x = widget->geometry->getX() + difX;
        widget->geometry->destR.y = widget->geometry->getY() + difY;
        widget->geometry->adjustCenter();
        anyhover += widget->isHoverScrollable();
        confineObject(widget);
        widget->update();
    }

    if (Point::mousePos().intercept(geometry->destR))
    {
        onHover();
    }
}
void TOOLKIT_NAMESPACE::Box::draw()
{
    return_if(!visible);

    window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
    for (auto widget : widgets)
        widget->draw();
    window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}
