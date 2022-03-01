#ifndef B2172A1A_DED7_46DB_B052_6DA43C9154EB
#define B2172A1A_DED7_46DB_B052_6DA43C9154EB

#include <sstream>
#include <cmath>

#include "mt_application.hpp"

class Mt_point : public SDL_Point
{
public:
	Mt_point()
	{
	}
	Mt_point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Mt_point(const SDL_Point &point)
	{
		this->x = point.x;
		this->y = point.y;
	}
	~Mt_point()
	{
	}

	Mt_point &operator=(const SDL_Point &vec)
	{
		this->x = vec.x;
		this->y = vec.y;

		return *this;
	}

	bool intercept(const SDL_Rect &rect) const
	{
		return (this->x < rect.x + rect.w && this->x >= rect.x &&
				this->y < rect.y + rect.h && this->y >= rect.y);
	}

	void Rotate(float angle)
	{
		Mt_point v(this->x, this->y);
		x = (std::cos(angle) * v.x) - (std::sin(angle) * v.y);
		y = (std::sin(angle) * v.x) + (std::cos(angle) * v.y);
	}

	friend std::ostream &operator<<(std::ostream &o, const Mt_point &vec)
	{
		o << "(" << vec.x << "," << vec.y << ")";
		return o;
	}

	static Mt_point mousePos()
	{
		Mt_point mousePos;

		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		return mousePos;
	}
};

#endif /* B2172A1A_DED7_46DB_B052_6DA43C9154EB */
