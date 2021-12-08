#ifndef D1F85919_0137_4B95_8145_7C93672B9E7B
#define D1F85919_0137_4B95_8145_7C93672B9E7B

#include "mt_application.hpp"

class Mt_widget;

enum WindowFlags
{
	WINDOW_ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP,
	WINDOW_BORDERLESS = SDL_WINDOW_BORDERLESS,
	WINDOW_FULLSCREEN = SDL_WINDOW_FULLSCREEN,
	WINDOW_HIDDEN = SDL_WINDOW_HIDDEN,
	WINDOW_MAXIMIZED = SDL_WINDOW_MAXIMIZED,
	WINDOW_MINIMIZED = SDL_WINDOW_MINIMIZED,
	WINDOW_RESIZABLE = SDL_WINDOW_RESIZABLE,

	WINDOW_DRAGGABLE
};

class Mt_window
{
private:
	Mt_application &application;
	std::string title;

	SDL_Rect rect;

	Uint32 flags = 0;

	Uint32 windowID;

	bool shown = true;
	bool active = false;
	bool initialized = false;

	bool draggable = false;

	Mt_window(Mt_application &application, const std::string &title, int w, int h);
	Mt_window(const Mt_window &) = delete;

public:
	static Mt_window &create(Mt_application &application, const std::string &title, int w, int h);

	~Mt_window();

	std::vector<Mt_widget *> widgets;

	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Color renderColor = {255, 255, 255, 255};

	SDL_Event event;

	template <typename... Args>
	void setFlags(const Args... args)
	{
		flags = 0;
		std::vector<WindowFlags> vec = {args...};
		for (auto flag : vec)
		{
			switch (flag)
			{
			case WINDOW_DRAGGABLE:
				draggable = true;
				break;
			default:
				flags |= flag;
				break;
			}
		}
	}

	bool destroyOnClose = false;
	bool border = false;
	SDL_Color borderColor = {0, 0, 0, 255};

	void *hovering = nullptr;
	bool hover(void *obj);

	void init();

	bool isActive() const { return active; }

	void hide()
	{
		if (window)
			SDL_HideWindow(window);
		shown = false;
	}
	void show()
	{
		if (window)
			SDL_ShowWindow(window);
		shown = true;
	}
	void destroy() { active = false; }

	void setIcon(const char *path);
	void setSize(int w, int h);

	int getH() const;
	int getW() const;

	static SDL_HitTestResult SDLCALL hitTest(SDL_Window *, const SDL_Point *pt, void *data);

	Mt_application &getApplication() const;

	void handleEvents();
	void update();
	void draw();
};

#endif /* D1F85919_0137_4B95_8145_7C93672B9E7B */
