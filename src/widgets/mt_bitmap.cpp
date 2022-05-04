
#include "widgets/mt_bitmap.hpp"

#include "mt_point.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Bitmap::counter;

TOOLKIT_NAMESPACE::Bitmap::Bitmap(Window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h) : Widget(window, getClassId(), x, y, w, h)
{

	alloc(map_w, map_h);
}
TOOLKIT_NAMESPACE::Bitmap::Bitmap(Window &window, int x, int y, size_t w, size_t h) : Widget(window, getClassId(), x, y, w, h)
{

	alloc(w, h);
}

void TOOLKIT_NAMESPACE::Bitmap::init()
{
	currentBackgroundColor = scheme.background.normal;
	currentBorderColor = scheme.border.normal;
}

TOOLKIT_NAMESPACE::Bitmap &TOOLKIT_NAMESPACE::Bitmap::create(Window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h)
{
	TOOLKIT_NAMESPACE::Bitmap *bitmap = new TOOLKIT_NAMESPACE::Bitmap(window, x, y, w, h, map_w, map_h);
	// window.widgets.emplace_back(bitmap);
	window.add(*bitmap);
	return *bitmap;
}
TOOLKIT_NAMESPACE::Bitmap &TOOLKIT_NAMESPACE::Bitmap::create(Window &window, int x, int y, size_t w, size_t h)
{
	TOOLKIT_NAMESPACE::Bitmap *bitmap = new TOOLKIT_NAMESPACE::Bitmap(window, x, y, w, h);
	// window.widgets.emplace_back(bitmap);
	window.add(*bitmap);
	return *bitmap;
}

TOOLKIT_NAMESPACE::Bitmap::~Bitmap()
{
	Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

	erase();
}

TOOLKIT_NAMESPACE::RGBA &TOOLKIT_NAMESPACE::Bitmap::at(size_t x, size_t y)
{
	render = true;
	return data[y][x];
}

void TOOLKIT_NAMESPACE::Bitmap::alloc(size_t width, size_t height)
{
	erase();
	geometry->srcR.w = w = width;
	geometry->srcR.h = h = height;

	data = new TOOLKIT_NAMESPACE::RGBA *[height];
	for (size_t i = 0; i < height; ++i)
		data[i] = new TOOLKIT_NAMESPACE::RGBA[width];
	surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if (surface == nullptr)
	{
		SDL_PrintError(Error);
	}
}

void TOOLKIT_NAMESPACE::Bitmap::erase()
{
	if (w == 0 || h == 0)
		return;

	SDL_FreeSurface(surface);
	if (tex != nullptr)
		SDL_DestroyTexture(tex);
	SDL_PrintIfError(Warn);

	geometry->srcR.w = w = 0;
	geometry->srcR.h = h = 0;

	data = nullptr;
}

void TOOLKIT_NAMESPACE::Bitmap::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);
}

void TOOLKIT_NAMESPACE::Bitmap::update()
{
	return_if(!visible);

	if (Point::mousePos().intercept(geometry->destR))
	{
		// if (window.hovering == nullptr)
		// {
		// 	window.hovering = this;
		// }
		// if (window.hovering == this)
		// {
		onHover();
		if (pressed)
		{
			onClicked();

			if (window.event.type == SDL_MOUSEBUTTONUP && window.event.button.button == SDL_BUTTON_LEFT)
			{
				onMouseUp();

				pressed = false;
			}
		}
		else
		{
			if (window.event.type == SDL_MOUSEBUTTONDOWN && window.event.button.button == SDL_BUTTON_LEFT)
			{
				onMouseDown();
				onClicked();

				pressed = true;
			}
		}
		// }
	}
	else
	{
		// if (window.hovering == this)
		// {
		pressed = false;
		// window.hovering = nullptr;
		// }
	}

	if (render)
	{
		if (tex != NULL)
			SDL_DestroyTexture(tex);
		SDL_PrintIfError(Warn);

		updateSurface();
		tex = window.renderer->createTextureFromSurface(surface);

		render = false;
	}
}

void TOOLKIT_NAMESPACE::Bitmap::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	window.renderer->drawTexture(tex, &geometry->srcR, &geometry->destR);
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}
