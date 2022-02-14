#ifndef D1F85919_0137_4B95_8145_7C93672B9E7B
#define D1F85919_0137_4B95_8145_7C93672B9E7B

#include "mt_core.hpp"

#include <thread>
#include <exception>

#include "mt_color.hpp"

enum windowFlags
{
	FULLSCREEN = SDL_WINDOW_FULLSCREEN,
	OPENGL = SDL_WINDOW_OPENGL,
	SHOWN = SDL_WINDOW_SHOWN,
	HIDDEN = SDL_WINDOW_HIDDEN,
	BORDERLESS = SDL_WINDOW_BORDERLESS,
	RESIZABLE = SDL_WINDOW_RESIZABLE,
	MINIMIZED = SDL_WINDOW_MINIMIZED,
	MAXIMIZED = SDL_WINDOW_MAXIMIZED,
	INPUT_GRABBED = SDL_WINDOW_INPUT_GRABBED,
	INPUT_FOCUS = SDL_WINDOW_INPUT_FOCUS,
	MOUSE_FOCUS = SDL_WINDOW_MOUSE_FOCUS,
	FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
	FOREIGN = SDL_WINDOW_FOREIGN,
	ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI,
	MOUSE_CAPTURE = SDL_WINDOW_MOUSE_CAPTURE,
	ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP,
	SKIP_TASKBAR = SDL_WINDOW_SKIP_TASKBAR,
	UTILITY = SDL_WINDOW_UTILITY,
	TOOLTIP = SDL_WINDOW_TOOLTIP,
	POPUP_MENU = SDL_WINDOW_POPUP_MENU,
	VULKAN = SDL_WINDOW_VULKAN,
	METAL = SDL_WINDOW_METAL
};

struct window_not_found : public std::exception
{
	const char *what(const char *e) const noexcept
	{
		return e;
	}
};

enum messageBoxFlags
{
	ERROR = SDL_MESSAGEBOX_ERROR,
	WARNING = SDL_MESSAGEBOX_WARNING,
	INFORMATION = SDL_MESSAGEBOX_INFORMATION,
	BUTTONS_LTR = SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
	BUTTONS_RTL = SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT,
};

class Mt_application;
class Mt_widget;
class Mt_window
{
private:
	Mt_application &application;
	SDL_Window *window = nullptr;
	Uint32 windowID;
	std::string title;
	SDL_Rect rect;

	bool shown = true;
	bool active = false;
	bool initialized = false;
	bool draggable = false;

	std::map<std::string, Mt_window *> windows;
	std::vector<std::thread *> coroutines;

	Mt_window(const Mt_window &) = delete;

	class Mt_messageBox
	{
	private:
		Mt_window &window;

		std::vector<SDL_MessageBoxButtonData> buttons;

	public:
		Mt_messageBox(Mt_window &window) : window(window)
		{
		}

		std::string title;
		std::string message;
		Uint32 flags = INFORMATION;

		int buttonid;
		void addButton(const char *text, bool defaultEnter, bool defaultEscape = false)
		{
			Uint32 flags = 0;
			if (defaultEnter)
				flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
			else if (defaultEscape)
				flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;

			SDL_MessageBoxButtonData data;
			data.buttonid = (signed)buttons.size();
			data.flags = flags;
			data.text = text;
			buttons.emplace_back(data);
		}
		void show()
		{
			SDL_MessageBoxData messageboxdata = {
				flags,
				window.window,
				title.c_str(),
				message.c_str(),
				(signed)buttons.size(),
				buttons.data(),
				NULL};
			SDL_ShowMessageBox(&messageboxdata, &buttonid);
		}
	};

public:
	Mt_window(Mt_application &application, const std::string &title, int w, int h, int flags = 0);
	Mt_window(Mt_window &parentWindow, const std::string &title, int w, int h, int flags = 0);
	~Mt_window();

	std::vector<Mt_widget *> widgets;

	Mt_renderer *renderer = nullptr;
	SDL_Event event;

	Mt_colormanager colorManager;
	Mt_color backgroundColor = {255, 255, 255, 255};
	Mt_color borderColor = {0, 0, 0, 255};

	bool destroyOnClose = false;
	bool border = false;

	void setDraggable(bool draggable = true);
	bool isDraggable();

	void *hovering = nullptr;

	bool hover(void *obj);

	bool isActive() const;

	Mt_window &createChild(const std::string &title, const std::string &id, int width, int height, int flags = 0);
	Mt_window &getChildById(const std::string &id);

	void showSimpleMessageBox(const char *title, const char *message, int flags = INFORMATION);

	Mt_messageBox createMessageBox();

	void getPostition(int &x, int &y);

	void hide();
	void show();
	void destroy();

	void setSize(int w, int h);
	void setIcon(const char *file);

	int height() const;
	int width() const;

	static SDL_HitTestResult SDLCALL hitTest(SDL_Window *, const SDL_Point *pt, void *data);

	Mt_application &getApplication() const;

	void handleEvents();
	void update();
	void draw();
};

#endif /* D1F85919_0137_4B95_8145_7C93672B9E7B */
