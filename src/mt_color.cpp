
#include "mt_color.hpp"

// ANCHOR RGBA CLASS
Mt_RGBA::Mt_RGBA()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 255;
}
Mt_RGBA::Mt_RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

bool Mt_RGBA::operator==(const SDL_Color &color)
{
	return (
		this->r == color.r &&
		this->g == color.g &&
		this->b == color.b &&
		this->a == color.a);
}
bool Mt_RGBA::operator!=(const SDL_Color &color)
{
	return (
		this->r != color.r ||
		this->g != color.g ||
		this->b != color.b ||
		this->a != color.a);
}
Mt_RGBA &Mt_RGBA::operator=(const Mt_RGBA &color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;

	return *this;
}
std::ostream &operator<<(std::ostream &o, const Mt_RGBA &color)
{
	o << "(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", " << (int)color.a << ")";
	return o;
}

void Mt_RGBA::negative()
{
	r = 255 - r;
	g = 255 - g;
	b = 255 - b;
}

void Mt_RGBA::hsl(float h, float s, float l)
{
	float C = (1 - fabs((2 * l) - 1)) * s;
	float X = C * (1 - fabs(fmod(h / 60., 2) - 1));
	float m = l - (C / 2);

	float rt, gt, bt;
	if (h >= 0 && h < 60)
	{
		rt = C;
		gt = X;
		bt = 0;
	}
	else if (h >= 60 && h < 120)
	{
		rt = X;
		gt = C;
		bt = 0;
	}
	else if (h >= 120 && h < 180)
	{
		rt = 0;
		gt = C;
		bt = X;
	}
	else if (h >= 180 && h < 240)
	{
		rt = 0;
		gt = X;
		bt = C;
	}
	else if (h >= 240 && h < 300)
	{
		rt = X;
		gt = 0;
		bt = C;
	}
	else
	{
		rt = C;
		gt = 0;
		bt = X;
	}

	r = (rt + m) * 255;
	g = (gt + m) * 255;
	b = (bt + m) * 255;
}
void Mt_RGBA::hsv(float h, float s, float v)
{
	float C = v * s;
	float X = C * (1 - fabs(fmod(h / 60., 2) - 1));
	float m = v - C;

	float rt, gt, bt;
	if (h >= 0 && h < 60)
	{
		rt = C;
		gt = X;
		bt = 0;
	}
	else if (h >= 60 && h < 120)
	{
		rt = X;
		gt = C;
		bt = 0;
	}
	else if (h >= 120 && h < 180)
	{
		rt = 0;
		gt = C;
		bt = X;
	}
	else if (h >= 180 && h < 240)
	{
		rt = 0;
		gt = X;
		bt = C;
	}
	else if (h >= 240 && h < 300)
	{
		rt = X;
		gt = 0;
		bt = C;
	}
	else
	{
		rt = C;
		gt = 0;
		bt = X;
	}

	r = (rt + m) * 255;
	g = (gt + m) * 255;
	b = (bt + m) * 255;
}
void Mt_RGBA::rgb(int r, int g, int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}
void Mt_RGBA::bw(int value)
{
	this->r = value;
	this->g = value;
	this->b = value;
}
void Mt_RGBA::hex(int value)
{
	r = (value >> 16) & 0xFF;
	g = (value >> 8) & 0xFF;
	b = value & 0xFF;
	a = 0xFF;
}

// ANCHOR COLOR MANAGER CLASS
std::vector<Mt_color *> Mt_colormanager::colors;

void Mt_colormanager::update()
{
	for (auto &c : colors)
		c->update();
}

// ANCHOR COLOR CLASS
Mt_color::Mt_color() : Mt_RGBA(0, 0, 0, 255)
{
	Mt_colormanager::colors.emplace_back(this);
}
Mt_color::Mt_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : Mt_RGBA(r, g, b, a)
{
	Mt_colormanager::colors.emplace_back(this);
}

Mt_color &Mt_color::operator=(const SDL_Color &color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;

	return *this;
}

void Mt_color::fadeInto(Mt_RGBA *target, float frames)
{
	if (*this == *target)
		return;

	this->frames = frames;
	this->target = target;

	current.r = this->r;
	step.r = (target->r - this->r) / frames;
	current.g = this->g;
	step.g = (target->g - this->g) / frames;
	current.b = this->b;
	step.b = (target->b - this->b) / frames;
	current.a = this->a;
	step.a = (target->a - this->a) / frames;
}

void Mt_color::update()
{
	if (target == nullptr)
		return;

	current.r += step.r;
	this->r = static_cast<Uint8>(current.r);
	current.g += step.g;
	this->g = static_cast<Uint8>(current.g);
	current.b += step.b;
	this->b = static_cast<Uint8>(current.b);
	current.a += step.a;
	this->a = static_cast<Uint8>(current.a);

	currentFrame++;
	if (currentFrame > frames)
	{
		this->r = target->r;
		this->g = target->g;
		this->b = target->b;
		this->a = target->a;
		target = nullptr;
		currentFrame = 0;
		return;
	}
}