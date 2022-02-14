
#include "mt_caret.hpp"

#include "mt_lib.hpp"

Mt_caret::Mt_caret(Mt_widget &widget) : Mt_widget(widget)
{
	geometry->destR.w = geometry->srcR.w = geometry->getW();
	geometry->destR.h = geometry->srcR.h = geometry->getH();
}
Mt_caret::Mt_caret(const Mt_caret &) = delete;
Mt_caret &Mt_caret::create(Mt_widget &widget)
{
	return *(new Mt_caret(widget));
}

Mt_caret::~Mt_caret()
{
	Debug("Destroying caret...");
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
}

void Mt_caret::update()
{
	return_if(!visible);

	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
	texture = window.renderer->renderText("|", font, geometry, TTF_RenderUTF8_Blended);
}

void Mt_caret::draw()
{
	return_if(!visible);

	if (SDL_GetTicks() - lastBlink > blinkInterval)
	{
		lastBlink = SDL_GetTicks();
		show = !show;
	}
	if (show)
	{
		window.renderer->drawTexture(texture, &geometry->srcR, &geometry->destR);
	}
}
