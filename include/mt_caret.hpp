#ifndef AE945488_D359_47A3_81D7_5EC2E140A8A2
#define AE945488_D359_47A3_81D7_5EC2E140A8A2

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_lib.hpp"
#include "mt_font.hpp"

class Mt_caret : public Mt_widget
{
private:
	SDL_Texture *texture = nullptr;

	const unsigned short blinkInterval = 500;
	Uint32 lastBlink = 0;

	bool show = true;

	Mt_caret(Mt_widget &widget) : Mt_widget(widget)
	{
		geometry->destR.w = geometry->srcR.w = geometry->getW();
		geometry->destR.h = geometry->srcR.h = geometry->getH();
	}
	Mt_caret(const Mt_caret &) = delete;

public:
	static Mt_caret &create(Mt_widget &widget) { return *(new Mt_caret(widget)); }

	~Mt_caret()
	{
		Debug("Destroying caret");
		SDL_DestroyTexture(texture);
	}

	void update() override
	{
		return_if(!visible);

		if (texture != nullptr)
			SDL_DestroyTexture(texture);
		texture = Mt_lib::renderText(window.renderer, "|", font, geometry, TTF_RenderUTF8_Blended);
	}

	void draw() override
	{
		return_if(!visible);

		if (SDL_GetTicks() - lastBlink > blinkInterval)
		{
			lastBlink = SDL_GetTicks();
			show = !show;
		}
		if (show)
		{
			Mt_lib::drawTexture(window.renderer, texture, &geometry->srcR, &geometry->destR);
		}
	}
};

#endif /* AE945488_D359_47A3_81D7_5EC2E140A8A2 */
