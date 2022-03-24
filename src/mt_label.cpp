
#include "mt_label.hpp"

Mt_label::Mt_label(Mt_window& window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	wrap = true;
	init();
}
Mt_label::Mt_label(Mt_window& window, int x, int y) : Mt_widget(window, x, y)
{
	init();
}
Mt_label::Mt_label(Mt_widget& widget) : Mt_widget(widget)
{
	init();
}

void Mt_label::init()
{
	backgroundColor = Mt_RGBA(0, 0, 0, 0);
	borderColor = Mt_RGBA(0, 0, 0, 0);
}
Mt_label& Mt_label::create(Mt_window& window, int x, int y, int w, int h)
{
	Mt_label* label = new Mt_label(window, x, y, w, h);
	window.widgets.emplace_back(label);
	return *label;
}
Mt_label& Mt_label::create(Mt_window& window, int x, int y)
{
	Mt_label* label = new Mt_label(window, x, y);
	window.widgets.emplace_back(label);
	return *label;
}
Mt_label& Mt_label::create(Mt_widget& widget)
{
	Mt_label* label = new Mt_label(widget);
	// widget.window.widgets.emplace_back(label);
	return *label;
}
Mt_label::~Mt_label()
{
	Debug("Destroying label");

	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
}

void Mt_label::loadIcon(const std::string& path)
{
	SDL_Surface* surf = IMG_Load(path.c_str());
	if (surf == nullptr)
	{
		SDL_PrintError(Error);
	}
	else
	{
		texture = window.renderer->createTextureFromSurface(surf);
		geometry->setW(surf->w);
		geometry->setH(surf->h);

		SDL_FreeSurface(surf);

		geometry->normalize();
	}
	setColorMod();
}
void Mt_label::setColorMod()
{
	SDL_SetTextureColorMod(texture, font->color.r, font->color.g, font->color.b);
	renderedColor = font->color;
}
void Mt_label::handleEvent()
{
	HANDLE_WINDOW_EVENTS;
}

void Mt_label::update()
{
	return_if(!visible);

	if (text != renderedText || font->getFont() != renderedFont)
	{
		if (texture != nullptr)
			SDL_DestroyTexture(texture);
		SDL_PrintIfError(Warn);

		if (text.size() > 0)
		{
			if (wrap)
				texture = window.renderer->renderWrapped(text, font->getFont(), geometry.get(), geometry->destR.w, TTF_RenderUTF8_Blended_Wrapped);
			else
				texture = window.renderer->renderText(text, font->getFont(), geometry.get(), TTF_RenderUTF8_Blended);

			setColorMod();
		}
		else
		{
			texture = nullptr;
		}
		renderedText = text;
		renderedFont = font->getFont();
	}
	if (font->color != renderedColor)
		setColorMod();

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
