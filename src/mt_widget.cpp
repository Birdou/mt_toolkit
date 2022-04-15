
#include "mt_widget.hpp"

std::string Mt_widget::defaultFont = "assets/fonts/segoeui.ttf";
int Mt_widget::defaultFontSize = 14;
long Mt_widget::widgetCount = 0;
long Mt_widget::destroyedWidgetCount = 0;

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

Mt_widget::Mt_widget(Mt_widget& widget) : id(widgetCount++), window(widget.window)
{
	font = widget.font;
	geometry = std::unique_ptr<Mt_geometry>(new Mt_geometry());
}
Mt_widget::Mt_widget(Mt_window& window, int x, int y) : id(widgetCount++), window(window)
{
	font = std::shared_ptr<Mt_font>(new Mt_font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Mt_geometry>(new Mt_geometry(x, y));

	// window.widgets.emplace_back(this);
}
Mt_widget::Mt_widget(Mt_window& window, int x, int y, int w, int h) : id(widgetCount++), window(window)
{
	font = std::shared_ptr<Mt_font>(new Mt_font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Mt_geometry>(new Mt_geometry(x, y, w, h));
	geometry->normalize();

	// window.widgets.emplace_back(this);
}
Mt_widget::~Mt_widget()
{
	if (cursor != nullptr)
		SDL_FreeCursor(cursor);

	Debug("Destroyed widget (" << ++destroyedWidgetCount << "/" << widgetCount << ")");
}
bool Mt_widget::isEnabled() const
{
	return enabled;
}
void Mt_widget::enable()
{
	fadeToNormal();
	enabled = true;
}
void Mt_widget::disable()
{
	fadeToDisabled();
	enabled = false;
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

void Mt_widget::setScheme(Mt_colorScheme scheme)
{
	this->scheme = scheme;
	this->currentBackgroundColor = scheme.background.normal;
	this->currentBorderColor = scheme.border.normal;
}
void Mt_widget::fadeToNormal()
{
	currentBackgroundColor.fadeInto(scheme.background.normal);
	currentBorderColor.fadeInto(scheme.border.normal);
	font->color.fadeInto(scheme.font.normal);
}
void Mt_widget::fadeToHover()
{
	currentBackgroundColor.fadeInto(scheme.background.hover);
	currentBorderColor.fadeInto(scheme.border.hover);
	font->color.fadeInto(scheme.font.hover);
}
void Mt_widget::fadeToClicked()
{
	currentBackgroundColor.fadeInto(scheme.background.clicked);
	currentBorderColor.fadeInto(scheme.border.clicked);
	font->color.fadeInto(scheme.font.clicked);
}
void Mt_widget::fadeToFocused()
{
	currentBackgroundColor.fadeInto(scheme.background.focused);
	currentBorderColor.fadeInto(scheme.border.focused);
	font->color.fadeInto(scheme.font.focused);
}
void Mt_widget::fadeToDisabled()
{
	currentBackgroundColor.fadeInto(scheme.background.disabled);
	currentBorderColor.fadeInto(scheme.border.disabled);
	font->color.fadeInto(scheme.font.disabled);
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