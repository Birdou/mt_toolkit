
#include "mt_label.hpp"

Mt_label::Mt_label(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	wrap = true;
	init();
}
Mt_label::Mt_label(Mt_window &window, int x, int y) : Mt_widget(window, x, y)
{
	init();
}
Mt_label::Mt_label(Mt_widget &widget) : Mt_widget(widget)
{
	init();
}

Mt_label::Mt_label(const Mt_label &) = delete;

void Mt_label::init()
{
	backgroundColor = Mt_RGBA(0, 0, 0, 0);
	borderColor = Mt_RGBA(0, 0, 0, 0);
}
Mt_label &Mt_label::create(Mt_window &window, int x, int y, int w, int h)
{
	return *(new Mt_label(window, x, y, w, h));
}
Mt_label &Mt_label::create(Mt_window &window, int x, int y)
{
	return *(new Mt_label(window, x, y));
}
Mt_label &Mt_label::create(Mt_widget &widget)
{
	return *(new Mt_label(widget));
}
Mt_label::~Mt_label()
{
	Debug("Destroying label");

	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
}

void Mt_label::handleEvent()
{
	HANDLE_WINDOW_EVENTS;
}

void Mt_label::update()
{
	return_if(!visible || text.empty());

	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);

	if (wrap)
	{
		texture = window.renderer->renderWrapped(text, font, geometry, geometry->destR.w, TTF_RenderUTF8_Blended_Wrapped);
	}
	else
	{
		texture = window.renderer->renderText(text, font, geometry, TTF_RenderUTF8_Blended);
	}

	if (autoupdate)
		geometry->normalize();
}

void Mt_label::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	if (texture != nullptr)
		window.renderer->drawTexture(texture, &geometry->srcR, &geometry->destR);
}
