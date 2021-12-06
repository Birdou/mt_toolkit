#ifndef B8EAFDEA_B267_4EC6_896F_9F8F363AFC52
#define B8EAFDEA_B267_4EC6_896F_9F8F363AFC52

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_color.hpp"
#include "mt_vector.hpp"

class Mt_checkbox : public Mt_widget
{
private:
	SDL_Rect checkDest;
	bool isChecked = false;
	bool pressed = false;

	Mt_color color;
	Mt_color frame_color;

	void onHover()
	{
		if (window.hovering == nullptr)
		{
			window.hovering = this;
			color.fadeInto(&hover_color);
			frame_color.fadeInto(&frame_hover_color);
		}
	}
	void onMouseDown()
	{
		if (window.hovering == this)
		{
			pressed = true;
			color.fadeInto(&clicked_color);
			frame_color.fadeInto(&frame_clicked_color);
		}
	}
	void onMouseUp()
	{
		if (window.hovering == this)
		{
			isChecked = !isChecked;
			pressed = false;
		}
	}
	void onMouseLeave()
	{
		if (window.hovering == this)
		{
			pressed = false;
			window.hovering = nullptr;
			color.fadeInto(&normal_color);
			frame_color.fadeInto(&frame_normal_color);
		}
	}

public:
	SDL_Color normal_color = {255, 255, 255, 255};
	SDL_Color hover_color = {255, 255, 255, 255};
	SDL_Color clicked_color = {204, 228, 247, 255};
	SDL_Color frame_normal_color = {51, 51, 51, 255};
	SDL_Color frame_hover_color = {0, 120, 215, 255};
	SDL_Color frame_clicked_color = {0, 84, 153, 255};

	float checked_size = .7f;

	Mt_checkbox(Mt_window &window, int x, int y, int size) : Mt_widget(window, x, y, size, size)
	{
		geometry->destR.w = geometry->destR.h = size;
		geometry->destR.x = x;
		geometry->destR.y = y;

		checkDest.w = checkDest.h = size * checked_size;

		color.color = normal_color;
		frame_color.color = frame_normal_color;
	}
	~Mt_checkbox()
	{
	}

	bool checked()
	{
		return isChecked;
	}

	void handleEvent() override
	{
		return_if(!visible);

		Mt_vector<int> mouse(Mt_vector<int>::mousePos());
		if (mouse.x < geometry->destR.x + geometry->destR.w && mouse.y < geometry->destR.y + geometry->destR.h &&
			mouse.x >= geometry->destR.x && mouse.y >= geometry->destR.y)
		{
			if (!pressed)
			{
				onHover();
				if (window.event.type == SDL_MOUSEBUTTONDOWN)
				{
					switch (window.event.button.button)
					{
					case SDL_BUTTON_LEFT:
						onMouseDown();
						break;
					default:
						break;
					}
				}
			}
			else if (window.event.type == SDL_MOUSEBUTTONUP)
			{
				switch (window.event.button.button)
				{
				case SDL_BUTTON_LEFT:
					onMouseUp();
					break;
				default:
					break;
				}
			}
		}
		else
		{
			onMouseLeave();
		}
	}

	void update() override
	{
		return_if(!visible);

		color.update();
		frame_color.update();
	}

	void draw() override
	{
		return_if(!visible);

		Mt_lib::drawFillRectangle(window.renderer, geometry->destR, color.color);
		Mt_lib::drawRectangle(window.renderer, geometry->destR, frame_color.color);
		if (isChecked)
		{
			checkDest.x = geometry->destR.x + ((geometry->destR.w - checkDest.w) / 2);
			checkDest.y = geometry->destR.y + ((geometry->destR.h - checkDest.h) / 2);
			Mt_lib::drawFillRectangle(window.renderer, checkDest, frame_color.color);
		}
	}
};

#endif /* B8EAFDEA_B267_4EC6_896F_9F8F363AFC52 */
