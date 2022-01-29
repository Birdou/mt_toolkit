#ifndef F485EFCF_8079_4F33_91F7_A134EA3822B3
#define F485EFCF_8079_4F33_91F7_A134EA3822B3

#include "mt_application.hpp"
#include "mt_widget.hpp"
#include <algorithm>
#include <cmath>

class Mt_rgb
{
private:
	int r = 0;
	int g = 0;
	int b = 0;

public:
	Mt_rgb() {}
	Mt_rgb(int r, int g, int b)
	{
		rgb(r, g, b);
	}
	Mt_rgb(const Mt_rgb &color)
	{
		*this = color;
	}
	~Mt_rgb() {}

	Mt_rgb operator+(double num) const;
	Mt_rgb operator-(double num) const;
	Mt_rgb operator*(double num) const;
	Mt_rgb operator/(double num) const;

	Mt_rgb operator+(const Mt_rgb &color) const;
	Mt_rgb operator-(const Mt_rgb &color) const;
	Mt_rgb operator*(const Mt_rgb &color) const;
	Mt_rgb operator/(const Mt_rgb &color) const;

	Mt_rgb &operator=(const Mt_rgb &color)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;

		return *this;
	}
	Mt_rgb &operator=(const SDL_Color &color)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;

		return *this;
	}

	bool operator==(const Mt_rgb &color) const
	{
		if (this->r == color.r && this->g == color.g && this->b == color.b)
			return true;
		return false;
	}
	bool operator!=(const Mt_rgb &color) const
	{
		if (this->r == color.r && this->g == color.g && this->b == color.b)
			return false;
		return true;
	}

	bool operator==(const SDL_Color &color) const;
	bool operator!=(const SDL_Color &color) const;

	int getR() const { return this->r; }
	void setR(int r)
	{
		r = std::min(r, 255);
		r = std::max(0, r);
		this->r = r;
	}

	int getG() const { return this->g; }
	void setG(int g)
	{
		g = std::min(g, 255);
		g = std::max(0, g);
		this->g = g;
	}

	int getB() const { return this->b; }
	void setB(int b)
	{
		b = std::min(b, 255);
		b = std::max(0, b);
		this->b = b;
	}

	void rgb(int r, int g, int b)
	{
		setR(r);
		setG(g);
		setB(b);
	}

	void negative()
	{
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	}

	void hsl(float h, float s, float l)
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
	void hsv(float h, float s, float v)
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

	enum Colors
	{
		Black,
		grey11,
		grey21,
		grey31,
		DimGray,
		Gray,
		DarkGray,
		Silver,
		LightGrey,
		Gainsboro,
		White,
		SlateBlue,
		SlateBlue1,
		SlateBlue3,
		DarkSlateBlue,
		MidnightBlue,
		Navy,
		DarkBlue,
		MediumBlue,
		Blue,
		refnflowerBlue,
		RoyalBlue,
		DodgerBlue,
		DeepSkyBlue,
		LightSkyBlue,
		SkyBlue,
		LightBlue,
		SteelBlue,
		LightSteelBlue,
		SlateGray,
		LightSlateGray,
		Aqua,
		Cyan,
		DarkTurquoise,
		Turquoise,
		MediumTurquoise,
		LightSeaGreen,
		DarkCyan,
		Teal,
		Aquamarine,
		MediumAquamarine,
		CadetBlue,
		DarkSlateGray,
		MediumSpringGreen,
		SpringGreen,
		PaleGreen,
		LightGreen,
		DarkSeaGreen,
		MediumSeaGreen,
		SeaGreen,
		DarkGreen,
		Green,
		ForestGreen,
		LimeGreen,
		Lime,
		LawnGreen,
		Chartreuse,
		GreenYellow,
		YellowGreen,
		OliveDrab,
		DarkOliveGreen,
		Olive,
		DarkKhaki,
		Goldenrod,
		DarkGoldenrod,
		SaddleBrown,
		Sienna,
		RosyBrown,
		Peru,
		Chocolate,
		SandyBrown,
		NavajoWhite,
		Wheat,
		BurlyWood,
		Tan,
		MediumSlateBlue,
		MediumPurple,
		BlueViolet,
		Indigo,
		DarkViolet,
		DarkOrchid,
		MediumOrchid,
		Purple,
		DarkMagenta,
		Fuchsia,
		Magenta,
		Violet,
		Orchid,
		Plum,
		MediumVioletRed,
		DeepPink,
		HotPink,
		PaleVioletRed,
		LightPink,
		Pink,
		Lightrefal,
		IndianRed,
		Crimson,
		Maroon,
		DarkRed,
		FireBrick,
		Brown,
		Salmon,
		DarkSalmon,
		LightSalmon,
		refal,
		Tomato,
		Red,
		OrangeRed,
		DarkOrange,
		Orange,
		Gold,
		Yellow,
		Khaki,
		AliceBlue,
		GhostWhite,
		Snow,
		Seashell,
		FloralWhite,
		WhiteSmoke,
		Beige,
		OldLace,
		Ivory,
		Linen,
		refnsilk,
		AntiqueWhite,
		BlanchedAlmond,
		Bisque,
		LightYellow,
		LemonChiffon,
		LightGoldenrodYellow,
		PapayaWhip,
		PeachPuff,
		Moccasin,
		PaleGoldenrod,
		MistyRose,
		LavenderBlush,
		Lavender,
		Thistle,
		Azure,
		LightCyan,
		PowderBlue,
		PaleTurquoise,
		Honeydew,
		MintCream,
		end
	};

	Mt_rgb getrgbval(Colors color)
	{
		switch (color)
		{
		case Black:
			return Mt_rgb(0, 0, 0);
		case grey11:
			return Mt_rgb(28, 28, 28);
		case grey21:
			return Mt_rgb(54, 54, 54);
		case grey31:
			return Mt_rgb(79, 79, 79);
		case DimGray:
			return Mt_rgb(105, 105, 105);
		case Gray:
			return Mt_rgb(128, 128, 128);
		case DarkGray:
			return Mt_rgb(169, 169, 169);
		case Silver:
			return Mt_rgb(192, 192, 192);
		case LightGrey:
			return Mt_rgb(211, 211, 211);
		case Gainsboro:
			return Mt_rgb(220, 220, 220);
		case White:
			return Mt_rgb(255, 255, 255);
		case SlateBlue:
			return Mt_rgb(106, 90, 205);
		case SlateBlue1:
			return Mt_rgb(131, 111, 255);
		case SlateBlue3:
			return Mt_rgb(105, 89, 205);
		case DarkSlateBlue:
			return Mt_rgb(72, 61, 139);
		case MidnightBlue:
			return Mt_rgb(25, 25, 112);
		case Navy:
			return Mt_rgb(0, 0, 128);
		case DarkBlue:
			return Mt_rgb(0, 0, 139);
		case MediumBlue:
			return Mt_rgb(0, 0, 205);
		case Blue:
			return Mt_rgb(0, 0, 255);
		case refnflowerBlue:
			return Mt_rgb(100, 149, 237);
		case RoyalBlue:
			return Mt_rgb(65, 105, 225);
		case DodgerBlue:
			return Mt_rgb(30, 144, 255);
		case DeepSkyBlue:
			return Mt_rgb(0, 191, 255);
		case LightSkyBlue:
			return Mt_rgb(135, 206, 250);
		case SkyBlue:
			return Mt_rgb(135, 206, 235);
		case LightBlue:
			return Mt_rgb(173, 216, 230);
		case SteelBlue:
			return Mt_rgb(70, 130, 180);
		case LightSteelBlue:
			return Mt_rgb(176, 196, 222);
		case SlateGray:
			return Mt_rgb(112, 128, 144);
		case LightSlateGray:
			return Mt_rgb(119, 136, 153);
		case Aqua:
			return Mt_rgb(0, 255, 255);
		case Cyan:
			return Mt_rgb(0, 255, 255);
		case DarkTurquoise:
			return Mt_rgb(0, 206, 209);
		case Turquoise:
			return Mt_rgb(64, 224, 208);
		case MediumTurquoise:
			return Mt_rgb(72, 209, 204);
		case LightSeaGreen:
			return Mt_rgb(32, 178, 170);
		case DarkCyan:
			return Mt_rgb(0, 139, 139);
		case Teal:
			return Mt_rgb(0, 128, 128);
		case Aquamarine:
			return Mt_rgb(127, 255, 212);
		case MediumAquamarine:
			return Mt_rgb(102, 205, 170);
		case CadetBlue:
			return Mt_rgb(95, 158, 160);
		case DarkSlateGray:
			return Mt_rgb(47, 79, 79);
		case MediumSpringGreen:
			return Mt_rgb(0, 250, 154);
		case SpringGreen:
			return Mt_rgb(0, 255, 127);
		case PaleGreen:
			return Mt_rgb(152, 251, 152);
		case LightGreen:
			return Mt_rgb(144, 238, 144);
		case DarkSeaGreen:
			return Mt_rgb(143, 188, 143);
		case MediumSeaGreen:
			return Mt_rgb(60, 179, 113);
		case SeaGreen:
			return Mt_rgb(46, 139, 87);
		case DarkGreen:
			return Mt_rgb(0, 100, 0);
		case Green:
			return Mt_rgb(0, 128, 0);
		case ForestGreen:
			return Mt_rgb(34, 139, 34);
		case LimeGreen:
			return Mt_rgb(50, 205, 50);
		case Lime:
			return Mt_rgb(0, 255, 0);
		case LawnGreen:
			return Mt_rgb(124, 252, 0);
		case Chartreuse:
			return Mt_rgb(127, 255, 0);
		case GreenYellow:
			return Mt_rgb(173, 255, 47);
		case YellowGreen:
			return Mt_rgb(154, 205, 50);
		case OliveDrab:
			return Mt_rgb(107, 142, 35);
		case DarkOliveGreen:
			return Mt_rgb(85, 107, 47);
		case Olive:
			return Mt_rgb(128, 128, 0);
		case DarkKhaki:
			return Mt_rgb(189, 83, 107);
		case Goldenrod:
			return Mt_rgb(218, 165, 32);
		case DarkGoldenrod:
			return Mt_rgb(184, 134, 11);
		case SaddleBrown:
			return Mt_rgb(139, 69, 19);
		case Sienna:
			return Mt_rgb(160, 82, 45);
		case RosyBrown:
			return Mt_rgb(188, 143, 143);
		case Peru:
			return Mt_rgb(205, 133, 63);
		case Chocolate:
			return Mt_rgb(210, 105, 30);
		case SandyBrown:
			return Mt_rgb(244, 164, 96);
		case NavajoWhite:
			return Mt_rgb(255, 222, 173);
		case Wheat:
			return Mt_rgb(245, 222, 179);
		case BurlyWood:
			return Mt_rgb(222, 184, 135);
		case Tan:
			return Mt_rgb(210, 180, 140);
		case MediumSlateBlue:
			return Mt_rgb(123, 104, 238);
		case MediumPurple:
			return Mt_rgb(147, 112, 219);
		case BlueViolet:
			return Mt_rgb(138, 43, 226);
		case Indigo:
			return Mt_rgb(75, 0, 130);
		case DarkViolet:
			return Mt_rgb(148, 0, 211);
		case DarkOrchid:
			return Mt_rgb(153, 50, 204);
		case MediumOrchid:
			return Mt_rgb(186, 85, 211);
		case Purple:
			return Mt_rgb(128, 0, 128);
		case DarkMagenta:
			return Mt_rgb(139, 0, 139);
		case Fuchsia:
			return Mt_rgb(255, 0, 255);
		case Magenta:
			return Mt_rgb(255, 0, 255);
		case Violet:
			return Mt_rgb(238, 130, 238);
		case Orchid:
			return Mt_rgb(218, 112, 214);
		case Plum:
			return Mt_rgb(221, 160, 221);
		case MediumVioletRed:
			return Mt_rgb(199, 21, 133);
		case DeepPink:
			return Mt_rgb(255, 20, 147);
		case HotPink:
			return Mt_rgb(255, 105, 180);
		case PaleVioletRed:
			return Mt_rgb(219, 112, 147);
		case LightPink:
			return Mt_rgb(255, 182, 193);
		case Pink:
			return Mt_rgb(255, 192, 203);
		case Lightrefal:
			return Mt_rgb(240, 128, 128);
		case IndianRed:
			return Mt_rgb(205, 92, 92);
		case Crimson:
			return Mt_rgb(220, 20, 60);
		case Maroon:
			return Mt_rgb(128, 0, 0);
		case DarkRed:
			return Mt_rgb(139, 0, 0);
		case FireBrick:
			return Mt_rgb(178, 34, 34);
		case Brown:
			return Mt_rgb(165, 42, 42);
		case Salmon:
			return Mt_rgb(250, 128, 114);
		case DarkSalmon:
			return Mt_rgb(233, 150, 122);
		case LightSalmon:
			return Mt_rgb(255, 160, 122);
		case refal:
			return Mt_rgb(255, 127, 80);
		case Tomato:
			return Mt_rgb(255, 99, 71);
		case Red:
			return Mt_rgb(255, 0, 0);
		case OrangeRed:
			return Mt_rgb(255, 69, 0);
		case DarkOrange:
			return Mt_rgb(255, 140, 0);
		case Orange:
			return Mt_rgb(255, 165, 0);
		case Gold:
			return Mt_rgb(255, 215, 0);
		case Yellow:
			return Mt_rgb(255, 255, 0);
		case Khaki:
			return Mt_rgb(240, 230, 140);
		case AliceBlue:
			return Mt_rgb(240, 248, 255);
		case GhostWhite:
			return Mt_rgb(248, 248, 255);
		case Snow:
			return Mt_rgb(255, 250, 250);
		case Seashell:
			return Mt_rgb(255, 245, 238);
		case FloralWhite:
			return Mt_rgb(255, 250, 240);
		case WhiteSmoke:
			return Mt_rgb(245, 245, 245);
		case Beige:
			return Mt_rgb(245, 245, 220);
		case OldLace:
			return Mt_rgb(253, 245, 230);
		case Ivory:
			return Mt_rgb(255, 255, 240);
		case Linen:
			return Mt_rgb(250, 240, 230);
		case refnsilk:
			return Mt_rgb(255, 248, 220);
		case AntiqueWhite:
			return Mt_rgb(250, 235, 215);
		case BlanchedAlmond:
			return Mt_rgb(255, 235, 205);
		case Bisque:
			return Mt_rgb(255, 228, 196);
		case LightYellow:
			return Mt_rgb(255, 255, 224);
		case LemonChiffon:
			return Mt_rgb(255, 250, 205);
		case LightGoldenrodYellow:
			return Mt_rgb(250, 250, 210);
		case PapayaWhip:
			return Mt_rgb(255, 239, 213);
		case PeachPuff:
			return Mt_rgb(255, 218, 185);
		case Moccasin:
			return Mt_rgb(255, 228, 181);
		case PaleGoldenrod:
			return Mt_rgb(238, 232, 170);
		case MistyRose:
			return Mt_rgb(255, 228, 225);
		case LavenderBlush:
			return Mt_rgb(255, 240, 245);
		case Lavender:
			return Mt_rgb(230, 230, 250);
		case Thistle:
			return Mt_rgb(216, 191, 216);
		case Azure:
			return Mt_rgb(240, 255, 255);
		case LightCyan:
			return Mt_rgb(224, 255, 255);
		case PowderBlue:
			return Mt_rgb(176, 224, 230);
		case PaleTurquoise:
			return Mt_rgb(175, 238, 238);
		case Honeydew:
			return Mt_rgb(240, 255, 240);
		case MintCream:
			return Mt_rgb(245, 255, 250);

		default:
			return Mt_rgb(0, 0, 0);
		}
	}
};

#include <fstream>
#include <iomanip>
#include "mt_lib.hpp"

class Mt_pixelgrid
{
protected:
	Mt_rgb **data = nullptr;
	size_t w = 0, h = 0;

	void updateSurface(SDL_Surface *surface)
	{
		unsigned char *pixels = (unsigned char *)surface->pixels;
		for (size_t y = 0; y < h; ++y)
		{
			for (size_t x = 0; x < w; ++x)
			{
				pixels[4 * (y * surface->w + x) + 2] = data[y][x].getR(); // 2103
				pixels[4 * (y * surface->w + x) + 1] = data[y][x].getG(); // 2103
				pixels[4 * (y * surface->w + x) + 0] = data[y][x].getB(); // 2103
				pixels[4 * (y * surface->w + x) + 3] = 255;				  // 2103
			}
		}
	}

	Mt_pixelgrid() {}

public:
	Mt_pixelgrid(size_t w, size_t h)
	{
		alloc(w, h);
	}
	~Mt_pixelgrid()
	{
		erase();
	}
	virtual Mt_rgb &at(size_t x, size_t y) { return data[y][x]; }
	virtual const Mt_rgb &get(size_t x, size_t y) const { return data[y][x]; }

	virtual void alloc(size_t width, size_t height)
	{
		erase();
		data = new Mt_rgb *[height];
		for (size_t i = 0; i < height; ++i)
			data[i] = new Mt_rgb[width];
		w = width;
		h = height;
	}

	virtual void erase()
	{
		if (data == nullptr)
			return;

		for (size_t i = 0; i < h; ++i)
			delete[] data[i];
		delete[] data;

		data = nullptr;
	}

	size_t width() const noexcept { return w; }
	size_t height() const noexcept { return h; }

	void savePPMA(const std::string &file)
	{
		std::ofstream save;
		save.open(file);

		save << "P3\r";
		save << w << " " << h << "\r";
		save << "255\r";

		for (size_t i = 0; i < h; ++i) // o laço superior é relacionado à altura devido a ordem de gravação.
			for (size_t j = 0; j < w; ++j)
				save << data[i][j].getR() << " " << data[i][j].getG() << " " << data[i][j].getB() << "\r";

		save.close();
	}
	void savePPMB(const std::string &file)
	{
		std::ofstream save;
		save.open(file, std::ios_base::binary);

		save << "P6\r";
		save << w << " " << h << "\r";
		save << "255\r";

		for (size_t i = 0; i < h; ++i) // o laço superior é relacionado à altura devido a ordem de gravação.
			for (size_t j = 0; j < w; ++j)
				save << (unsigned char)data[i][j].getR() << (unsigned char)data[i][j].getG() << (unsigned char)data[i][j].getB();

		save.close();
	}
	void saveJPG(const std::string &file)
	{
		SDL_Surface *surface = getSDLSurface();
		IMG_SaveJPG(surface, file.c_str(), 100);
		SDL_FreeSurface(surface);
		if (strcmp(SDL_GetError(), "") != 0)
			std::cout << SDL_GetError() << std::endl;
	}
	void savePNG(const std::string &file)
	{
		SDL_Surface *surface = getSDLSurface();
		IMG_SavePNG(surface, file.c_str());
		SDL_FreeSurface(surface);
		if (strcmp(SDL_GetError(), "") != 0)
			std::cout << SDL_GetError() << std::endl;
	}
	void saveBMP(const std::string &file)
	{
		SDL_Surface *surface = getSDLSurface();
		SDL_SaveBMP(surface, file.c_str());
		SDL_FreeSurface(surface);
		if (strcmp(SDL_GetError(), "") != 0)
			std::cout << SDL_GetError() << std::endl;
	}

	virtual SDL_Surface *getSDLSurface()
	{
		SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
		updateSurface(surface);
		return surface;
	}
};

class Mt_bitmap : public Mt_widget, public Mt_pixelgrid
{
private:
	SDL_Texture *tex = nullptr;
	SDL_Surface *surface = nullptr;
	SDL_Rect imgDestR;

	bool render = false;
	bool pressed = false;

	Mt_bitmap(Mt_window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h) : Mt_widget(window, x, y, w, h)
	{
		alloc(map_w, map_h);
	}
	Mt_bitmap(const Mt_bitmap &) = delete;

public:
	static Mt_bitmap &create(Mt_window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h) { return *(new Mt_bitmap(window, x, y, w, h, map_w, map_h)); }

	~Mt_bitmap()
	{
		Debug("Destroying bitmap...");
		erase();
	}

	Mt_rgb &at(size_t x, size_t y) override
	{
		render = true;
		return data[y][x];
	}

	void alloc(size_t width, size_t height) override
	{
		erase();
		geometry->srcR.w = w = width;
		geometry->srcR.h = h = height;

		surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
		data = new Mt_rgb *[height];
		for (size_t i = 0; i < height; ++i)
			data[i] = new Mt_rgb[width];
	}

	void erase() override
	{
		SDL_FreeSurface(surface);
		if (tex != NULL)
			SDL_DestroyTexture(tex);

		geometry->srcR.w = w = 0;
		geometry->srcR.h = h = 0;
	}

	SDL_Surface *getSDLSurface() override
	{
		return surface;
	}

	void handleEvent() override
	{
		HANDLE_WINDOW_EVENTS;

		return_if(!visible);
	}

	void update() override
	{
		return_if(!visible);

		if (Mt_vector<int>::mousePos().intercept(geometry->destR))
		{
			if (window.hovering == nullptr)
			{
				window.hovering = this;
			}
			if (window.hovering == this)
			{
				onHover();
				if (pressed)
				{
					onClicked();

					if (window.event.type == SDL_MOUSEBUTTONUP && window.event.button.button == SDL_BUTTON_LEFT)
					{
						onMouseUp();

						pressed = false;
					}
				}
				else
				{
					if (window.event.type == SDL_MOUSEBUTTONDOWN && window.event.button.button == SDL_BUTTON_LEFT)
					{
						onMouseDown();
						onClicked();

						pressed = true;
					}
				}
			}
		}
		else
		{
			if (window.hovering == this)
			{
				pressed = false;
				window.hovering = nullptr;
			}
		}

		if (render)
		{
			if (tex != NULL)
				SDL_DestroyTexture(tex);

			updateSurface(surface);
			tex = SDL_CreateTextureFromSurface(window.renderer, surface);

			double ri = w / (double)h;
			double rg = geometry->destR.w / (double)geometry->destR.h;
			if (ri == 1. && rg == 1.)
			{
				imgDestR.w = geometry->destR.w;
				imgDestR.h = geometry->destR.h;
				imgDestR.x = geometry->destR.x;
				imgDestR.y = geometry->destR.y;
			}
			else if (ri > 1. && ri > rg)
			{
				imgDestR.w = geometry->destR.w;
				imgDestR.h = geometry->destR.w / ri;
				imgDestR.x = geometry->destR.x;
				imgDestR.y = geometry->destR.y + (geometry->destR.h - imgDestR.h) / 2;
			}
			else if (ri < 1. && ri < rg)
			{
				imgDestR.w = geometry->destR.h * ri;
				imgDestR.h = geometry->destR.h;
				imgDestR.x = geometry->destR.x + (geometry->destR.w - imgDestR.w) / 2;
				imgDestR.y = geometry->destR.y;
			}

			render = false;
		}
	}

	void draw() override
	{
		return_if(!visible);

		Mt_lib::drawFillRectangle(window.renderer, geometry->destR, backgroundColor.color);
		Mt_lib::drawTexture(window.renderer, tex, &geometry->srcR, &imgDestR);
	}
};

#endif /* F485EFCF_8079_4F33_91F7_A134EA3822B3 */
