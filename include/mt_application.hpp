#ifndef B8D83158_3027_49A9_BAA8_0A7468DAD697
#define B8D83158_3027_49A9_BAA8_0A7468DAD697

#include <iostream>
#include <vector>
#include <map>
#include <thread>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "escolor.hpp"

#ifdef DEBUG
#define Debug(x) std::cout << colorstream("[DEBUG] " << __PRETTY_FUNCTION__ << ": " << x, fDARK_GRAY) << std::endl
#define DebugFrame(renderer, destR, color) drawRectangle(renderer, destR, color)
#else
#define Debug(x)
#define DebugFrame(renderer, destR, color)
#endif
#define Log(x) std::cout << "[LOG] " << __FUNCTION__ << ": " << x << std::endl
#define Warn(x) std::cout << colorstream("[WARN] " << __FUNCTION__ << ": " << x, fLIGHT_YELLOW) << std::endl
#define Error(x) std::cout << colorstream("[ERROR] " << __FUNCTION__ << ": " << x, fRED) << std::endl
#define Critical(x) std::cout << colorstream("[CRITICAL] " << __FUNCTION__ << ": " << x, bWHITE << fRED) << std::endl

class Mt_window;

class Mt_application
{
private:
	bool running = false;

	std::map<std::pair<std::string, int>, TTF_Font *> fonts;

	Uint32 fStart = 0;
	Uint32 frameTime = 0;

	unsigned frameDelay;

	const unsigned short targetFPS = 60;

public:
	std::map<std::string, Mt_window *> windows;
	SDL_Event event;

	TTF_Font *getFont(const std::string &path, int fontSize);

	Mt_application(const std::string &);
	~Mt_application();

	Mt_window &window;

	int operator()();
	int run();
	void stop();
};

#endif /* B8D83158_3027_49A9_BAA8_0A7468DAD697 */
