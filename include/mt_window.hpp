#ifndef D1F85919_0137_4B95_8145_7C93672B9E7B
#define D1F85919_0137_4B95_8145_7C93672B9E7B

#include "mt_core.hpp"

#include <thread>
#include <exception>
#include <algorithm>

#include "mt_color.hpp"

enum windowFlags
{
	/**
	 * @brief Define que a janela seja executada em tela cheia sem alterar sua resolução.
	 *
	 */
	FULLSCREEN = SDL_WINDOW_FULLSCREEN,
	// OPENGL = SDL_WINDOW_OPENGL,
	/**
	 * @brief Define que a janela seja exibida no momento em que for criada (esse é o padrão).
	 * 
	 */
	SHOWN = SDL_WINDOW_SHOWN,
	/**
	 * @brief Define que a janela não seja exibida no momento em que for criada.
	 * 
	 */
	HIDDEN = SDL_WINDOW_HIDDEN,
	/**
	 * @brief Define que a janela seja criada sem bordas, título e sem controles de interface do sistema (botões de minimizar, maximizar|restaurar e fechar).
	 * 
	 */
	BORDERLESS = SDL_WINDOW_BORDERLESS,
	/**
	 * @brief Define que a janela permita ser redimensionada.
	 * 
	 */
	RESIZABLE = SDL_WINDOW_RESIZABLE,
	/**
	 * @brief Define que a janela seja minimizada no momento em que for criada.
	 * 
	 */
	MINIMIZED = SDL_WINDOW_MINIMIZED,
	/**
	 * @brief Define que a janela seja maximizada no momento em que for criada.
	 * 
	 */
	MAXIMIZED = SDL_WINDOW_MAXIMIZED,
	// INPUT_GRABBED = SDL_WINDOW_INPUT_GRABBED,
	// INPUT_FOCUS = SDL_WINDOW_INPUT_FOCUS,
	// MOUSE_FOCUS = SDL_WINDOW_MOUSE_FOCUS,
	/**
	 * @brief Define que a janela seja executada em tela cheia usando da resolução nativa do monitor.
	 * 
	 */
	FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
	// FOREIGN = SDL_WINDOW_FOREIGN,
	// ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI,
	// MOUSE_CAPTURE = SDL_WINDOW_MOUSE_CAPTURE,
	/**
	 * @brief Define que a janela fique sempre à frente de outras janelas.
	 * 
	 */
	ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP,
	/**
	 * @brief Define que a janela não seja exibida na barra de tarefas.
	 * 
	 */
	SKIP_TASKBAR = SDL_WINDOW_SKIP_TASKBAR,
	/**
	 * @brief Define que a janela seja tratada como uma janela de utilidade.
	 * 
	 */
	UTILITY = SDL_WINDOW_UTILITY,
	/**
	 * @brief Define que a janela seja tratada como um tooltip.
	 * 
	 */
	TOOLTIP = SDL_WINDOW_TOOLTIP,
	/**
	 * @brief Define que a janela seja tratada como um menu popup.
	 * 
	 */
	POPUP_MENU = SDL_WINDOW_POPUP_MENU,
	// VULKAN = SDL_WINDOW_VULKAN,
	// METAL = SDL_WINDOW_METAL
};

struct window_not_found : public std::exception
{
	const char* what(const char* e) const noexcept
	{
		return e;
	}
};

enum messageBoxFlags
{
	mbERROR = SDL_MESSAGEBOX_ERROR,
	mbWARNING = SDL_MESSAGEBOX_WARNING,
	mbINFORMATION = SDL_MESSAGEBOX_INFORMATION,
#if defined(_WIN32)
	mbBUTTONS_LTR = SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
	mbBUTTONS_RTL = SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT,
#endif
};

class Mt_widget;
class Mt_parent
{
protected:
	std::vector<Mt_widget*> widgets;

public:
	virtual ~Mt_parent();

	const std::vector<Mt_widget*>& get() const noexcept;
	void add(Mt_widget& widget);
};

class Mt_application;
class Mt_window : public Mt_parent
{
private:
	Mt_application& application;
	SDL_Window* window = nullptr;
	Uint32 windowID;
	std::string title;
	SDL_Rect rect;

	bool shown = true;
	bool active = false;
	bool initialized = false;
	bool draggable = false;

	std::map<std::string, Mt_window*> windows;

	Mt_window(const Mt_window&) = delete;

	class Mt_messageBox
	{
	private:
		Mt_window& window;

		std::vector<SDL_MessageBoxButtonData> buttons;

	public:
		Mt_messageBox(Mt_window& window) : window(window)
		{}

		std::string title;
		std::string message;
		Uint32 flags = mbINFORMATION;

		int buttonid;
		void addButton(const char* text, bool defaultEnter, bool defaultEscape = false)
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
				NULL };
			SDL_ShowMessageBox(&messageboxdata, &buttonid);
		}
	};

public:
	Mt_window(Mt_application& application, const std::string& title, int w, int h, int flags = 0);
	Mt_window(Mt_window& parentWindow, const std::string& title, int w, int h, int flags = 0);
	~Mt_window();

	//std::vector<Mt_widget*> widgets;

	Mt_renderer* renderer = nullptr;
	SDL_Event event;

	Mt_colormanager colorManager;
	Mt_color backgroundColor = { 255, 255, 255, 255 };
	Mt_color borderColor = { 0, 0, 0, 255 };

	bool destroyOnClose = false;
	bool persistent = false;
	bool border = false;

	void setDraggable(bool draggable = true);
	bool isDraggable();

	Mt_widget* hovering = nullptr;

	bool hover(void* obj);

	bool isActive() const;

	Mt_window& createChild(const std::string& title, const std::string& id, int width, int height, int flags = 0);
	Mt_window& getChildById(const std::string& id);

	void showSimpleMessageBox(const char* title, const char* message, int flags = mbINFORMATION);

	Mt_messageBox createMessageBox();

	void getPostition(int& x, int& y);

	void hide();
	void show();
	void destroy();

	void setSize(int w, int h);
	void setIcon(const char* file);
	void setTitle(const char* title);

	int height() const;
	int width() const;

	static SDL_HitTestResult SDLCALL hitTest(SDL_Window*, const SDL_Point* pt, void* data);

	Mt_application& getApplication() const;

	void handleEvents();
	void update();
	void draw();
};

#endif /* D1F85919_0137_4B95_8145_7C93672B9E7B */
