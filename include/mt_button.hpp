#ifndef FFA654A9_41EE_4A84_BF89_4157CBE82750
#define FFA654A9_41EE_4A84_BF89_4157CBE82750

#include <string>
#include <functional>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"
#include "mt_widget.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"

class Mt_button : public Mt_widget
{
private:
	bool pressed = false;
	int scale = 1;
	std::function<void()> func;

	Mt_color color;
	Mt_color frameColor;

	bool clicked = false;

public:
	SDL_Color normal_color = {225, 225, 225, 255};
	SDL_Color hover_color = {229, 241, 251, 255};
	SDL_Color clicked_color = {204, 228, 247, 255};
	SDL_Color frame_normal_color = {173, 173, 173, 255};
	SDL_Color frame_hover_color = {0, 120, 215, 255};
	SDL_Color frame_clicked_color = {0, 84, 153, 255};

	Mt_label *label = nullptr;

	Mt_button(Mt_widget &widget) : Mt_widget(widget)
	{
		label = new Mt_label(*this);
		label->geometry->setAnchor(Mt_geometry::middle_center);
		label->renderMethod = [&](Mt_label *label)
		{
			return Mt_lib::renderWrapped(window.renderer, label->text, label->font, label->geometry, geometry->destR.w, TTF_RenderUTF8_Blended_Wrapped);
		};

		color.color = normal_color;
		frameColor.color = frame_normal_color;
	}
	Mt_button(Mt_window &window, int x, int y, int w, int h, std::function<void()> func) : Mt_widget(window, x, y, w, h), func(func)
	{
		label = new Mt_label(*this);
		label->geometry->setAnchor(Mt_geometry::middle_center);
		label->renderMethod = [&](Mt_label *label)
		{
			return Mt_lib::renderWrapped(window.renderer, label->text, label->font, label->geometry, geometry->destR.w, TTF_RenderUTF8_Blended_Wrapped);
		};

		color.color = normal_color;
		frameColor.color = frame_normal_color;
	}
	~Mt_button()
	{
		delete label;
	}

	void setFunction(std::function<void()> function)
	{
		this->func = function;
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	void updateTextPosition()
	{
		label->geometry->setX(geometry->destR.x + (geometry->destR.w / 2));
		label->geometry->setY(geometry->destR.y + (geometry->destR.h / 2));
	}

	bool actioned()
	{
		return clicked;
	}

	void fitH(int padding = 4)
	{
		geometry->setH(label->font->getH() + (2 * padding));
	}

	void handleEvent() override
	{
	}

	void update() override
	{
		clicked = false;
		color.update();
		frameColor.update();

		updateTextPosition();
		label->update();

		if (visible)
		{
			if (Mt_vector<int>::mousePos().intercept(geometry->destR))
			{
				onHover();
				// SetCursor(SDL_SYSTEM_CURSOR_IBEAM);
				if (window.hovering == nullptr)
				{
					window.hovering = this;
					color.fadeInto(&hover_color);
					frameColor.fadeInto(&frame_hover_color);
				}
				if (!pressed)
				{
					if (window.event.type == SDL_MOUSEBUTTONDOWN)
					{
						if (window.event.button.button == SDL_BUTTON_LEFT)
						{
							if (window.hovering == this)
							{
								onMouseDown();

								pressed = true;
								color.fadeInto(&clicked_color);
								frameColor.fadeInto(&frame_clicked_color);
							}
						}
					}
				}
				else if (window.event.type == SDL_MOUSEBUTTONUP)
				{
					if (window.event.button.button == SDL_BUTTON_LEFT)
					{
						if (window.hovering == this)
						{
							onMouseUp();

							pressed = false;
							clicked = true;

							func();
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
					color.fadeInto(&normal_color);
					frameColor.fadeInto(&frame_normal_color);
				}
			}
		}
	}

	void draw() override
	{
		if (visible)
		{
			Mt_lib::drawFillRectangle(window.renderer, geometry->destR, color.color);
			Mt_lib::drawRectangle(window.renderer, geometry->destR, frameColor.color);
			label->draw();
		}
	}
};

#endif /* FFA654A9_41EE_4A84_BF89_4157CBE82750 */
