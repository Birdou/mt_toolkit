#ifndef B72EFF8C_D70C_44BC_BB4A_88698734A49A
#define B72EFF8C_D70C_44BC_BB4A_88698734A49A

#include "mt_application.hpp"

class Mt_font
{
private:
	Mt_application &application;

	TTF_Font *font = nullptr;
	unsigned short fontSize = 0;
	int maxHeight = 0;

public:
	SDL_Color color = {0, 0, 0, 255};

	Mt_font(Mt_application &application) : application(application)
	{
	}
	Mt_font(Mt_application &application, const std::string &path, unsigned short fontSize) : application(application)
	{
		setFont(path, fontSize);
	}
	~Mt_font()
	{
		Debug("Destroying font");

		// Debug("Done.");
	}

	void setFont(const std::string &path, unsigned short size)
	{
		fontSize = size;
		font = application.getFont(path, size);
		if (!font)
		{
			Warn("Invalid font");
		}
		else
		{
			TTF_SizeUTF8(font, "0", nullptr, &maxHeight);
		}
	}

	TTF_Font *getFont()
	{
		return font;
	}

	unsigned short getSize() const
	{
		return fontSize;
	}

	int getH() const
	{
		return maxHeight;
	}
};

#endif /* B72EFF8C_D70C_44BC_BB4A_88698734A49A */
