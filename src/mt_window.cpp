
#include "mt_window.hpp"

#include "mt_widget.hpp"

Mt_window::Mt_window(Mt_application &application, const std::string &title, int w, int h) : application(application), title(title), width(w), height(h)
{
}

Mt_window::~Mt_window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Mt_window::init()
{
	if (fullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable)
		flags |= SDL_WINDOW_RESIZABLE;

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

void Mt_window::setIcon(const char *path)
{
	SDL_Surface *surf = IMG_Load(path);
	SDL_SetWindowIcon(window, surf);
	SDL_FreeSurface(surf);
}

void Mt_window::setSize(int w, int h)
{
	width = w;
	height = h;
	SDL_SetWindowSize(window, w, h);
}

int Mt_window::getH() const
{
	return height;
}

int Mt_window::getW() const
{
	return width;
}

Mt_application &Mt_window::getApplication() const
{
	return application;
}

void Mt_window::handleEvents()
{
	this->event = application.event;

	switch (event.window.event)
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow(window);
			break;
		}
	}
	for (auto widget : widgets)
		widget->handleEvent();
}

void Mt_window::update()
{
	for (auto widget : widgets)
		widget->update();
}

void Mt_window::draw()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);

	for (auto widget : widgets)
		widget->draw();

	SDL_RenderPresent(renderer);
}