
#include "widgets/mt_checkbox.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Checkbox::counter;

TOOLKIT_NAMESPACE::Checkbox::Checkbox(TOOLKIT_NAMESPACE::Window &window, int x, int y, int size) : Widget(window, getClassId(), x, y, size, size)
{

	geometry->destR.w = geometry->destR.h = size;
	geometry->destR.x = x;
	geometry->destR.y = y;

	scheme = UI_CHECKBOX_COLOR_SCHEME;

	currentBackgroundColor = scheme.background.normal;
	currentBorderColor = scheme.border.normal;

	check = &Label::create(*this);
	check->geometry->setW(size * checked_size);
	check->geometry->setH(size * checked_size);
	check->geometry->normalize();
}

void TOOLKIT_NAMESPACE::Checkbox::init()
{
}

TOOLKIT_NAMESPACE::Checkbox &TOOLKIT_NAMESPACE::Checkbox::create(TOOLKIT_NAMESPACE::Window &window, int x, int y, int size)
{
	TOOLKIT_NAMESPACE::Checkbox *checkbox = new TOOLKIT_NAMESPACE::Checkbox(window, x, y, size);
	// window.widgets.emplace_back(checkbox);
	window.add(*checkbox);
	return *checkbox;
}

TOOLKIT_NAMESPACE::Checkbox::~Checkbox()
{
	Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

	delete check;
}
bool TOOLKIT_NAMESPACE::Checkbox::checked()
{
	return isChecked;
}

void TOOLKIT_NAMESPACE::Checkbox::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);
}

void TOOLKIT_NAMESPACE::Checkbox::update()
{
	return_if(!visible);

	if (Point::mousePos().intercept(geometry->destR))
	{
		// if (window.hovering == nullptr)
		// {
		// window.hovering = this;
		fadeToHover();
		// }
		// if (window.hovering == this)
		// {
		onHover();
		if (window.event.type == SDL_MOUSEBUTTONDOWN && !pressed)
		{
			switch (window.event.button.button)
			{
			case SDL_BUTTON_LEFT:
				onMouseDown();

				pressed = true;
				fadeToClicked();
				break;
			default:
				break;
			}
		}
		else if (window.event.type == SDL_MOUSEBUTTONUP && pressed)
		{
			switch (window.event.button.button)
			{
			case SDL_BUTTON_LEFT:
				onMouseUp();

				isChecked = !isChecked;
				pressed = false;
				break;
			default:
				break;
			}
		}
		// }
	}
	else
	{
		// if (window.hovering == this)
		// {
		onMouseLeave();

		pressed = false;
		// window.hovering = nullptr;
		fadeToNormal();
		// }
	}
}

void TOOLKIT_NAMESPACE::Checkbox::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

	if (isChecked)
	{
		check->geometry->destR.x = (geometry->destR.x - geometry->srcR.x) + ((geometry->getW() - check->geometry->getW()) / 2);
		check->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + ((geometry->getH() - check->geometry->getH()) / 2);
		check->geometry->confine(geometry->destR);
		check->currentBackgroundColor = currentBorderColor;
		check->draw();
	}
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}