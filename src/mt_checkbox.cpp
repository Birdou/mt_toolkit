
#include "mt_checkbox.hpp"

Mt_checkbox::Mt_checkbox(Mt_window &window, int x, int y, int size) : Mt_widget(window, x, y, size, size)
{
	geometry->destR.w = geometry->destR.h = size;
	geometry->destR.x = x;
	geometry->destR.y = y;

	scheme = UI_CHECKBOX_COLOR_SCHEME;

	backgroundColor = scheme.background.normal;
	borderColor = scheme.border.normal;

	check = &Mt_label::create(*this);
	check->geometry->setW(size * checked_size);
	check->geometry->setH(size * checked_size);
	check->geometry->normalize();
}

void Mt_checkbox::init()
{
}

Mt_checkbox &Mt_checkbox::create(Mt_window &window, int x, int y, int size)
{
	Mt_checkbox *checkbox = new Mt_checkbox(window, x, y, size);
	window.widgets.emplace_back(checkbox);
	return *checkbox;
}

Mt_checkbox::~Mt_checkbox()
{
	Debug("Destroying checkbox...");

	delete check;

	Debug("Done.");
}
bool Mt_checkbox::checked()
{
	return isChecked;
}

void Mt_checkbox::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);
}

void Mt_checkbox::update()
{
	return_if(!visible);

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		if (window.hovering == nullptr)
		{
			window.hovering = this;
			backgroundColor.fadeInto(scheme.background.hover);
			borderColor.fadeInto(scheme.border.hover);
		}
		if (window.hovering == this)
		{
			onHover();
			if (window.event.type == SDL_MOUSEBUTTONDOWN && !pressed)
			{
				switch (window.event.button.button)
				{
				case SDL_BUTTON_LEFT:
					onMouseDown();

					pressed = true;
					backgroundColor.fadeInto(scheme.background.clicked);
					borderColor.fadeInto(scheme.border.clicked);
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
		}
	}
	else
	{
		if (window.hovering == this)
		{
			onMouseLeave();

			pressed = false;
			window.hovering = nullptr;
			backgroundColor.fadeInto(scheme.background.normal);
			borderColor.fadeInto(scheme.border.normal);
		}
	}
}

void Mt_checkbox::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	if (isChecked)
	{
		check->geometry->destR.x = (geometry->destR.x - geometry->srcR.x) + ((geometry->getW() - check->geometry->getW()) / 2);
		check->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + ((geometry->getH() - check->geometry->getH()) / 2);
		check->geometry->confine(geometry->destR);
		check->backgroundColor = borderColor;
		check->draw();
	}
}