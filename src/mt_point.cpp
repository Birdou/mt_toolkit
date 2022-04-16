
#include "mt_point.hpp"

TOOLKIT_NAMESPACE::Point::Point()
{
}
TOOLKIT_NAMESPACE::Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}
TOOLKIT_NAMESPACE::Point::Point(const SDL_Point &point)
{
    this->x = point.x;
    this->y = point.y;
}
TOOLKIT_NAMESPACE::Point::~Point()
{
}

TOOLKIT_NAMESPACE::Point &TOOLKIT_NAMESPACE::Point::operator=(const SDL_Point &vec)
{
    this->x = vec.x;
    this->y = vec.y;

    return *this;
}

bool TOOLKIT_NAMESPACE::Point::intercept(const SDL_Rect &rect) const
{
    return (this->x < rect.x + rect.w && this->x >= rect.x &&
            this->y < rect.y + rect.h && this->y >= rect.y);
}

void TOOLKIT_NAMESPACE::Point::rotate(float angle)
{
    Point v(this->x, this->y);
    x = (std::cos(angle) * v.x) - (std::sin(angle) * v.y);
    y = (std::sin(angle) * v.x) + (std::cos(angle) * v.y);
}

TOOLKIT_NAMESPACE::Point TOOLKIT_NAMESPACE::Point::mousePos()
{
    Point mousePos;

    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    return mousePos;
}