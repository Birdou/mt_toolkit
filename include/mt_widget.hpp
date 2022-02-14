#ifndef C7773821_9806_4DA9_86DA_A944C8E7E591
#define C7773821_9806_4DA9_86DA_A944C8E7E591

#include <functional>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_font.hpp"
#include "mt_geometry.hpp"
#include "mt_color.hpp"

#define HANDLE_WINDOW_EVENTS                                                               \
	if (window.event.type == SDL_WINDOWEVENT)                                              \
	{                                                                                      \
		switch (window.event.window.event)                                                 \
		{                                                                                  \
		case SDL_WINDOWEVENT_SIZE_CHANGED:                                                 \
			if (onWindowSizeChanged)                                                       \
				onWindowSizeChanged(window.event.window.data1, window.event.window.data2); \
			break;                                                                         \
		case SDL_WINDOWEVENT_RESIZED:                                                      \
			if (onWindowResized)                                                           \
				onWindowResized(window.event.window.data1, window.event.window.data2);     \
			break;                                                                         \
		}                                                                                  \
	}

template <typename... Args>
using Event = std::function<void(Args...)>;

class Mt_widget
{
protected:
	Mt_window &window;

	bool focused = false;
	bool released = true;

	SDL_SystemCursor cursorId;
	SDL_Cursor *cursor = nullptr;
	void SetCursor(SDL_SystemCursor id);

	void *parent = nullptr;

	bool active = true;

public:
	bool visible = true;
	Mt_geometry *geometry = nullptr;
	Mt_font *font = nullptr;

	Mt_color backgroundColor;
	Mt_color borderColor;

	Mt_RGBA normalColor;
	Mt_RGBA focusedColor;
	Mt_RGBA hoverColor;
	Mt_RGBA clickedColor;

	Mt_RGBA frameNormalColor;
	Mt_RGBA frameFocusedColor;
	Mt_RGBA frameHoverColor;
	Mt_RGBA frameClickedColor;

	static std::string defaultFont;
	static int defaultFontSize;

	Mt_widget(Mt_widget &widget);
	Mt_widget(Mt_window &window, int x, int y);
	Mt_widget(Mt_window &window, int x, int y, int w, int h);
	virtual ~Mt_widget();

	void destroy();
	bool isActive();

	void *getParent();

	Mt_window &getApplication() const;

	const Event<> none = []() {};

	Event<> onHovering = none;
	Event<> onClicked = none;
	Event<> onHover = none;
	Event<> onMouseDown = none;
	Event<> onMouseUp = none;
	Event<> onMouseLeave = none;
	Event<> onFocus = none;
	Event<> onLostFocus = none;

	Event<> onCaretMoved = none;
	Event<> onTextModified = none;

	Event<> onKeydown = none;
	Event<> onKeyup = none;
	Event<> onMouseWheelMoved = none;

	Event<int, int> onWindowSizeChanged;
	Event<int, int> onWindowResized;

	virtual void handleEvent();
	virtual void update();
	virtual void draw();
};

#endif /* C7773821_9806_4DA9_86DA_A944C8E7E591 */
