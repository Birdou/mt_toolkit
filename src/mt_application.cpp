
#include "mt_application.hpp"

#include <algorithm>

#include "mt_lib.hpp"
#include "mt_window.hpp"

Mt_application::Mt_application()
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

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

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

Mt_window *Mt_application::createWindow(const std::string &title, int w, int h)
{
	auto window = new Mt_window(*this, title, w, h);
	window->init();

	windows.emplace(title, window);

	return window;
}

int Mt_application::operator()()
{
	return run();
}

int Mt_application::run()
{
	running = true;
	while (running)
	{
		// FRAME START
		fStart = SDL_GetTicks();

		for (auto &window : windows)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					running = false;
					break;
				}

				window.second->handleEvents();
			}
			window.second->update();
			window.second->draw();
		}

		// FRAME END
		frameTime = SDL_GetTicks() - fStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return 0;
}
void Mt_application::stop()
{
	running = false;
}