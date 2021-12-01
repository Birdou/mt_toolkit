#ifndef FFA654A9_41EE_4A84_BF89_4157CBE82750
#define FFA654A9_41EE_4A84_BF89_4157CBE82750

#include <string>
#include <functional>

#include "mt_application.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"
#include "mt_widget.hpp"
#include "mt_color.hpp"

class Mt_button : public Mt_widget
{
private:
	SDL_Rect btn;
	std::string buttonText;
	SDL_Rect destR;
	bool pressed = false;
	int scale = 1;
	std::function<void()> func;

	SDL_Texture *font_texture = nullptr;
	SDL_Color font_color;

	Mt_color color;
	Mt_color frame_color;

	bool clicked = false;

	void onHover()
	{
		if (application.hovering == nullptr)
		{
			application.hovering = this;
			color.fadeInto(&hover_color);
			frame_color.fadeInto(&frame_hover_color);
		}
	}
	void onMouseDown()
	{
		if (application.hovering == this)
		{
			pressed = true;
			color.fadeInto(&clicked_color);
			frame_color.fadeInto(&frame_clicked_color);
		}
	}
	void onMouseUp()
	{
		if (application.hovering == this)
		{
			pressed = false;
			clicked = true;
			onHover();
			func();
		}
	}
	void onMouseLeave()
	{
		if (application.hovering == this)
		{
			pressed = false;
			application.hovering = nullptr;
			color.fadeInto(&normal_color);
			frame_color.fadeInto(&frame_normal_color);
		}
	}

public:
	SDL_Color normal_color = {225, 225, 225, 255};
	SDL_Color hover_color = {229, 241, 251, 255};
	SDL_Color clicked_color = {204, 228, 247, 255};

	SDL_Color frame_normal_color = {173, 173, 173, 255};
	SDL_Color frame_hover_color = {0, 120, 215, 255};
	SDL_Color frame_clicked_color = {0, 84, 153, 255};

	Mt_font font;

	Mt_button(Mt_application &application, int xpos, int ypos, int width, int height, const std::string &text, std::function<void()> func) : Mt_widget(application), buttonText(text), func(func), font(application)
	{

		font.setFont("assets/fonts/segoeui.ttf", 11);
		font_color = {0, 0, 0, 0};

		destR.w = width;
		destR.h = height;
		destR.x = xpos - (destR.w / 2);
		destR.y = ypos - (destR.h / 2);

		color.color = normal_color;
		frame_color.color = frame_normal_color;

		SetButtonText(text);
	}
	~Mt_button()
	{
		SDL_DestroyTexture(font_texture);
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	int getX() const { return destR.x; }
	int getY() const { return destR.y; }
	int getH() const { return destR.h; }
	int getW() const { return destR.w; }

	void setX(int x)
	{
		destR.x = x;
		updateTextPosition();
	}
	void setY(int y)
	{
		destR.y = y;
		updateTextPosition();
	}
	void fitH(int padding = 0)
	{
		destR.h = btn.h + padding;
		updateTextPosition();
	}

	void SetButtonText(const std::string &text)
	{
		if (buttonText == text)
			return;

		buttonText = text;

		font_texture = Mt_lib::renderWrapped(application.renderer, text, font.getFont(), destR.w, btn, TTF_RenderUTF8_Blended_Wrapped);

		updateTextPosition();
	}

	void updateTextPosition()
	{
		btn.x = destR.x + ((destR.w - btn.w) / 2);
		btn.y = destR.y + ((destR.h - btn.h) / 2);
	}

	bool actioned()
	{
		return clicked;
	}

	void handleEvents() override
	{
	}

	void update() override
	{
		clicked = false;
		color.update();
		frame_color.update();
		if (visible)
		{
			Mt_vector<int> mouse(Mt_vector<int>::mousePos());
			if (mouse.x < destR.x + destR.w && mouse.y < destR.y + destR.h &&
				mouse.x >= destR.x && mouse.y >= destR.y)
			{
				if (!pressed)
				{
					onHover();
					if (application.event.type == SDL_MOUSEBUTTONDOWN)
					{
						switch (application.event.button.button)
						{
						case SDL_BUTTON_LEFT:
							onMouseDown();
							break;
						default:
							break;
						}
					}
				}
				else if (application.event.type == SDL_MOUSEBUTTONUP)
				{
					switch (application.event.button.button)
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
	}

	void draw() override
	{
		if (visible)
		{
			Mt_lib::drawFillRectangle(application.renderer, destR, color.color);
			Mt_lib::drawRectangle(application.renderer, destR, frame_color.color);
			if (buttonText != "")
			{
				Mt_lib::drawTexture(application.renderer, font_texture, nullptr, &btn);
			}
		}
	}
};

#endif /* FFA654A9_41EE_4A84_BF89_4157CBE82750 */
