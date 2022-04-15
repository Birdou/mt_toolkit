
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
	setScheme(UI_LABEL_COLOR_SCHEME);
}
Mt_label& Mt_label::create(Mt_window& window, int x, int y, int w, int h)
{
	Mt_label* label = new Mt_label(window, x, y, w, h);
	window.add(*label);
	return *label;
}
Mt_label& Mt_label::create(Mt_window& window, int x, int y)
{
	Mt_label* label = new Mt_label(window, x, y);
	window.add(*label);
	return *label;
}
Mt_label& Mt_label::create(Mt_widget& widget)
{
	return *new Mt_label(widget);
}
Mt_label::~Mt_label()
{
	Debug("Destroying label");

	if (textTexture != nullptr)
		SDL_DestroyTexture(textTexture);
	SDL_PrintIfError(Warn);
	if (iconTexture != nullptr)
		SDL_DestroyTexture(iconTexture);
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
		iconTexture = window.renderer->createTextureFromSurface(surf);
		geometry->setW(surf->w);
		geometry->setH(surf->h);

		SDL_FreeSurface(surf);

		geometry->normalize();
	}
	setColorMod();
}
void Mt_label::setColorMod()
{
	if (textTexture != nullptr)
		SDL_SetTextureColorMod(textTexture, font->color.r, font->color.g, font->color.b);
	if (iconTexture != nullptr)
		SDL_SetTextureColorMod(iconTexture, font->color.r, font->color.g, font->color.b);
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
		if (textTexture != nullptr)
			SDL_DestroyTexture(textTexture);
		SDL_PrintIfError(Warn);

		if (text.size() > 0)
		{
			if (wrap)
			{
				textTexture = window.renderer->renderWrapped(text, font->getFont(), geometry.get(), geometry->destR.w, TTF_RenderUTF8_Blended_Wrapped);
			}
			else
			{
				textTexture = window.renderer->renderText(text, font->getFont(), geometry.get(), TTF_RenderUTF8_Blended);
				if (autoupdate)
					geometry->normalize();
			}
			setColorMod();
		}
		else
		{
			textTexture = nullptr;
		}
		renderedText = text;
		renderedFont = font->getFont();
	}
	if (font->color != renderedColor)
		setColorMod();
}

void Mt_label::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	if (iconTexture != nullptr)
		window.renderer->drawTexture(iconTexture, &geometry->srcR, &geometry->destR);
	if (textTexture != nullptr)
		window.renderer->drawTexture(textTexture, &geometry->srcR, &geometry->destR);
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}
