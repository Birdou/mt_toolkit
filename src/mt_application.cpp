
#include "mt_application.hpp"

#include <algorithm>

#include "mt_lib.hpp"
#include "mt_window.hpp"

Mt_application::Mt_application(const std::string &title, int width, int height, int flags) : window(*new Mt_window(*this, title, width, height, flags))
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
		SDL_PrintError(Error);
	}
	// TODO seria mais apropriado que essa função fosse manipulada pelos widgets de textinput
	SDL_StartTextInput();

	window.destroyOnClose = true;
}

Mt_application::~Mt_application()
{
	Debug("Destroying application...");
	SDL_StopTextInput();

	delete &window;

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
		TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);
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
	running = true;

	while (running)
	{
		fStart = SDL_GetTicks();

		if (!window.isActive())
			break;

		while (SDL_PollEvent(&event))
			window.handleEvents();

		Mt_colormanager::update();

		window.update();
		window.draw();

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