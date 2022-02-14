
#include "mt_button.hpp"

Mt_button::Mt_button(Mt_widget &widget) : Mt_widget(widget)
{
	init();
}
Mt_button::Mt_button(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	init();
}

Mt_button::Mt_button(const Mt_button &) = delete;

void Mt_button::init()
{
	label = &Mt_label::create(*this);
	label->geometry->setAnchor(middle_center);

	normalColor = {225, 225, 225, 255};
	hoverColor = {229, 241, 251, 255};
	clickedColor = {204, 228, 247, 255};
	frameNormalColor = {173, 173, 173, 255};
	frameHoverColor = {0, 120, 215, 255};
	frameClickedColor = {0, 84, 153, 255};

	backgroundColor = normalColor;
	borderColor = frameNormalColor;
}
Mt_button &Mt_button::create(Mt_widget &widget)
{
	return *(new Mt_button(widget));
}
Mt_button &Mt_button::create(Mt_window &window, int x, int y, int w, int h)
{
	return *(new Mt_button(window, x, y, w, h));
}

Mt_button::~Mt_button()
{
	Debug("Destroying button...");

	delete label;

	Debug("Done.");
}
void Mt_button::updateTextPosition()
{
	label->geometry->setX((geometry->destR.x - geometry->srcR.x) + (geometry->getW() / 2));
	Mt_lib::confineX(label->geometry, geometry->destR);
	label->geometry->setY((geometry->destR.y - geometry->srcR.y) + (geometry->getH() / 2));
	Mt_lib::confineY(label->geometry, geometry->destR);
}

bool Mt_button::actioned()
{
	return clicked;
}

void Mt_button::fitH(int padding)
{
	geometry->setH(label->font->getH() + (2 * padding));
}

void Mt_button::handleEvent()
{
	HANDLE_WINDOW_EVENTS;
}

void Mt_button::update()
{
	return_if(!visible);

	if (clicked)
	{
		function();
		clicked = false;
	}

	label->update();
	updateTextPosition();

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
			if (pressed)
			{
				if (SDL_GetTicks() - lastPressed > 300)
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
					onMouseDown();
					onClicked();

					pressed = true;
					lastPressed = SDL_GetTicks();

					backgroundColor.fadeInto(&clickedColor);
					borderColor.fadeInto(&frameClickedColor);
				}
			}
		}
	}
	else
	{
		if (window.hovering == this)
		{
			pressed = false;
			window.hovering = nullptr;
			backgroundColor.fadeInto(&normalColor);
			borderColor.fadeInto(&frameNormalColor);
		}
	}
}

void Mt_button::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	label->draw();
}