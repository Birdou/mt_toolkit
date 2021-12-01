
#include "mt_application.hpp"

#include <algorithm>

#include "mt_lib.hpp"
#include "mt_widget.hpp"

Mt_application::Mt_application(int width, int height, const std::string &title) : title(title), width(width), height(height)
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
}

Mt_application::~Mt_application()
{
	SDL_StopTextInput();
	for (auto font : fonts)
	{
		TTF_CloseFont(font.second);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Mt_application::setIcon(const char *path)
{
	if (!initialized)
		std::cout << "Warning: " << __func__ << " must be used after Application init to work properly." << std::endl;

	SDL_Surface *surf = IMG_Load(path);
	SDL_SetWindowIcon(window, surf);
	SDL_FreeSurface(surf);
}

void Mt_application::setSize(int w, int h)
{
	if (!initialized)
		std::cout << "Warning: " << __func__ << " must be used after Application init to work properly." << std::endl;

	width = w;
	height = h;
	SDL_SetWindowSize(window, w, h);
}

int Mt_application::getHeight() const
{
	return height;
}

int Mt_application::getWidth() const
{
	return width;
}

TTF_Font *Mt_application::getFont(const std::string &path, int fontSize)
{
	auto iterator = std::find_if(fonts.begin(), fonts.end(),
								 [&](std::pair<std::pair<std::string, int>, TTF_Font *> pair)
								 {
									 return pair.first.first == path && pair.first.second == fontSize;
								 });
	if (iterator != fonts.end())
	{
		return iterator->second;
	}
	else
	{
		TTF_Font *font = Mt_lib::loadFont(path, fontSize);
		fonts.emplace(std::pair<std::string, int>(path, fontSize), font);
		return font;
	}
}

void Mt_application::init()
{
	frameDelay = 1000 / targetFPS;

	if (fullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable)
		flags |= SDL_WINDOW_RESIZABLE;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);

		const int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
		if (IMG_Init(flags) != flags)
		{
			std::cout << "IMG_Init: " << IMG_GetError() << std::endl;
		}
		if (TTF_Init() != 0)
		{
			std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
		}
	}
	else
	{
		std::cout << "SDL_Init: " << SDL_GetError() << std::endl;
	}
	SDL_StartTextInput();
	initialized = true;
}

void Mt_application::run()
{
	if (!initialized)
	{
		std::cout << "The application cannot run without first being initialized." << std::endl;
		return;
	}
	running = true;
	while (running)
	{
		//FRAME START
		fStart = SDL_GetTicks();

		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);

		//HANDLE EVENTS
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}

			for (auto widget : widgets)
				widget->handleEvents();
		}

		//UPDATE
		for (auto widget : widgets)
			widget->update();

		//DRAW
		for (auto widget : widgets)
			widget->draw();

		//FRAME END
		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - fStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
}
