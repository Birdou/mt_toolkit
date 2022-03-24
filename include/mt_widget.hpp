#ifndef C7773821_9806_4DA9_86DA_A944C8E7E591
#define C7773821_9806_4DA9_86DA_A944C8E7E591

#include <functional>
#include <memory>

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

#define UI_BUTTON_COLOR_SCHEME        \
	{                                 \
		{                             \
			{225, 225, 225, 255},     \
			{229, 241, 251, 255},     \
			{204, 228, 247, 255},     \
			{0, 0, 0, 0},             \
		},                            \
			{                         \
				{173, 173, 173, 255}, \
				{0, 120, 215, 255},   \
				{0, 84, 153, 255},    \
				{0, 0, 0, 0},         \
			},                        \
			{                         \
				{0, 0, 0, 255},       \
				{0, 0, 0, 255},       \
				{0, 0, 0, 255},       \
				{0, 0, 0, 255},       \
			},                        \
	}
#define BUTTON_COLOR_SCHEME           \
	{                                 \
		{                             \
			{240, 240, 240, 255},     \
			{218, 218, 218, 255},     \
			{96, 96, 96, 255},        \
			{0, 0, 0, 0},             \
		},                            \
			{                         \
				{0, 0, 0, 0},         \
				{0, 0, 0, 0},         \
				{0, 0, 0, 0},         \
				{0, 0, 0, 0},         \
			},                        \
			{                         \
				{96, 96, 96, 255},    \
				{0, 0, 0, 255},       \
				{255, 255, 255, 255}, \
				{0, 0, 0, 255},       \
			},                        \
	}
#define BAR_COLOR_SCHEME          \
	{                             \
		{                         \
			{205, 205, 205, 255}, \
			{166, 166, 166, 255}, \
			{96, 96, 96, 255},    \
			{0, 0, 0, 0},         \
		},                        \
			{                     \
				{0, 0, 0, 0},     \
				{0, 0, 0, 0},     \
				{0, 0, 0, 0},     \
				{0, 0, 0, 0},     \
			},                    \
			{                     \
				{0, 0, 0, 255},   \
				{0, 0, 0, 255},   \
				{0, 0, 0, 255},   \
				{0, 0, 0, 255},   \
			},                    \
	}
#define UI_TEXTINPUT_COLOR_SCHEME     \
	{                                 \
		{                             \
			{255, 255, 255, 255},     \
			{255, 255, 255, 255},     \
			{255, 255, 255, 255},     \
			{255, 255, 255, 255},     \
		},                            \
			{                         \
				{122, 122, 122, 255}, \
				{23, 23, 23, 255},    \
				{23, 23, 23, 255},    \
				{0, 120, 215, 255},   \
			},                        \
			{                         \
				{0, 0, 0, 255},       \
				{0, 0, 0, 255},       \
				{0, 0, 0, 255},       \
				{0, 0, 0, 255},       \
			},                        \
	}
#define UI_CHECKBOX_COLOR_SCHEME    \
	{                               \
		{                           \
			{255, 255, 255, 255},   \
			{255, 255, 255, 255},   \
			{204, 228, 247, 255},   \
			{0, 0, 0, 0},           \
		},                          \
			{                       \
				{51, 51, 51, 255},  \
				{0, 120, 215, 255}, \
				{0, 84, 153, 255},  \
				{0, 0, 0, 0},       \
			},                      \
			{                       \
				{0, 0, 0, 255},     \
				{0, 0, 0, 255},     \
				{0, 0, 0, 255},     \
				{0, 0, 0, 255},     \
			},                      \
	}

template <typename T>
using Event = std::function<T>;

class Mt_widget
{
protected:
	bool focused = false;
	bool released = true;
	bool hoverScroll = false;

	SDL_SystemCursor cursorId;
	SDL_Cursor* cursor = nullptr;
	void SetCursor(SDL_SystemCursor id);

	void* parent = nullptr;

	bool active = true;

	virtual void init();

public:
	Mt_window& window;
	bool visible = true;

	std::unique_ptr<Mt_geometry> geometry;
	std::shared_ptr<Mt_font> font;

	Mt_color backgroundColor;
	Mt_color borderColor;

	struct Mt_colorScheme
	{
		struct Mt_colors
		{
			Mt_RGBA normal;
			Mt_RGBA hover;
			Mt_RGBA clicked;
			Mt_RGBA focused;
		};
		Mt_colors background;
		Mt_colors border;
		Mt_colors font;
	};
	Mt_colorScheme scheme;

	static std::string defaultFont;
	static int defaultFontSize;

	Mt_widget(Mt_widget& widget);
	Mt_widget(Mt_window& window, int x, int y);
	Mt_widget(Mt_window& window, int x, int y, int w, int h);
	virtual ~Mt_widget();

	void destroy();

	bool isActive() const;
	bool isHoverScrollable() const;

	void* getParent();

	void setScheme(Mt_colorScheme scheme);
	void fadeToNormal();
	void fadeToHover();
	void fadeToClicked();
	void fadeToFocused();

	Mt_window& getApplication() const;

	const Event<void()> none = []() {};

	Event<void()> onHovering = none;
	Event<void()> onClicked = none;
	Event<void()> onHover = none;
	Event<void()> onMouseDown = none;
	Event<void()> onMouseUp = none;
	Event<void()> onMouseLeave = none;
	Event<void()> onFocus = none;
	Event<void()> onLostFocus = none;

	Event<void()> onCaretMoved = none;
	Event<void()> onTextModified = none;

	Event<void()> onKeydown = none;
	Event<void()> onKeyup = none;
	Event<void()> onMouseWheelMoved = none;

	Event<void(int width, int height)> onWindowSizeChanged;
	Event<void(int width, int height)> onWindowResized;

	virtual void handleEvent();
	virtual void update();
	virtual void draw();
};

#endif /* C7773821_9806_4DA9_86DA_A944C8E7E591 */
