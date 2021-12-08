#ifndef DAB8A644_4736_40F5_A341_651834B4D009
#define DAB8A644_4736_40F5_A341_651834B4D009

#include "mt_application.hpp"
#include "mt_window.hpp"
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

	Mt_label(Mt_widget &widget) : Mt_widget(widget) {}
	Mt_label(Mt_window &window, int x, int y) : Mt_widget(window, x, y) {}
	Mt_label(const Mt_label &) = delete;

public:
	static Mt_label &create(Mt_window &window, int x, int y) { return *(new Mt_label(window, x, y)); }
	static Mt_label &create(Mt_widget &widget) { return *(new Mt_label(widget)); }

	~Mt_label()
	{
		Debug("Destroying label");
		SDL_DestroyTexture(texture);
	}

	std::string text;
	bool autoupdate = true;

	RenderMethod renderMethod = [](Mt_label *label)
	{
		return Mt_lib::renderText(label->window.renderer, label->text, label->font, label->geometry, TTF_RenderUTF8_Blended);
	};

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
			Mt_lib::drawTexture(window.renderer, texture, &geometry->srcR, &geometry->destR);
	}
};

#endif /* DAB8A644_4736_40F5_A341_651834B4D009 */
