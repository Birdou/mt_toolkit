#ifndef B2172A1A_DED7_46DB_B052_6DA43C9154EB
#define B2172A1A_DED7_46DB_B052_6DA43C9154EB

#include <sstream>
#include <cmath>

#include "mt_application.hpp"

template <typename Type>
class Mt_vector
{
public:
	Type x;
	Type y;

	Mt_vector() {}
	Mt_vector(Type x, Type y)
	{
		this->x = x;
		this->y = y;
	}
	Mt_vector(const Mt_vector<Type> &vec)
	{
		*this = vec;
	}
	Mt_vector(const SDL_Point &point)
	{
		this->x = point.x;
		this->y = point.y;
	}
	~Mt_vector() {}

	Mt_vector<Type> &operator=(const Mt_vector<Type> &vec)
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
		Mt_vector v(this->x, this->y);
		x = (cos(angle) * v.x) - (sin(angle) * v.y);
		y = (sin(angle) * v.x) + (cos(angle) * v.y);
	}

	static Mt_vector<Type> Add(const Mt_vector<Type> &vec1, const Mt_vector<Type> &vec2) { return Mt_vector<Type>(vec1.x + vec2.x, vec1.y + vec2.y); }
	static Mt_vector<Type> Subtract(const Mt_vector<Type> &vec1, const Mt_vector<Type> &vec2) { return Mt_vector<Type>(vec1.x - vec2.x, vec1.y - vec2.y); }
	static Mt_vector<Type> Multiply(const Mt_vector<Type> &vec1, const Mt_vector<Type> &vec2) { return Mt_vector<Type>(vec1.x * vec2.x, vec1.y * vec2.y); }
	static Mt_vector<Type> Divide(const Mt_vector<Type> &vec1, const Mt_vector<Type> &vec2) { return Mt_vector<Type>(vec1.x / vec2.x, vec1.y / vec2.y); }

	Mt_vector<Type> &operator+(const Mt_vector<Type> &vec)
	{
		return Add(*this, vec);
	}
	Mt_vector<Type> &operator-(const Mt_vector<Type> &vec)
	{
		return Subtract(*this, vec);
	}
	Mt_vector<Type> &operator*(const Mt_vector<Type> &vec)
	{
		return Multiply(*this, vec);
	}
	Mt_vector<Type> &operator/(const Mt_vector<Type> &vec)
	{
		return Divide(*this, vec);
	}

	Mt_vector<Type> &operator+=(const Mt_vector<Type> &vec)
	{
		return (*this = Add(*this, vec));
	}
	Mt_vector<Type> &operator-=(const Mt_vector<Type> &vec)
	{
		return (*this = Subtract(*this, vec));
	}
	Mt_vector<Type> &operator*=(const Mt_vector<Type> &vec)
	{
		return (*this = Multiply(*this, vec));
	}
	Mt_vector<Type> &operator/=(const Mt_vector<Type> &vec)
	{
		return (*this = Divide(*this, vec));
	}

	bool operator==(const Mt_vector<Type> &vec)
	{
		if (this->x != vec.x || this->y != vec.y)
			return false;

		return true;
	}
	bool operator!=(const Mt_vector<Type> &vec)
	{
		if (this->x == vec.x || this->y == vec.y)
			return false;

		return true;
	}

	template <typename cast>
	Mt_vector<Type> operator*(const cast &i)
	{
		return Mt_vector<Type>(this->x * i, this->y * i);
	}

	friend std::ostream &operator<<(std::ostream &stream, const Mt_vector<Type> &vec)
	{
		stream << "(" << vec.x << "," << vec.y << ")";
		return stream;
	}

	static Mt_vector<Type> mousePos()
	{
		Mt_vector<Type> mousePos;

		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		return mousePos;
	}
};

#endif /* B2172A1A_DED7_46DB_B052_6DA43C9154EB */
