
#include "mt_button.hpp"

Mt_button::Mt_button(Mt_widget &widget) : Mt_widget(widget)
{
	init();
}
Mt_button::Mt_button(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	init();
}

void Mt_button::init()
{
	label = &Mt_label::create(*this);

	scheme = UI_BUTTON_COLOR_SCHEME;
	// scheme.font.hoverColor = {255, 255, 255, 255};

	backgroundColor = scheme.background.normalColor;
	borderColor = scheme.frame.normalColor;
}
Mt_button &Mt_button::create(Mt_widget &widget)
{
	return *new Mt_button(widget);
}
Mt_button &Mt_button::create(Mt_window &window, int x, int y, int w, int h)
{
	return *new Mt_button(window, x, y, w, h);
}

Mt_button::~Mt_button()
{
	Debug("Destroying button...");

	delete label;

	Debug("Done.");
}
void Mt_button::updateTextPosition()
{
	label->geometry->destR.x = (geometry->destR.x - geometry->srcR.x) + ((geometry->getW() - label->geometry->getW()) / 2);
	label->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + ((geometry->getH() - label->geometry->getH()) / 2);
	label->geometry->confine(geometry->destR);
}

bool Mt_button::actioned() const
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

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		if (window.hovering == nullptr)
		{
			window.hovering = this;
			fadeToHover();
		}
		if (window.hovering == this)
		{
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
						x - (geometry->destR.x - geometry->srcR.x),
						y - (geometry->destR.y - geometry->srcR.y),
					};

					onMouseDown();
					onClicked();

					pressed = true;
					lastPressed = SDL_GetTicks();

					fadeToClicked();
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
			fadeToNormal();
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
