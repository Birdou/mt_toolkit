
#include "widgets/mt_caret.hpp"

#include "mt_util.hpp"

TOOLKIT_NAMESPACE::Caret::Caret(Widget &widget) : Widget(widget)
{
	geometry->destR.w = geometry->srcR.w = geometry->getW();
	geometry->destR.h = geometry->srcR.h = geometry->getH();
}
void TOOLKIT_NAMESPACE::Caret::init()
{
}
TOOLKIT_NAMESPACE::Caret &TOOLKIT_NAMESPACE::Caret::create(Widget &widget)
{
	TOOLKIT_NAMESPACE::Caret *caret = new TOOLKIT_NAMESPACE::Caret(widget);
	// widget.window.widgets.emplace_back(caret);
	return *caret;
}

TOOLKIT_NAMESPACE::Caret::~Caret()
{
	Debug("Destroying caret...");
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
}

void TOOLKIT_NAMESPACE::Caret::setColorMod()
{
	SDL_SetTextureColorMod(texture, font->color.r, font->color.g, font->color.b);
	renderedColor = font->color;
}

void TOOLKIT_NAMESPACE::Caret::update()
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

void TOOLKIT_NAMESPACE::Caret::draw()
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
