
#include "mt_font.hpp"

Mt_font::Mt_font(Mt_application &application) : application(application)
{
}
Mt_font::Mt_font(Mt_application &application, const std::string &path, unsigned short fontSize) : application(application)
{
	setFont(path, fontSize);
}
Mt_font::~Mt_font()
{
	Debug("Destroying font...");
}

void Mt_font::setFont(const std::string &path, unsigned short size)
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

TTF_Font *Mt_font::getFont()
{
	return font;
}

unsigned short Mt_font::getSize() const
{
	return fontSize;
}

int Mt_font::getH() const
{
	return maxHeight;
}