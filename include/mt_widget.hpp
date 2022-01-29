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

class Mt_widget
{
protected:
	Mt_window &window;

	bool focused = false;
	bool released = true;

	SDL_SystemCursor cursorId;
	SDL_Cursor *cursor = nullptr;
	void SetCursor(SDL_SystemCursor id)
	{
		if (id == cursorId)
			return;

		if (cursor)
			SDL_FreeCursor(cursor);

		cursorId = id;
		cursor = SDL_CreateSystemCursor(id);
		SDL_SetCursor(cursor);
	}

	void *parent = nullptr;

	bool active = true;

public:
	bool visible = true;
	Mt_geometry *geometry = nullptr;
	Mt_font *font = nullptr;

	Mt_color backgroundColor;
	Mt_color borderColor;

	static std::string defaultFont;
	static int defaultFontSize;

	Mt_widget(Mt_widget &widget) : window(widget.window), font(widget.font)
	{
		geometry = new Mt_geometry();
		parent = &widget;

		window.widgets.emplace_back(this);
	}
	Mt_widget(Mt_window &window, int x, int y) : window(window)
	{
		font = new Mt_font(window.getApplication(), defaultFont, defaultFontSize);
		geometry = new Mt_geometry(x, y);

		window.widgets.emplace_back(this);
	}
	Mt_widget(Mt_window &window, int x, int y, int w, int h) : window(window)
	{
		font = new Mt_font(window.getApplication(), defaultFont, defaultFontSize);
		geometry = new Mt_geometry(x, y, w, h);
		geometry->normalize();

		window.widgets.emplace_back(this);
	}
	virtual ~Mt_widget()
	{
		Debug("Destroying widget...");
		if (parent == nullptr)
			delete font;
		delete geometry;

		if (cursor)
			SDL_FreeCursor(cursor);

		Debug("Done.");
	}

	void destroy() { active = false; }
	bool isActive() { return active; }

	void *getParent() { return parent; }

	Mt_window &getApplication() const { return window; }

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

	virtual void handleEvent() {}
	virtual void update() {}
	virtual void draw() {}
};

#endif /* C7773821_9806_4DA9_86DA_A944C8E7E591 */
