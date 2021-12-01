#ifndef C7773821_9806_4DA9_86DA_A944C8E7E591
#define C7773821_9806_4DA9_86DA_A944C8E7E591

#include <functional>

#include "mt_application.hpp"
#include "mt_font.hpp"
#include "mt_geometry.hpp"

using event = std::function<void()>;

class Mt_widget
{
protected:
	Mt_application &application;

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

public:
	bool visible = true;
	Mt_font font;
	Mt_geometry geometry;

	Mt_widget(Mt_application &application, int x, int y) : application(application), font(application, "assets/fonts/segoeui.ttf", 14), geometry(x, y)
	{
		application.widgets.emplace_back(this);
	}
	Mt_widget(Mt_application &application, int x, int y, int w, int h) : application(application), font(application, "assets/fonts/segoeui.ttf", 14), geometry(x, y, w, h)
	{
		application.widgets.emplace_back(this);
	}
	virtual ~Mt_widget()
	{
		if (cursor)
		{
			SDL_FreeCursor(cursor);
		}
	}

	const event none = []() {};
	event onHovering = none;
	event onClicked = none;
	event onHover = none;
	event onMouseDown = none;
	event onMouseUp = none;
	event onMouseLeave = none;
	event onFocus = none;
	event onLostFocus = none;
	event onCaretMoved = none;
	event onTextModified = none;
	event onWindowResized = none;

	virtual void handleEvents() {}
	virtual void update() {}
	virtual void draw() {}
};

#endif /* C7773821_9806_4DA9_86DA_A944C8E7E591 */
