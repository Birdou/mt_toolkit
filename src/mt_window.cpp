
#include "mt_window.hpp"

#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"

#define RESIZE_BORDER 5
SDL_HitTestResult SDLCALL Mt_window::hitTest(SDL_Window *, const SDL_Point *pt, void *data)
{
	Mt_window *window = (Mt_window *)data;
	Mt_vector<int> vec(*pt);

	for (auto &widget : window->widgets)
		if (vec.intercept(widget->geometry->destR))
			return SDL_HITTEST_NORMAL;

	int w = window->getW();
	int h = window->getH();

	if (vec.x < RESIZE_BORDER && vec.y < RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_TOPLEFT;
	else if (vec.x > RESIZE_BORDER && vec.x < w - RESIZE_BORDER && vec.y < RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_TOP;
	else if (vec.x > w - RESIZE_BORDER && vec.y < RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_TOPRIGHT;
	else if (vec.x > w - RESIZE_BORDER && vec.y > RESIZE_BORDER && vec.y < h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_RIGHT;
	else if (vec.x > w - RESIZE_BORDER && vec.y > h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
	else if (vec.x < w - RESIZE_BORDER && vec.x > RESIZE_BORDER && vec.y > h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_BOTTOM;
	else if (vec.x < RESIZE_BORDER && vec.y > h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_BOTTOMLEFT;
	else if (vec.x < RESIZE_BORDER && vec.y < h - RESIZE_BORDER && vec.y > RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_LEFT;

	return SDL_HITTEST_DRAGGABLE;
}

Mt_window::Mt_window(Mt_application &application, const std::string &title, int w, int h) : application(application), title(title)
{
	rect.w = w;
	rect.h = h;

	application.windows.emplace(title, this);
}

Mt_window &Mt_window::create(Mt_application &application, const std::string &title, int w, int h)
{
	return *(new Mt_window(application, title, w, h));
}

Mt_window::~Mt_window()
{
	Debug("Destroying window");
	for (auto widget : widgets)
		delete widget;

	if (renderer)
		SDL_DestroyRenderer(renderer);

	if (window)
		SDL_DestroyWindow(window);
}

void Mt_window::init()
{
	return_if(initialized);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, rect.w, rect.h, flags);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	windowID = SDL_GetWindowID(window);

	if (draggable)
		SDL_SetWindowHitTest(this->window, hitTest, (void *)this);

	active = true;
}

void Mt_window::setIcon(const char *path)
{
	SDL_Surface *surf = IMG_Load(path);
	SDL_SetWindowIcon(window, surf);
	SDL_FreeSurface(surf);
}

void Mt_window::setSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
	SDL_SetWindowSize(window, w, h);
}

int Mt_window::getH() const
{
	return rect.h;
}

int Mt_window::getW() const
{
	return rect.w;
}

Mt_application &Mt_window::getApplication() const
{
	return application;
}

void Mt_window::handleEvents()
{
	return_if(!shown);

	this->event = application.event;

	if (event.window.windowID == windowID)
	{
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
			if (destroyOnClose)
				active = false;
			shown = false;
			break;
		}

		for (auto widget : widgets)
			widget->handleEvent();
	}
}

void Mt_window::update()
{
	return_if(!shown);

	for (auto widget : widgets)
		widget->update();
}

void Mt_window::draw()
{
	return_if(!shown);

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);

	for (auto widget : widgets)
		widget->draw();

	if (border)
		Mt_lib::drawRectangle(renderer, rect, Mt_lib::color(0, 0, 0));

	SDL_RenderPresent(renderer);
}