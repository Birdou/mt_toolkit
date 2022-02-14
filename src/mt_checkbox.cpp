
#include "mt_checkbox.hpp"

Mt_checkbox::Mt_checkbox(Mt_window &window, int x, int y, int size) : Mt_widget(window, x, y, size, size)
{
	geometry->destR.w = geometry->destR.h = size;
	geometry->destR.x = x;
	geometry->destR.y = y;

	normalColor = {255, 255, 255, 255};
	hoverColor = {255, 255, 255, 255};
	clickedColor = {204, 228, 247, 255};
	frameNormalColor = {51, 51, 51, 255};
	frameHoverColor = {0, 120, 215, 255};
	frameClickedColor = {0, 84, 153, 255};

	backgroundColor = normalColor;
	borderColor = frameNormalColor;

	check = &Mt_label::create(*this);
	check->geometry->setW(size * checked_size);
	check->geometry->setH(size * checked_size);
	check->geometry->normalize();
}
Mt_checkbox::Mt_checkbox(const Mt_checkbox &) = delete;
Mt_checkbox &Mt_checkbox::create(Mt_window &window, int x, int y, int size)
{
	return *(new Mt_checkbox(window, x, y, size));
}

Mt_checkbox::~Mt_checkbox()
{
	Debug("Destroying checkbox");
	// Debug("Done.");
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

	if (Mt_vector<int>::mousePos().intercept(geometry->destR))
	{
		if (window.hovering == nullptr)
		{
			window.hovering = this;
			backgroundColor.fadeInto(&hoverColor);
			borderColor.fadeInto(&frameHoverColor);
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
					backgroundColor.fadeInto(&clickedColor);
					borderColor.fadeInto(&frameClickedColor);
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
			backgroundColor.fadeInto(&normalColor);
			borderColor.fadeInto(&frameNormalColor);
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
		Mt_lib::confineX(check->geometry, geometry->destR);
		check->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + ((geometry->getH() - check->geometry->getH()) / 2);
		Mt_lib::confineY(check->geometry, geometry->destR);
		check->backgroundColor = borderColor;
		check->draw();
	}
}