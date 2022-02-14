
#include "mt_widget.hpp"

std::string Mt_widget::defaultFont = "assets/fonts/segoeui.ttf";
int Mt_widget::defaultFontSize = 14;

void Mt_widget::SetCursor(SDL_SystemCursor id)
{
	if (id == cursorId)
		return;

	if (cursor)
		SDL_FreeCursor(cursor);

	cursorId = id;
	cursor = SDL_CreateSystemCursor(id);
	SDL_SetCursor(cursor);
}

Mt_widget::Mt_widget(Mt_widget &widget) : window(widget.window), font(widget.font)
{
	geometry = new Mt_geometry();
	parent = &widget;

	window.widgets.emplace_back(this);
}
Mt_widget::Mt_widget(Mt_window &window, int x, int y) : window(window)
{
	font = new Mt_font(window.getApplication(), defaultFont, defaultFontSize);
	geometry = new Mt_geometry(x, y);

	window.widgets.emplace_back(this);
}
Mt_widget::Mt_widget(Mt_window &window, int x, int y, int w, int h) : window(window)
{
	font = new Mt_font(window.getApplication(), defaultFont, defaultFontSize);
	geometry = new Mt_geometry(x, y, w, h);
	geometry->normalize();

	window.widgets.emplace_back(this);
}
Mt_widget::~Mt_widget()
{
	Debug("Destroying widget...");
	if (parent == nullptr)
		delete font;
	delete geometry;

	if (cursor)
		SDL_FreeCursor(cursor);

	Debug("Done.");
}

void Mt_widget::destroy()
{
	active = false;
}
bool Mt_widget::isActive()
{
	return active;
}

void *Mt_widget::getParent()
{
	return parent;
}

Mt_window &Mt_widget::getApplication() const
{
	return window;
}

void Mt_widget::handleEvent()
{
}
void Mt_widget::update()
{
}
void Mt_widget::draw()
{
}