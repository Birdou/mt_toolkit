
#include "mt_widget.hpp"

std::string TOOLKIT_NAMESPACE::Widget::defaultFont = "assets/fonts/segoeui.ttf";
int TOOLKIT_NAMESPACE::Widget::defaultFontSize = 14;

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Widget::counter;

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

TOOLKIT_NAMESPACE::Widget::Widget(Widget &widget, const std::string &id) : id(id), window(widget.window)
{
	font = widget.font;
	geometry = std::unique_ptr<Geometry>(new Geometry());
	counter.widgetCount++;
}
TOOLKIT_NAMESPACE::Widget::Widget(Window &window, const std::string &id, int x, int y) : id(id), window(window)
{
	font = std::shared_ptr<Font>(new Font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Geometry>(new Geometry(x, y));
	counter.widgetCount++;

	// window.widgets.emplace_back(this);
}
TOOLKIT_NAMESPACE::Widget::Widget(Window &window, const std::string &id, int x, int y, int w, int h) : id(id), window(window)
{
	font = std::shared_ptr<Font>(new Font(window.getApplication(), defaultFont, defaultFontSize));
	geometry = std::unique_ptr<Geometry>(new Geometry(x, y, w, h));
	geometry->normalize();
	counter.widgetCount++;

	// window.widgets.emplace_back(this);
}
TOOLKIT_NAMESPACE::Widget::~Widget()
{
	if (cursor != nullptr)
		SDL_FreeCursor(cursor);

	Debug("Destroyed " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");
}
bool TOOLKIT_NAMESPACE::Widget::isEnabled() const noexcept
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

bool TOOLKIT_NAMESPACE::Widget::isActive() const noexcept
{
	return active;
}
bool TOOLKIT_NAMESPACE::Widget::isHoverScrollable() const noexcept
{
	return hoverScroll;
}
std::string TOOLKIT_NAMESPACE::Widget::getId() const noexcept
{
	return id;
}

void TOOLKIT_NAMESPACE::Widget::setScheme(ColorScheme scheme)
{
	this->scheme = scheme;
	currentBackgroundColor = scheme.background.normal;
	currentBorderColor = scheme.border.normal;
	font->color = scheme.font.normal;
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

TOOLKIT_NAMESPACE::Window &TOOLKIT_NAMESPACE::Widget::getApplication() const noexcept
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