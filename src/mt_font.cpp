
#include "mt_font.hpp"

TOOLKIT_NAMESPACE::Font::Font(Application &application) : application(application)
{
}
TOOLKIT_NAMESPACE::Font::Font(Application &application, const std::string &path, unsigned short fontSize) : application(application)
{
	setFont(path, fontSize);
}
TOOLKIT_NAMESPACE::Font::~Font()
{
	// Debug("Destroying font...");
}

void TOOLKIT_NAMESPACE::Font::setFont(const std::string &path, unsigned short size)
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

TTF_Font *TOOLKIT_NAMESPACE::Font::getFont()
{
	return font;
}

unsigned short TOOLKIT_NAMESPACE::Font::getSize() const
{
	return fontSize;
}

int TOOLKIT_NAMESPACE::Font::getH() const
{
	return maxHeight;
}