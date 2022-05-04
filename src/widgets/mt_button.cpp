
#include "widgets/mt_button.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Button::counter;

TOOLKIT_NAMESPACE::Button::Button(Widget &widget) : Widget(widget, getClassId())
{

	init();
}
TOOLKIT_NAMESPACE::Button::Button(TOOLKIT_NAMESPACE::Window &window, int x, int y, int w, int h) : Widget(window, getClassId(), x, y, w, h)
{

	init();
}

void TOOLKIT_NAMESPACE::Button::init()
{
	label = &Label::create(*this);

	setScheme(UI_BUTTON_COLOR_SCHEME);
}
TOOLKIT_NAMESPACE::Button &TOOLKIT_NAMESPACE::Button::create(Widget &widget)
{
	TOOLKIT_NAMESPACE::Button *button = new TOOLKIT_NAMESPACE::Button(widget);
	// widget.window.widgets.emplace_back(button);
	return *button;
}
TOOLKIT_NAMESPACE::Button &TOOLKIT_NAMESPACE::Button::create(TOOLKIT_NAMESPACE::Window &window, int x, int y, int w, int h)
{
	TOOLKIT_NAMESPACE::Button *button = new TOOLKIT_NAMESPACE::Button(window, x, y, w, h);
	// window.widgets.emplace_back(button);
	window.add(*button);
	return *button;
}

TOOLKIT_NAMESPACE::Button::~Button()
{
	Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

	delete label;
}

bool TOOLKIT_NAMESPACE::Button::actioned() const
{
	return clicked;
}

void TOOLKIT_NAMESPACE::Button::fitH(int padding)
{
	geometry->setH(label->font->getH() + (2 * padding));
}

void TOOLKIT_NAMESPACE::Button::operator()()
{
	this->function();
}

void TOOLKIT_NAMESPACE::Button::handleEvent()
{
	HANDLE_WINDOW_EVENTS;
}

void TOOLKIT_NAMESPACE::Button::update()
{
	return_if(!visible);

	if (clicked)
	{
		function();
		clicked = false;
	}
	const int difX = geometry->destR.x - geometry->srcR.x;
	const int difY = geometry->destR.y - geometry->srcR.y;

	label->geometry->destR.x = difX + ((geometry->getW() - label->geometry->getW()) / 2);
	label->geometry->destR.y = difY + ((geometry->getH() - label->geometry->getH()) / 2);
	label->geometry->confine(geometry->destR);
	label->update();

	if (Point::mousePos().intercept(geometry->destR) && enabled)
	{
		// if (window.hovering == nullptr)
		// {
		// window.hovering = this;
		fadeToHover();
		// }
		// if (window.hovering == this)
		// {
		onHover();
		if (pressed)
		{
			if (repeatInterval == 0 || SDL_GetTicks() - lastPressed > repeatInterval)
				onClicked();

			if (window.event.type == SDL_MOUSEBUTTONUP && window.event.button.button == SDL_BUTTON_LEFT)
			{
				onMouseUp();

				pressed = false;
				clicked = true;
			}
		}
		else
		{
			if (window.event.type == SDL_MOUSEBUTTONDOWN && window.event.button.button == SDL_BUTTON_LEFT)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				clickOffset = {
					x - difX,
					y - difY,
				};

				onMouseDown();
				onClicked();

				pressed = true;
				lastPressed = SDL_GetTicks();

				fadeToClicked();
			}
		}
		// }
	}
	else if (enabled)
	{
		fadeToNormal();
		// if (window.hovering == this)
		// {
		onMouseLeave();
		pressed = false;
		// window.hovering = nullptr;
		// }
	}
	// else
	// {
	// window.hovering = nullptr;
	// }
}

void TOOLKIT_NAMESPACE::Button::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	label->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}