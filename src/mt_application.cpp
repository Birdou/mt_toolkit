
#include "mt_application.hpp"

#include <algorithm>

#include "mt_lib.hpp"
#include "mt_window.hpp"

Mt_application::Mt_application(const std::string &title) : window(Mt_window::create(*this, title, 600, 400))
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

	frameDelay = 1000 / targetFPS;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		const int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
		if (IMG_Init(flags) != flags)
		{
			Warn("IMG_Init: " << IMG_GetError());
		}
		if (TTF_Init() != 0)
		{
			Warn("TTF_Init: " << TTF_GetError());
		}
	}
	else
	{
		Error("SDL_Init: " << SDL_GetError());
	}
	SDL_StartTextInput();

	window.destroyOnClose = true;
}

Mt_application::~Mt_application()
{
	Debug("Destroying application...");
	SDL_StopTextInput();

	for (auto window : windows)
		delete window.second;

	for (auto font : fonts)
		TTF_CloseFont(font.second);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	Debug("Done.");
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

int Mt_application::operator()()
{
	return run();
}

int Mt_application::run()
{
	for (auto window : windows)
		window.second->init();

	running = true;

	while (running)
	{
		fStart = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			for (auto &window : windows)
				window.second->handleEvents();
		}

		for (auto it = windows.begin(); it != windows.end();)
		{
			if (!it->second->isActive())
			{
				if (it == windows.begin())
				{
					running = false;
					break;
				}
				else
				{
					delete it->second;
					windows.erase(it);
				}
			}
			else
			{
				it++;
			}
		}

		for (auto &window : windows)
		{
			window.second->update();
			window.second->draw();
		}

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
