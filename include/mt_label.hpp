#ifndef DAB8A644_4736_40F5_A341_651834B4D009
#define DAB8A644_4736_40F5_A341_651834B4D009

#include "mt_application.hpp"
#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"
#include "mt_font.hpp"

class Mt_label : public Mt_widget
{
private:
	SDL_Texture *texture = nullptr;

	std::string prev;

public:
	std::string text;
	bool autoupdate = true;

	Mt_label(Mt_application &application, int x, int y) : Mt_widget(application, x, y), text("")
	{
	}
	~Mt_label()
	{
		SDL_DestroyTexture(texture);
	}

	void update() override
	{
		if (text != prev)
		{
			if (texture != nullptr)
				SDL_DestroyTexture(texture);

			texture = Mt_lib::renderText(application.renderer, text, font.getFont(), geometry.srcR, TTF_RenderUTF8_Blended);
			geometry.setW(geometry.srcR.w);
			geometry.setH(geometry.srcR.h);

			if (autoupdate)
			{
				geometry.posCenter();
				geometry.srcR.x = geometry.srcR.y = 0;
				geometry.destR.w = geometry.srcR.w;
				geometry.destR.h = geometry.srcR.h;
			}
			prev = text;
		}
	}

	void draw() override
	{
		if (texture != nullptr && visible)
			Mt_lib::drawTexture(application.renderer, texture, &geometry.srcR, &geometry.destR);
	}
};

#endif /* DAB8A644_4736_40F5_A341_651834B4D009 */
