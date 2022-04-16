#ifndef B2172A1A_DED7_46DB_B052_6DA43C9154EB
#define B2172A1A_DED7_46DB_B052_6DA43C9154EB

#include <sstream>
#include <cmath>

#include "mt_application.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Point : public SDL_Point
	{
	public:
		Point();
		Point(int x, int y);
		Point(const SDL_Point &point);
		~Point();

		Point &operator=(const SDL_Point &vec);

		bool intercept(const SDL_Rect &rect) const;

		void rotate(float angle);

		friend std::ostream &operator<<(std::ostream &o, const Point &vec)
		{
			o << "(" << vec.x << "," << vec.y << ")";
			return o;
		}

		static Point mousePos();
	};
}
#endif /* B2172A1A_DED7_46DB_B052_6DA43C9154EB */
