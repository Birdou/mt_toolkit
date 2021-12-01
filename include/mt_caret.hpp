#ifndef AE945488_D359_47A3_81D7_5EC2E140A8A2
#define AE945488_D359_47A3_81D7_5EC2E140A8A2

#include "mt_application.hpp"
#include "mt_lib.hpp"
#include "mt_font.hpp"

class Mt_caret
{
private:
	Mt_application &application;

	SDL_Texture *texture = nullptr;

	const unsigned short blinkInterval = 500;
	Uint32 lastBlink = 0;

	bool show = true;

	int width, height;

public:
	SDL_Rect destR = {0, 0, 0, 0};
	SDL_Rect srcR = {0, 0, 0, 0};

	Mt_font font;

	bool visible = true;

	Mt_caret(Mt_application &application, int fontSize) : application(application), font(application)
	{
		font.setFont("assets/fonts/segoeui.ttf", fontSize);
		texture = Mt_lib::renderText(application.renderer, "|", font.getFont(), srcR, TTF_RenderUTF8_Blended);

		width = destR.w = srcR.w;
		height = destR.h = srcR.h;
	}
	~Mt_caret()
	{
		SDL_DestroyTexture(texture);
	}

	int getHeight() const
	{
		return height;
	}
	int getWidth() const
	{
		return width;
	}

	void draw()
	{
		if (visible)
		{
			if (SDL_GetTicks() - lastBlink > blinkInterval)
			{
				lastBlink = SDL_GetTicks();
				show = !show;
			}
			if (show)
			{
				Mt_lib::drawTexture(application.renderer, texture, &srcR, &destR);
			}
		}
	}
};

#endif /* AE945488_D359_47A3_81D7_5EC2E140A8A2 */
