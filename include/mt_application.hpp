#ifndef B8D83158_3027_49A9_BAA8_0A7468DAD697
#define B8D83158_3027_49A9_BAA8_0A7468DAD697

#include <iostream>
#include <vector>
#include <map>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Mt_widget;

class Mt_application
{
private:
	std::string title;

	int width, height;

	int flags = 0;

	Uint32 fStart;
	Uint32 frameTime;

	unsigned frameDelay;

	bool running = false;
	bool initialized = false;

	std::map<std::pair<std::string, int>, TTF_Font *> fonts;

public:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;

	SDL_Event event;

	SDL_Color renderColor = {255, 255, 255, 255};

	unsigned short targetFPS = 60;
	bool fullscreen = false;
	bool resizable = false;

	std::vector<Mt_widget *> widgets;

	TTF_Font *getFont(const std::string &path, int fontSize);

	void *hovering = nullptr;
	bool hover(void *obj);

	Mt_application(int width, int height, const std::string &title);
	~Mt_application();

	void setIcon(const char *path);
	void setSize(int w, int h);

	int getH() const;
	int getW() const;

	void init();
	int run();
};

#endif /* B8D83158_3027_49A9_BAA8_0A7468DAD697 */
