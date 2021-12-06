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

class Mt_window;

class Mt_application
{
private:
	bool running = false;
	bool initialized = false;

	std::map<std::pair<std::string, int>, TTF_Font *> fonts;

	Uint32 fStart;
	Uint32 frameTime;

	unsigned frameDelay;

public:
	std::map<std::string, Mt_window *> windows;
	SDL_Event event;

	unsigned short targetFPS = 60;

	TTF_Font *getFont(const std::string &path, int fontSize);

	Mt_application();
	~Mt_application();

	void init();

	Mt_window *createWindow(const std::string &title, int w, int h);

	int operator()();
	int run();
	void stop();
};

#endif /* B8D83158_3027_49A9_BAA8_0A7468DAD697 */
