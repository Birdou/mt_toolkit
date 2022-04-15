
#include "mt_application.hpp"

#include <algorithm>

#include "mt_lib.hpp"
#include "mt_window.hpp"

Mt_application::Mt_application(const std::string& title, int width, int height, int flags)
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

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
		SDL_PrintError(Error);
	}

	//std::unique_ptr<Mt_window> _window(new Mt_window(*this, title, width, height, flags));
	//window = std::move(_window);
	window = new Mt_window(*this, title, width, height, flags);
	window->destroyOnClose = true;

	running = true;
}

Mt_application::~Mt_application()
{
	Debug("Destroying application...");
	SDL_StopTextInput();

	for (auto coroutine : coroutines)
	{
		coroutine->join();
		delete coroutine;
	}

	for (auto font : fonts)
		TTF_CloseFont(font.second);

	delete window;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	Debug("Done.");
}

TTF_Font* Mt_application::getFont(const std::string& path, int fontSize)
{
	auto iterator = std::find_if(fonts.begin(), fonts.end(),
		[&](std::pair<std::pair<std::string, int>, TTF_Font*> pair)
		{
			return pair.first.first == path && pair.first.second == fontSize;
		});
	if (iterator != fonts.end())
	{
		return iterator->second;
	}
	else
	{
		TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
		if (font)
		{
			fonts.emplace(std::pair<std::string, int>(path, fontSize), font);
			return font;
		}
		else
		{
			Error("Invalid font. (did you enter the correct path?)");
			return nullptr;
		}
	}
}

int Mt_application::operator()()
{
	return run();
}

int Mt_application::run()
{
	Log("Start application running at " << targetFPS << " FPS...");
	Log("Current coroutines: " << coroutines.size());
	Log("Loaded fonts: " << fonts.size());

	frameDelay = 1000 / (double)targetFPS;
	while (window->isActive() && running)
	{
		fStart = SDL_GetTicks();

		while (SDL_PollEvent(&event))
			window->handleEvents();

		Mt_colormanager::update();

		window->update();
		window->draw();

		frameTime = SDL_GetTicks() - fStart;

		float elapsed = frameTime / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		SDL_Delay(std::floor(frameDelay - elapsed));

	}
	running = false;

	return 0;
}

void Mt_application::stop()
{
	running = false;
}