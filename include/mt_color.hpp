#ifndef FA5B1E82_3D8A_473B_A317_D74C285D595B
#define FA5B1E82_3D8A_473B_A317_D74C285D595B

#include <cmath>

#include "mt_application.hpp"

enum Mt_hexColors
{
	Black = 0x000000,
	grey11 = 0x1C1C1C,
	grey21 = 0x363636,
	grey31 = 0x4F4F4F,
	DimGray = 0x696969,
	Gray = 0x808080,
	DarkGray = 0xA9A9A9,
	Silver = 0xC0C0C0,
	LightGrey = 0xD3D3D3,
	Gainsboro = 0xDCDCDC,
	White = 0xFFFFFF,
	SlateBlue = 0x6A5ACD,
	SlateBlue1 = 0x836FFF,
	SlateBlue3 = 0x6959CD,
	DarkSlateBlue = 0x483D8B,
	MidnightBlue = 0x191970,
	Navy = 0x000080,
	DarkBlue = 0x00008B,
	MediumBlue = 0x0000CD,
	Blue = 0x0000FF,
	CornflowerBlue = 0x6495ED,
	RoyalBlue = 0x4169E1,
	DodgerBlue = 0x1E90FF,
	DeepSkyBlue = 0x00BFFF,
	LightSkyBlue = 0x87CEFA,
	SkyBlue = 0x87CEEB,
	LightBlue = 0xADD8E6,
	SteelBlue = 0x4682B4,
	LightSteelBlue = 0xB0C4DE,
	SlateGray = 0x708090,
	LightSlateGray = 0x778899,
	Aqua = 0x00FFFF,
	Cyan = 0x00FFFF,
	DarkTurquoise = 0x00CED1,
	Turquoise = 0x40E0D0,
	MediumTurquoise = 0x48D1CC,
	LightSeaGreen = 0x20B2AA,
	DarkCyan = 0x008B8B,
	Teal = 0x008080,
	Aquamarine = 0x7FFFD4,
	MediumAquamarine = 0x66CDAA,
	CadetBlue = 0x5F9EA0,
	DarkSlateGray = 0x2F4F4F,
	MediumSpringGreen = 0x00FA9A,
	SpringGreen = 0x00FF7F,
	PaleGreen = 0x98FB98,
	LightGreen = 0x90EE90,
	DarkSeaGreen = 0x8FBC8F,
	MediumSeaGreen = 0x3CB371,
	SeaGreen = 0x2E8B57,
	DarkGreen = 0x006400,
	Green = 0x008000,
	ForestGreen = 0x228B22,
	LimeGreen = 0x32CD32,
	Lime = 0x00FF00,
	LawnGreen = 0x7CFC00,
	Chartreuse = 0x7FFF00,
	GreenYellow = 0xADFF2F,
	YellowGreen = 0x9ACD32,
	OliveDrab = 0x6B8E23,
	DarkOliveGreen = 0x556B2F,
	Olive = 0x808000,
	DarkKhaki = 0xBD536B,
	Goldenrod = 0xDAA520,
	DarkGoldenrod = 0xB8860B,
	SaddleBrown = 0x8B4513,
	Sienna = 0xA0522D,
	RosyBrown = 0xBC8F8F,
	Peru = 0xCD853F,
	Chocolate = 0xD2691E,
	SandyBrown = 0xF4A460,
	NavajoWhite = 0xFFDEAD,
	Wheat = 0xF5DEB3,
	BurlyWood = 0xDEB887,
	Tan = 0xD2B48C,
	MediumSlateBlue = 0x7B68EE,
	MediumPurple = 0x9370DB,
	BlueViolet = 0x8A2BE2,
	Indigo = 0x4B0082,
	DarkViolet = 0x9400D3,
	DarkOrchid = 0x9932CC,
	MediumOrchid = 0xBA55D3,
	Purple = 0x800080,
	DarkMagenta = 0x8B008B,
	Fuchsia = 0xFF00FF,
	Magenta = 0xFF00FF,
	Violet = 0xEE82EE,
	Orchid = 0xDA70D6,
	Plum = 0xDDA0DD,
	MediumVioletRed = 0xC71585,
	DeepPink = 0xFF1493,
	HotPink = 0xFF69B4,
	PaleVioletRed = 0xDB7093,
	LightPink = 0xFFB6C1,
	Pink = 0xFFC0CB,
	LightCoral = 0xF08080,
	IndianRed = 0xCD5C5C,
	Crimson = 0xDC143C,
	Maroon = 0x800000,
	DarkRed = 0x8B0000,
	FireBrick = 0xB22222,
	Brown = 0xA52A2A,
	Salmon = 0xFA8072,
	DarkSalmon = 0xE9967A,
	LightSalmon = 0xFFA07A,
	Coral = 0xFF7F50,
	Tomato = 0xFF6347,
	Red = 0xFF0000,
	OrangeRed = 0xFF4500,
	DarkOrange = 0xFF8C00,
	Orange = 0xFFA500,
	Gold = 0xFFD700,
	Yellow = 0xFFFF00,
	Khaki = 0xF0E68C,
	AliceBlue = 0xF0F8FF,
	GhostWhite = 0xF8F8FF,
	Snow = 0xFFFAFA,
	Seashell = 0xFFF5EE,
	FloralWhite = 0xFFFAF0,
	WhiteSmoke = 0xF5F5F5,
	Beige = 0xF5F5DC,
	OldLace = 0xFDF5E6,
	Ivory = 0xFFFFF0,
	Linen = 0xFAF0E6,
	Cornsilk = 0xFFF8DC,
	AntiqueWhite = 0xFAEBD7,
	BlanchedAlmond = 0xFFEBCD,
	Bisque = 0xFFE4C4,
	LightYellow = 0xFFFFE0,
	LemonChiffon = 0xFFFACD,
	LightGoldenrodYellow = 0xFAFAD2,
	PapayaWhip = 0xFFEFD5,
	PeachPuff = 0xFFDAB9,
	Moccasin = 0xFFE4B5,
	PaleGoldenrod = 0xEEE8AA,
	MistyRose = 0xFFE4E1,
	LavenderBlush = 0xFFF0F5,
	Lavender = 0xE6E6FA,
	Thistle = 0xD8BFD8,
	Azure = 0xF0FFFF,
	LightCyan = 0xE0FFFF,
	PowderBlue = 0xB0E0E6,
	PaleTurquoise = 0xAFEEEE,
	Honeydew = 0xF0FFF0,
	MintCream = 0xF5FFFA
};

class Mt_RGBA : public SDL_Color
{
public:
	Mt_RGBA();

	Mt_RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	bool operator==(const SDL_Color &color);

	bool operator!=(const SDL_Color &color);

	Mt_RGBA &operator=(const Mt_RGBA &color);

	friend std::ostream &operator<<(std::ostream &o, const Mt_RGBA &color);

	void negative();

	void hsl(float h, float s, float l);
	void hsv(float h, float s, float v);
	void rgb(int r, int g, int b);

	void bw(int value);
	void hex(int value);
};

template <typename T>
class Mt_GenericRGBA
{
public:
	T r;
	T g;
	T b;
	T a;
	Mt_GenericRGBA() {}
	Mt_GenericRGBA(const T &r, const T &g, const T &b) : r(r), g(g), b(b) {}
	Mt_GenericRGBA(const T &r, const T &g, const T &b, const T &a) : r(r), g(g), b(b), a(a) {}
};

class Mt_color;

class Mt_colormanager
{
public:
	static std::vector<Mt_color *> colors;

	static void update();
};

class Mt_color : public Mt_RGBA
{
private:
	float currentFrame = 0.f;
	float frames = 0.f;

	Mt_RGBA *target = nullptr;

	Mt_GenericRGBA<float> current;
	Mt_GenericRGBA<float> step;

public:
	Mt_color();

	Mt_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	Mt_color &operator=(const SDL_Color &color);

	void fadeInto(Mt_RGBA *target, float frames = 8.f);

	void update();
};

#endif /* FA5B1E82_3D8A_473B_A317_D74C285D595B */
