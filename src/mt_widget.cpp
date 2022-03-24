
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
{}

Mt_widget::Mt_widget(Mt_widget& widget) : window(widget.window)
{
	font = widget.font;
	geometry = std::unique_ptr<Mt_geometry>(new Mt_geometry());
	parent = &widget;

	// Mesmo que as classes pai tratem do widget, isso precisa estar aqui
	// para que o hittest funcione adequadamente
	// window.widgets.emplace_back(this);
}
Mt_widget::Mt_widget(Mt_window& window, int x, int y) : window(window)
{
	font = std::shared_ptr<Mt_font>(new Mt_font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Mt_geometry>(new Mt_geometry(x, y));

	// window.widgets.emplace_back(this);
}
Mt_widget::Mt_widget(Mt_window& window, int x, int y, int w, int h) : window(window)
{
	font = std::shared_ptr<Mt_font>(new Mt_font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Mt_geometry>(new Mt_geometry(x, y, w, h));
	geometry->normalize();

	// window.widgets.emplace_back(this);
}
Mt_widget::~Mt_widget()
{
	Debug("Destroying widget...");

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

void* Mt_widget::getParent()
{
	return parent;
}

void Mt_widget::setScheme(Mt_colorScheme scheme)
{
	this->scheme = scheme;
	this->backgroundColor = scheme.background.normal;
	this->borderColor = scheme.border.normal;
}
void Mt_widget::fadeToNormal()
{
	backgroundColor.fadeInto(scheme.background.normal);
	borderColor.fadeInto(scheme.border.normal);
	font->color.fadeInto(scheme.font.normal);
}
void Mt_widget::fadeToHover()
{
	backgroundColor.fadeInto(scheme.background.hover);
	borderColor.fadeInto(scheme.border.hover);
	font->color.fadeInto(scheme.font.hover);
}
void Mt_widget::fadeToClicked()
{
	backgroundColor.fadeInto(scheme.background.clicked);
	borderColor.fadeInto(scheme.border.clicked);
	font->color.fadeInto(scheme.font.clicked);
}
void Mt_widget::fadeToFocused()
{
	backgroundColor.fadeInto(scheme.background.focused);
	borderColor.fadeInto(scheme.border.focused);
	font->color.fadeInto(scheme.font.focused);
}

Mt_window& Mt_widget::getApplication() const
{
	return window;
}

void Mt_widget::handleEvent()
{}
void Mt_widget::update()
{}
void Mt_widget::draw()
{}