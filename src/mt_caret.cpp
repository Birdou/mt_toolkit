
#include "mt_caret.hpp"

#include "mt_lib.hpp"

Mt_caret::Mt_caret(Mt_widget& widget) : Mt_widget(widget)
{
	geometry->destR.w = geometry->srcR.w = geometry->getW();
	geometry->destR.h = geometry->srcR.h = geometry->getH();
}
void Mt_caret::init()
{}
Mt_caret& Mt_caret::create(Mt_widget& widget)
{
	Mt_caret* caret = new Mt_caret(widget);
	// widget.window.widgets.emplace_back(caret);
	return *caret;
}

Mt_caret::~Mt_caret()
{
	Debug("Destroying caret...");
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
}

void Mt_caret::setColorMod()
{
	SDL_SetTextureColorMod(texture, font->color.r, font->color.g, font->color.b);
	renderedColor = font->color;
}

void Mt_caret::update()
{
	return_if(!visible);

	if (font->getFont() != renderedFont)
	{
		if (texture != nullptr)
			SDL_DestroyTexture(texture);
		SDL_PrintIfError(Warn);

		texture = window.renderer->renderText("|", font->getFont(), geometry.get(), TTF_RenderUTF8_Blended);

		setColorMod();
		renderedFont = font->getFont();
	}
	if (font->color != renderedColor)
	{
		setColorMod();
	}
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
