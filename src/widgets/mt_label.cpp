
#include "widgets/mt_label.hpp"

TOOLKIT_NAMESPACE::Label::Label(Window &window, int x, int y, int w, int h) : Widget(window, x, y, w, h)
{
	wrap = true;
	init();
}
TOOLKIT_NAMESPACE::Label::Label(Window &window, int x, int y) : Widget(window, x, y)
{
	init();
}
TOOLKIT_NAMESPACE::Label::Label(Widget &widget) : Widget(widget)
{
	init();
}

void TOOLKIT_NAMESPACE::Label::init()
{
	setScheme(UI_LABEL_COLOR_SCHEME);
}
TOOLKIT_NAMESPACE::Label &TOOLKIT_NAMESPACE::Label::create(Window &window, int x, int y, int w, int h)
{
	TOOLKIT_NAMESPACE::Label *label = new TOOLKIT_NAMESPACE::Label(window, x, y, w, h);
	window.add(*label);
	return *label;
}
TOOLKIT_NAMESPACE::Label &TOOLKIT_NAMESPACE::Label::create(Window &window, int x, int y)
{
	TOOLKIT_NAMESPACE::Label *label = new TOOLKIT_NAMESPACE::Label(window, x, y);
	window.add(*label);
	return *label;
}
TOOLKIT_NAMESPACE::Label &TOOLKIT_NAMESPACE::Label::create(Widget &widget)
{
	return *new TOOLKIT_NAMESPACE::Label(widget);
}
TOOLKIT_NAMESPACE::Label::~Label()
{
	Debug("Destroying label");

	if (textTexture != nullptr)
		SDL_DestroyTexture(textTexture);
	SDL_PrintIfError(Warn);
	if (iconTexture != nullptr)
		SDL_DestroyTexture(iconTexture);
	SDL_PrintIfError(Warn);
}

void TOOLKIT_NAMESPACE::Label::loadIcon(const std::string &path)
{
	SDL_Surface *surf = IMG_Load(path.c_str());
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
void TOOLKIT_NAMESPACE::Label::setColorMod()
{
	if (textTexture != nullptr)
		SDL_SetTextureColorMod(textTexture, font->color.r, font->color.g, font->color.b);
	if (iconTexture != nullptr)
		SDL_SetTextureColorMod(iconTexture, font->color.r, font->color.g, font->color.b);
	renderedColor = font->color;
}
void TOOLKIT_NAMESPACE::Label::handleEvent()
{
	HANDLE_WINDOW_EVENTS;
}

void TOOLKIT_NAMESPACE::Label::update()
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

void TOOLKIT_NAMESPACE::Label::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	if (iconTexture != nullptr)
		window.renderer->drawTexture(iconTexture, &geometry->srcR, &geometry->destR);
	if (textTexture != nullptr)
		window.renderer->drawTexture(textTexture, &geometry->srcR, &geometry->destR);
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}
