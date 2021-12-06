#ifndef C7773821_9806_4DA9_86DA_A944C8E7E591
#define C7773821_9806_4DA9_86DA_A944C8E7E591

#include <functional>

#include "mt_application.hpp"
#include "mt_font.hpp"
#include "mt_geometry.hpp"

#define return_if(x) \
	if (x)           \
	{                \
		return;      \
	}

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

	void *parent = nullptr;

public:
	bool visible = true;
	Mt_geometry *geometry = nullptr;
	Mt_font *font = nullptr;

	static std::string defaultFont;
	static int defaultFontSize;

	Mt_widget(Mt_widget &widget) : application(widget.application), font(widget.font)
	{
		geometry = new Mt_geometry();
		parent = &widget;
	}
	Mt_widget(Mt_application &application, int x, int y) : application(application)
	{
		font = new Mt_font(application, defaultFont, defaultFontSize);
		geometry = new Mt_geometry(x, y);

		application.widgets.emplace_back(this);
	}
	Mt_widget(Mt_application &application, int x, int y, int w, int h) : application(application)
	{
		font = new Mt_font(application, defaultFont, defaultFontSize);
		geometry = new Mt_geometry(x, y, w, h);
		geometry->normalize();

		application.widgets.emplace_back(this);
	}
	virtual ~Mt_widget()
	{
		if (parent == nullptr)
			delete font;
		delete geometry;

		if (cursor)
			SDL_FreeCursor(cursor);
	}

	Mt_application &getApplication() const { return application; }

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
