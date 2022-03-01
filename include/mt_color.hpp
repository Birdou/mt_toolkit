#ifndef FA5B1E82_3D8A_473B_A317_D74C285D595B
#define FA5B1E82_3D8A_473B_A317_D74C285D595B

#include <cmath>

#include "mt_application.hpp"

enum Mt_hexColors
{
	// ANCHOR PINK COLORS
	Pink = 0xFFC0CB,
	HotPink = 0xFF69B4,
	LightPink = 0xFFB6C1,
	DeepPink = 0xFF1493,
	PaleVioletRed = 0xDB7093,
	MediumVioletRed = 0xC71585,

	// ANCHOR PURPLE COLORS
	Lavender = 0xE6E6FA,
	Thistle = 0xD8BFD8,
	Plum = 0xDDA0DD,
	Orchid = 0xDA70D6,
	Violet = 0xEE82EE,
	Fuchsia = 0xFF00FF,
	Magenta = 0xFF00FF,
	MediumOrchid = 0xBA55D3,
	DarkOrchid = 0x9932CC,
	DarkViolet = 0x9400D3,
	BlueViolet = 0x8A2BE2,
	DarkMagenta = 0x8B008B,
	Purple = 0x800080,
	MediumPurple = 0x9370DB,
	MediumSlateBlue = 0x7B68EE,
	SlateBlue = 0x6A5ACD,
	DarkSlateBlue = 0x483D8B,
	RebeccaPurple = 0x663399,
	Indigo = 0x4B0082,

	// ANCHOR RED COLORS
	LightSalmon = 0xFFA07A,
	Salmon = 0xFA8072,
	DarkSalmon = 0xE9967A,
	LightCoral = 0xF08080,
	IndianRed = 0xCD5C5C,
	Crimson = 0xDC143C,
	Red = 0xFF0000,
	FireBrick = 0xB22222,
	DarkRed = 0x8B0000,

	// ANCHOR ORANGE COLORS
	Orange = 0xFFA500,
	DarkOrange = 0xFF8C00,
	Coral = 0xFF7F50,
	Tomato = 0xFF6347,
	OrangeRed = 0xFF4500,

	// ANCHOR YELLOW COLORS
	Gold = 0xFFD700,
	Yellow = 0xFFFF00,
	LightYellow = 0xFFFFE0,
	LemonChiffon = 0xFFFACD,
	LightGoldenrodYellow = 0xFAFAD2,
	PapayaWhip = 0xFFEFD5,
	Moccasin = 0xFFE4B5,
	PeachPuff = 0xFFDAB9,
	PaleGoldenrod = 0xEEE8AA,
	Khaki = 0xF0E68C,
	DarkKhaki = 0xBD536B,

	// ANCHOR GREEN COLORS
	GreenYellow = 0xADFF2F,
	Chartreuse = 0x7FFF00,
	LawnGreen = 0x7CFC00,
	Lime = 0x00FF00,
	LimeGreen = 0x32CD32,
	PaleGreen = 0x98FB98,
	LightGreen = 0x90EE90,
	MediumSpringGreen = 0x00FA9A,
	SpringGreen = 0x00FF7F,
	MediumSeaGreen = 0x3CB371,
	SeaGreen = 0x2E8B57,
	ForestGreen = 0x228B22,
	Green = 0x008000,
	DarkGreen = 0x006400,
	YellowGreen = 0x9ACD32,
	OliveDrab = 0x6B8E23,
	DarkOliveGreen = 0x556B2F,
	MediumAquamarine = 0x66CDAA,
	DarkSeaGreen = 0x8FBC8F,
	LightSeaGreen = 0x20B2AA,
	DarkCyan = 0x008B8B,
	Teal = 0x008080,

	// ANCHOR CYAN COLORS
	Aqua = 0x00FFFF,
	Cyan = 0x00FFFF,
	LightCyan = 0xE0FFFF,
	PaleTurquoise = 0xAFEEEE,
	Aquamarine = 0x7FFFD4,
	Turquoise = 0x40E0D0,
	MediumTurquoise = 0x48D1CC,
	DarkTurquoise = 0x00CED1,

	// ANCHOR BLUE COLORS
	CadetBlue = 0x5F9EA0,
	SteelBlue = 0x4682B4,
	LightSteelBlue = 0xB0C4DE,
	LightBlue = 0xADD8E6,
	PowderBlue = 0xB0E0E6,
	LightSkyBlue = 0x87CEFA,
	SkyBlue = 0x87CEEB,
	CornflowerBlue = 0x6495ED,
	DeepSkyBlue = 0x00BFFF,
	DodgerBlue = 0x1E90FF,
	RoyalBlue = 0x4169E1,
	Blue = 0x0000FF,
	MediumBlue = 0x0000CD,
	DarkBlue = 0x00008B,
	Navy = 0x000080,
	MidnightBlue = 0x191970,

	// ANCHOR BROWN COLORS
	Cornsilk = 0xFFF8DC,
	BlanchedAlmond = 0xFFEBCD,
	Bisque = 0xFFE4C4,
	NavajoWhite = 0xFFDEAD,
	Wheat = 0xF5DEB3,
	BurlyWood = 0xDEB887,
	Tan = 0xD2B48C,
	RosyBrown = 0xBC8F8F,
	SandyBrown = 0xF4A460,
	Goldenrod = 0xDAA520,
	DarkGoldenrod = 0xB8860B,
	Peru = 0xCD853F,
	Chocolate = 0xD2691E,
	Olive = 0x808000,
	SaddleBrown = 0x8B4513,
	Sienna = 0xA0522D,
	Brown = 0xA52A2A,
	Maroon = 0x800000,

	// ANCHOR WHITE COLORS
	White = 0xFFFFFF,
	Snow = 0xFFFAFA,
	Honeydew = 0xF0FFF0,
	MintCream = 0xF5FFFA,
	Azure = 0xF0FFFF,
	AliceBlue = 0xF0F8FF,
	GhostWhite = 0xF8F8FF,
	WhiteSmoke = 0xF5F5F5,
	Seashell = 0xFFF5EE,
	Beige = 0xF5F5DC,
	OldLace = 0xFDF5E6,
	FloralWhite = 0xFFFAF0,
	Ivory = 0xFFFFF0,
	AntiqueWhite = 0xFAEBD7,
	Linen = 0xFAF0E6,
	LavenderBlush = 0xFFF0F5,
	MistyRose = 0xFFE4E1,

	// ANCHOR GREY COLORS
	Gainsboro = 0xDCDCDC,
	LightGray = 0xD3D3D3,
	Silver = 0xC0C0C0,
	DarkGray = 0xA9A9A9,
	DimGray = 0x696969,
	Gray = 0x808080,
	LightSlateGray = 0x778899,
	SlateGray = 0x708090,
	DarkSlateGray = 0x2F4F4F,
	Black = 0x000000
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

	Mt_RGBA &hsl(float h, float s, float l);
	Mt_RGBA &hsv(float h, float s, float v);
	Mt_RGBA &rgb(int r, int g, int b);

	Mt_RGBA &rgba(int r, int g, int b, int a);

	Mt_RGBA &hex(int value);
	Mt_RGBA &bw(int value);
	Mt_RGBA &negative();
	Mt_RGBA &opaque();
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
