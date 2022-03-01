
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

void Mt_widget::init()
{
}

Mt_widget::Mt_widget(Mt_widget &widget) : window(widget.window)
{
	font = widget.font;
	geometry = new Mt_geometry();
	parent = &widget;

	// Mesmo que as classes pai tratem do widget, isso precisa estar aqui
	// para que o hittest funcione adequadamente
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
bool Mt_widget::isActive() const
{
	return active;
}
bool Mt_widget::isHoverScrollable() const
{
	return hoverScroll;
}

void *Mt_widget::getParent()
{
	return parent;
}

void Mt_widget::setScheme(Mt_colorScheme scheme)
{
	this->scheme = scheme;
	this->backgroundColor = scheme.background.normalColor;
	this->borderColor = scheme.frame.normalColor;
}
void Mt_widget::fadeToNormal()
{
	backgroundColor.fadeInto(&scheme.background.normalColor);
	borderColor.fadeInto(&scheme.frame.normalColor);
	// font->color.fadeInto(&scheme.font.normalColor);
}
void Mt_widget::fadeToHover()
{
	backgroundColor.fadeInto(&scheme.background.hoverColor);
	borderColor.fadeInto(&scheme.frame.hoverColor);
	// font->color.fadeInto(&scheme.font.hoverColor);
}
void Mt_widget::fadeToClicked()
{
	backgroundColor.fadeInto(&scheme.background.clickedColor);
	borderColor.fadeInto(&scheme.frame.clickedColor);
	// font->color.fadeInto(&scheme.font.clickedColor);
}
void Mt_widget::fadeToFocused()
{
	backgroundColor.fadeInto(&scheme.background.focusedColor);
	borderColor.fadeInto(&scheme.frame.focusedColor);
	// font->color.fadeInto(&scheme.font.focusedColor);
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