#ifndef D1F85919_0137_4B95_8145_7C93672B9E7B
#define D1F85919_0137_4B95_8145_7C93672B9E7B

#include "mt_application.hpp"

class Mt_widget;

class Mt_window
{
private:
	Mt_application &application;
	std::string title;

	int width, height;

	int flags = 0;

public:
	std::vector<Mt_widget *> widgets;

	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Color renderColor = {255, 255, 255, 255};

	SDL_Event event;

	bool fullscreen = false;
	bool resizable = false;

	void *hovering = nullptr;
	bool hover(void *obj);

	Mt_window(Mt_application &application, const std::string &title, int w, int h);
	~Mt_window();

	void init();

	void setIcon(const char *path);
	void setSize(int w, int h);

	int getH() const;
	int getW() const;

	Mt_application &getApplication() const;

	void handleEvents();
	void update();
	void draw();
};

#endif /* D1F85919_0137_4B95_8145_7C93672B9E7B */
