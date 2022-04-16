
#include "mt_widget.hpp"

std::string TOOLKIT_NAMESPACE::Widget::defaultFont = "assets/fonts/segoeui.ttf";
int TOOLKIT_NAMESPACE::Widget::defaultFontSize = 14;
long TOOLKIT_NAMESPACE::Widget::widgetCount = 0;
long TOOLKIT_NAMESPACE::Widget::destroyedWidgetCount = 0;

void TOOLKIT_NAMESPACE::Widget::SetCursor(SDL_SystemCursor id)
{
	if (id == cursorId)
		return;

	if (cursor)
		SDL_FreeCursor(cursor);

	cursorId = id;
	cursor = SDL_CreateSystemCursor(id);
	SDL_SetCursor(cursor);
}

void TOOLKIT_NAMESPACE::Widget::init()
{
}

TOOLKIT_NAMESPACE::Widget::Widget(Widget &widget) : id(widgetCount++), window(widget.window)
{
	font = widget.font;
	geometry = std::unique_ptr<Geometry>(new Geometry());
}
TOOLKIT_NAMESPACE::Widget::Widget(Window &window, int x, int y) : id(widgetCount++), window(window)
{
	font = std::shared_ptr<Font>(new Font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Geometry>(new Geometry(x, y));

	// window.widgets.emplace_back(this);
}
TOOLKIT_NAMESPACE::Widget::Widget(Window &window, int x, int y, int w, int h) : id(widgetCount++), window(window)
{
	font = std::shared_ptr<Font>(new Font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Geometry>(new Geometry(x, y, w, h));
	geometry->normalize();

	// window.widgets.emplace_back(this);
}
TOOLKIT_NAMESPACE::Widget::~Widget()
{
	if (cursor != nullptr)
		SDL_FreeCursor(cursor);

	Debug("Destroyed widget (" << ++destroyedWidgetCount << "/" << widgetCount << ")");
}
bool TOOLKIT_NAMESPACE::Widget::isEnabled() const
{
	return enabled;
}
void TOOLKIT_NAMESPACE::Widget::enable()
{
	fadeToNormal();
	enabled = true;
}
void TOOLKIT_NAMESPACE::Widget::disable()
{
	fadeToDisabled();
	enabled = false;
}
void TOOLKIT_NAMESPACE::Widget::destroy()
{
	active = false;
}

bool TOOLKIT_NAMESPACE::Widget::isActive() const
{
	return active;
}
bool TOOLKIT_NAMESPACE::Widget::isHoverScrollable() const
{
	return hoverScroll;
}

void TOOLKIT_NAMESPACE::Widget::setScheme(ColorScheme scheme)
{
	this->scheme = scheme;
	this->currentBackgroundColor = scheme.background.normal;
	this->currentBorderColor = scheme.border.normal;
}
void TOOLKIT_NAMESPACE::Widget::fadeToNormal()
{
	currentBackgroundColor.fadeInto(scheme.background.normal);
	currentBorderColor.fadeInto(scheme.border.normal);
	font->color.fadeInto(scheme.font.normal);
}
void TOOLKIT_NAMESPACE::Widget::fadeToHover()
{
	currentBackgroundColor.fadeInto(scheme.background.hover);
	currentBorderColor.fadeInto(scheme.border.hover);
	font->color.fadeInto(scheme.font.hover);
}
void TOOLKIT_NAMESPACE::Widget::fadeToClicked()
{
	currentBackgroundColor.fadeInto(scheme.background.clicked);
	currentBorderColor.fadeInto(scheme.border.clicked);
	font->color.fadeInto(scheme.font.clicked);
}
void TOOLKIT_NAMESPACE::Widget::fadeToFocused()
{
	currentBackgroundColor.fadeInto(scheme.background.focused);
	currentBorderColor.fadeInto(scheme.border.focused);
	font->color.fadeInto(scheme.font.focused);
}
void TOOLKIT_NAMESPACE::Widget::fadeToDisabled()
{
	currentBackgroundColor.fadeInto(scheme.background.disabled);
	currentBorderColor.fadeInto(scheme.border.disabled);
	font->color.fadeInto(scheme.font.disabled);
}

TOOLKIT_NAMESPACE::Window &TOOLKIT_NAMESPACE::Widget::getApplication() const
{
	return window;
}

void TOOLKIT_NAMESPACE::Widget::handleEvent()
{
}
void TOOLKIT_NAMESPACE::Widget::update()
{
}
void TOOLKIT_NAMESPACE::Widget::draw()
{
}