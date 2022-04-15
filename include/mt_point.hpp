#ifndef B2172A1A_DED7_46DB_B052_6DA43C9154EB
#define B2172A1A_DED7_46DB_B052_6DA43C9154EB

#include <sstream>
#include <cmath>

#include "mt_application.hpp"

class Mt_point : public SDL_Point
{
public:
	Mt_point();

	Mt_point(int x, int y);

	Mt_point(const SDL_Point& point);

	~Mt_point();


	Mt_point& operator=(const SDL_Point& vec);


	bool intercept(const SDL_Rect& rect) const;


	void Rotate(float angle);


	friend std::ostream& operator<<(std::ostream& o, const Mt_point& vec)
	{
		o << "(" << vec.x << "," << vec.y << ")";
		return o;
	}


	static Mt_point mousePos();

};

#endif /* B2172A1A_DED7_46DB_B052_6DA43C9154EB */
