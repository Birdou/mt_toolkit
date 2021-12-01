#ifndef A09E3D2A_8B1E_4F55_AB69_136314F85BE0
#define A09E3D2A_8B1E_4F55_AB69_136314F85BE0

#include "mt_application.hpp"
#include "mt_widget.hpp"
#include "mt_caret.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"
#include "mt_color.hpp"

class Mt_textbox : public Mt_widget
{
private:
	std::string input;

	SDL_Rect destR;
	SDL_Rect textSrc = {0, 0, 0, 0}, textDest = {0, 0, 0, 0};
	SDL_Texture *texture = nullptr;
	SDL_Color textColor;
	bool render = false;

	Mt_caret caret;
	size_t caretPos = 0;

	bool focused = false;
	bool released = true;

	Mt_color color;
	Mt_color frame_color;

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
			onFocus();
			released = false;
		}
	}
	void onMouseUp()
	{
		released = true;
	}
	void onMouseLeave()
	{
		if (application.hovering == this)
		{
			application.hovering = nullptr;
		}
	}
	void onFocus()
	{
		if (application.hovering == this)
		{
			color.fadeInto(&focused_color);
			frame_color.fadeInto(&frame_focused_color);
			focused = true;
		}
	}
	void onLostFocus()
	{
		color.fadeInto(&normal_color);
		frame_color.fadeInto(&frame_normal_color);
		focused = false;
	}

public:
	bool editable = true;

	SDL_Color normal_color = {255, 255, 255, 255};
	SDL_Color focused_color = {255, 255, 255, 255};
	SDL_Color hover_color = {255, 255, 255, 255};
	SDL_Color frame_normal_color = {122, 122, 122, 255};
	SDL_Color frame_focused_color = {0, 120, 215, 255};
	SDL_Color frame_hover_color = {23, 23, 23, 255};

	Mt_font font;
	Mt_textbox(Mt_application &application, int xpos, int ypos, int width, int height, int fontSize, SDL_Color textColor) : Mt_widget(application), textColor(textColor), caret(application, fontSize), font(application)
	{
		font.setFont("assets/fonts/segoeui.ttf", fontSize);

		destR.w = width;
		destR.h = height;
		destR.x = xpos - (destR.w / 2);
		destR.y = ypos - (destR.h / 2);

		caret.destR.x = destR.x + (caret.destR.w / 2);
		caret.destR.y = destR.y + ((destR.h / 2) - (caret.destR.h / 2));

		color.color = normal_color;
		frame_color.color = frame_normal_color;
	}
	~Mt_textbox()
	{
		SDL_DestroyTexture(texture);
	}

	std::string str() const
	{
		return input;
	}
	void str(const std::string &str)
	{
		input = str;
		caretPos = input.length();
		render = true;
		onTextModified();
	}

	void caretLeft()
	{
		if (caretPos > 0)
		{
			caretPos--;
			if (caret.destR.x < destR.x)
			{
				textSrc.x -= Mt_lib::substrWidth(application.renderer, font.getFont(), input, caretPos, 1);
			}
		}
		else
		{
			textSrc.x = 0;
		}
	}
	void caretRight(int len = 1)
	{
		if (caretPos + len <= input.length())
		{
			if (caret.destR.x + caret.destR.w > destR.x + destR.w)
			{
				textSrc.x += Mt_lib::substrWidth(application.renderer, font.getFont(), input, caretPos, len);
			}
			caretPos += len;
		}
		else
		{
			if (textSrc.w >= destR.w)
				textSrc.x = Mt_lib::substrWidth(application.renderer, font.getFont(), input, 0) - destR.w;
		}
	}

	void handleEvents() override
	{
		if (focused && editable)
		{
			if (application.event.type == SDL_KEYDOWN)
			{
				switch (application.event.key.keysym.sym)
				{
				case SDLK_LEFT:
					caretLeft();
					break;
				case SDLK_RIGHT:
					caretRight();
					break;
				case SDLK_HOME:
					caretPos = 0;
					textSrc.x = 0;
					break;
				case SDLK_END:
					caretPos = input.length();
					if (textSrc.w >= destR.w)
						textSrc.x = Mt_lib::substrWidth(application.renderer, font.getFont(), input, 0) - destR.w;
					break;
				case SDLK_BACKSPACE:
					if (input.size() > 0 && caretPos > 0)
					{
						caretPos--;
						textSrc.x -= Mt_lib::substrWidth(application.renderer, font.getFont(), input, caretPos, 1);
						if (textSrc.x < 0)
							textSrc.x = 0;
						input.erase(input.begin() + caretPos);

						render = true;
						onTextModified();
					}
					break;
				case SDLK_DELETE:
					if (input.size() > 0 && caretPos < input.length())
					{
						input.erase(input.begin() + caretPos);

						render = true;
						onTextModified();
					}
					break;
				case SDLK_c:
					if (SDL_GetModState() & KMOD_CTRL)
					{
						SDL_SetClipboardText(input.c_str());
					}
					break;
				case SDLK_v:
					if (SDL_GetModState() & KMOD_CTRL)
					{
						std::string text(SDL_GetClipboardText());
						input.insert(caretPos, SDL_GetClipboardText());
						caretRight(text.length());

						render = true;
						onTextModified();
					}
					break;
				}
			}
			else if (application.event.type == SDL_TEXTINPUT)
			{
				std::string text(application.event.text.text);
				if (!(SDL_GetModState() & KMOD_CTRL && (text[0] == 'c' || text[0] == 'C' || text[0] == 'v' || text[0] == 'V')))
				{
					input.insert(caretPos, text);
					caretRight(text.length());

					render = true;
					onTextModified();
				}
			}
		}
	}

	void update() override
	{
		color.update();
		frame_color.update();
		if (visible)
		{
			Mt_vector<int> mouse(Mt_vector<int>::mousePos());
			if (mouse.x < destR.x + destR.w && mouse.y < destR.y + destR.h &&
				mouse.x >= destR.x && mouse.y >= destR.y)
			{
				if (!focused)
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
			}
			else if (application.event.type == SDL_MOUSEBUTTONDOWN && released)
			{
				switch (application.event.button.button)
				{
				case SDL_BUTTON_LEFT:
					onLostFocus();
					break;
				}
			}
			else
			{
				onMouseLeave();
			}

			if (application.event.type == SDL_MOUSEBUTTONUP)
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

		if (render)
		{
			if (texture != nullptr)
			{
				SDL_DestroyTexture(texture);
			}
			texture = Mt_lib::renderText(application.renderer, input, font.getFont(), textSrc, TTF_RenderUTF8_Blended);

			textDest.x = destR.x;
			textDest.y = destR.y + ((destR.h / 2) - (textSrc.h / 2));

			if (textSrc.w < destR.w)
			{
				textSrc.x = 0;
				textDest.w = textSrc.w;
			}
			else
			{
				textSrc.w = textDest.w = destR.w;
			}

			textDest.h = textSrc.h;
			render = false;
		}

		if (caretPos > 0)
		{
			caret.destR.x = destR.x + Mt_lib::substrWidth(application.renderer, font.getFont(), input, 0, caretPos) - textSrc.x;
		}
		else
		{
			caret.destR.x = destR.x;
		}
	}

	void draw() override
	{
		Mt_lib::drawFillRectangle(application.renderer, destR, color.color);
		Mt_lib::drawRectangle(application.renderer, destR, frame_color.color);
		if (texture != nullptr)
			Mt_lib::drawTexture(application.renderer, texture, &textSrc, &textDest);

		if (focused && editable)
		{
			caret.draw();
		}
	}
};

#endif /* A09E3D2A_8B1E_4F55_AB69_136314F85BE0 */
