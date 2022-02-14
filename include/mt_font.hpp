#ifndef B72EFF8C_D70C_44BC_BB4A_88698734A49A
#define B72EFF8C_D70C_44BC_BB4A_88698734A49A

#include "mt_application.hpp"

#include "mt_color.hpp"

class Mt_font
{
private:
	Mt_application &application;

	TTF_Font *font = nullptr;
	unsigned short fontSize = 0;
	int maxHeight = 0;

public:
	Mt_color color;

	Mt_font(Mt_application &application);
	Mt_font(Mt_application &application, const std::string &path, unsigned short fontSize);
	~Mt_font();

	void setFont(const std::string &path, unsigned short size);

	TTF_Font *getFont();

	unsigned short getSize() const;

	int getH() const;
};

#endif /* B72EFF8C_D70C_44BC_BB4A_88698734A49A */
