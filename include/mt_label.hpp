#ifndef DAB8A644_4736_40F5_A341_651834B4D009
#define DAB8A644_4736_40F5_A341_651834B4D009

#include "mt_application.hpp"
#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"
#include "mt_font.hpp"

class Mt_label;

using RenderMethod = std::function<SDL_Texture *(Mt_label *)>;

class Mt_label : public Mt_widget
{
private:
	SDL_Texture *texture = nullptr;

public:
	std::string text;
	bool autoupdate = true;

	RenderMethod renderMethod = [](Mt_label *label)
	{
		return Mt_lib::renderText(label->application.renderer, label->text, label->font, label->geometry, TTF_RenderUTF8_Blended);
	};

	Mt_label(Mt_widget &widget) : Mt_widget(widget)
	{
	}
	Mt_label(Mt_application &application, int x, int y) : Mt_widget(application, x, y)
	{
	}
	~Mt_label()
	{
		SDL_DestroyTexture(texture);
	}

	void update() override
	{
		return_if(!visible);

		if (texture != nullptr)
			SDL_DestroyTexture(texture);

		texture = renderMethod(this);

		if (autoupdate)
		{
			geometry->normalize();
		}
	}

	void draw() override
	{
		return_if(!visible);

		if (texture != nullptr)
			Mt_lib::drawTexture(application.renderer, texture, &geometry->srcR, &geometry->destR);
	}
};

#endif /* DAB8A644_4736_40F5_A341_651834B4D009 */
