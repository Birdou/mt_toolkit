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

	Mt_checkbox(Mt_window &window, int x, int y, int size) : Mt_widget(window, x, y, size, size)
	{
		geometry->destR.w = geometry->destR.h = size;
		geometry->destR.x = x;
		geometry->destR.y = y;

		checkDest.w = checkDest.h = size * checked_size;

		color.color = normal_color;
		frame_color.color = frame_normal_color;
	}
	Mt_checkbox(const Mt_checkbox &) = delete;

public:
	static Mt_checkbox &create(Mt_window &window, int x, int y, int size) { return *(new Mt_checkbox(window, x, y, size)); }

	~Mt_checkbox()
	{
		Debug("Destroying checkbox");
		// Debug("Done.");
	}

	SDL_Color normal_color = {255, 255, 255, 255};
	SDL_Color hover_color = {255, 255, 255, 255};
	SDL_Color clicked_color = {204, 228, 247, 255};
	SDL_Color frame_normal_color = {51, 51, 51, 255};
	SDL_Color frame_hover_color = {0, 120, 215, 255};
	SDL_Color frame_clicked_color = {0, 84, 153, 255};

	float checked_size = .7f;

	bool checked()
	{
		return isChecked;
	}

	void handleEvent() override
	{
		HANDLE_WINDOW_EVENTS;

		return_if(!visible);
	}

	void update() override
	{
		color.update();
		frame_color.update();

		return_if(!visible);

		if (Mt_vector<int>::mousePos().intercept(geometry->destR))
		{
			if (window.hovering == nullptr)
			{
				window.hovering = this;
				color.fadeInto(&hover_color);
				frame_color.fadeInto(&frame_hover_color);
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
						color.fadeInto(&clicked_color);
						frame_color.fadeInto(&frame_clicked_color);
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
				color.fadeInto(&normal_color);
				frame_color.fadeInto(&frame_normal_color);
			}
		}
	}

	void
	draw() override
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
