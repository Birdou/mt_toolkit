#ifndef A09E3D2A_8B1E_4F55_AB69_136314F85BE0
#define A09E3D2A_8B1E_4F55_AB69_136314F85BE0

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_caret.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"

class Mt_textbox : public Mt_widget
{
private:
	Mt_caret *caret = nullptr;
	size_t caretPos = 0;

	Mt_label *text = nullptr;

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
		if (window.hovering == this)
		{
			window.hovering = nullptr;
		}
	}
	void onFocus()
	{
		if (window.hovering == this)
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

	Mt_textbox(Mt_widget &widget) : Mt_widget(widget)
	{
		caret = &Mt_caret::create(*this);

		text = &Mt_label::create(*this);
		text->autoupdate = false;

		color.color = normal_color;
		frame_color.color = frame_normal_color;
	}
	Mt_textbox(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
	{
		caret = &Mt_caret::create(*this);

		text = &Mt_label::create(*this);
		text->autoupdate = false;

		color.color = normal_color;
		frame_color.color = frame_normal_color;
	}
	Mt_textbox(const Mt_textbox &) = delete;

public:
	static Mt_textbox &create(Mt_widget &widget) { return *(new Mt_textbox(widget)); }
	static Mt_textbox &create(Mt_window &window, int x, int y, int w, int h) { return *(new Mt_textbox(window, x, y, w, h)); }

	~Mt_textbox()
	{
		Debug("Destroying textbox");
		delete caret;
		delete text;
	}

	bool editable = true;

	SDL_Color normal_color = {255, 255, 255, 255};
	SDL_Color focused_color = {255, 255, 255, 255};
	SDL_Color hover_color = {255, 255, 255, 255};
	SDL_Color frame_normal_color = {122, 122, 122, 255};
	SDL_Color frame_focused_color = {0, 120, 215, 255};
	SDL_Color frame_hover_color = {23, 23, 23, 255};

	std::string str() const
	{
		return text->text;
	}
	void str(const std::string &str)
	{
		text->text = str;
		caretPos = text->text.length();

		onTextModified();
	}

	void handleEvent() override
	{
		return_if(!visible);

		if (focused && editable)
		{
			if (window.event.type == SDL_KEYDOWN)
			{
				switch (window.event.key.keysym.sym)
				{
				case SDLK_LEFT:
					if (caretPos > 0)
					{
						caretPos--;
					}
					pointCursor();
					break;
				case SDLK_RIGHT:
					if (caretPos + 1 <= text->text.length())
					{
						caretPos++;
					}
					pointCursor();
					break;
				case SDLK_HOME:
					caretPos = 0;
					geometry->srcR.x = 0;
					pointCursor();
					break;
				case SDLK_END:
					caretPos = text->text.length();
					if (text->geometry->srcR.w >= geometry->destR.w)
						geometry->srcR.x = Mt_lib::substrWidth(window.renderer, font->getFont(), text->text, 0) - geometry->destR.w;
					pointCursor();
					break;

				case SDLK_BACKSPACE:
					if (text->text.size() > 0 && caretPos > 0)
					{
						caretPos--;
						text->geometry->srcR.x -= Mt_lib::substrWidth(window.renderer, font->getFont(), text->text, caretPos, 1);
						if (text->geometry->srcR.x < 0)
							text->geometry->srcR.x = 0;
						text->text.erase(text->text.begin() + caretPos);

						onTextModified();
						pointCursor();
					}
					break;
				case SDLK_DELETE:
					if (text->text.size() > 0 && caretPos < text->text.length())
					{
						text->text.erase(text->text.begin() + caretPos);

						onTextModified();
						pointCursor();
					}
					break;
				case SDLK_c:
					if (SDL_GetModState() & KMOD_CTRL)
						SDL_SetClipboardText(str().c_str());
					break;
				case SDLK_v:
					if (SDL_GetModState() & KMOD_CTRL)
					{
						const char *content = SDL_GetClipboardText();
						std::string ftext(content);
						Mt_lib::replaceAll(ftext, "\t", "    ");
						str(ftext);
						delete[] content;

						onTextModified();
						pointCursor();
					}
					break;
				}
			}
			else if (window.event.type == SDL_TEXTINPUT)
			{
				std::string content(window.event.text.text);
				if (!(SDL_GetModState() & KMOD_CTRL && (content[0] == 'c' || content[0] == 'C' || content[0] == 'v' || content[0] == 'V')))
				{
					text->text.insert(caretPos, content);
					text->update();

					caretPos += content.length();

					onTextModified();
					pointCursor();
				}
			}
		}
	}

	void pointCursor()
	{
		updateCaretPosition();
		if (caret->geometry->destR.x < geometry->destR.x)
		{
			geometry->srcR.x += geometry->destR.x - caret->geometry->destR.x;
		}
		else
		{
			int xw = geometry->destR.x + geometry->destR.w, cxw = caret->geometry->destR.x + caret->geometry->destR.w;
			if (cxw >= xw)
			{
				geometry->srcR.x -= cxw - xw;
			}
		}
		updateCaretPosition();
	}
	void updateCaretPosition()
	{
		if (caretPos > 0)
		{
			int tlen = Mt_lib::substrWidth(window.renderer, text->font->getFont(), text->text, 0, caretPos);
			caret->geometry->destR.x = geometry->destR.x + tlen + geometry->srcR.x;
		}
		else
		{
			caret->geometry->destR.x = geometry->destR.x;
		}

		int vspace = text->font->getH();
		caret->geometry->destR.y = geometry->destR.y;
		if (caret->geometry->destR.y + vspace <= geometry->destR.y || caret->geometry->destR.y >= geometry->destR.y + geometry->destR.h ||
			caret->geometry->destR.x + caret->geometry->getW() <= geometry->destR.x || caret->geometry->destR.x >= geometry->destR.x + geometry->destR.w)
		{
			caret->visible = false;
		}
		else
		{
			caret->visible = true;
			Mt_lib::confine(caret->geometry->destR.y, caret->geometry->srcR.y, caret->geometry->destR.h, caret->geometry->srcR.h, vspace, geometry->destR.y, geometry->destR.h);
			Mt_lib::confine(caret->geometry->destR.x, caret->geometry->srcR.x, caret->geometry->destR.w, caret->geometry->srcR.w, caret->geometry->getW(), geometry->destR.x, geometry->destR.w);
		}
	}

	void update() override
	{
		return_if(!visible);

		color.update();
		frame_color.update();
		text->update();

		if (focused && editable)
			caret->update();

		text->geometry->srcR.x = -geometry->srcR.x;

		text->geometry->destR.x = geometry->destR.x;
		text->geometry->destR.y = geometry->srcR.y + geometry->destR.y;

		text->geometry->srcR.w = text->geometry->destR.w = std::min(geometry->srcR.x + text->geometry->getW(), geometry->destR.w);
		if (text->geometry->destR.w > 0)
		{
			text->visible = true;
			Mt_lib::confine(text->geometry->destR.y, text->geometry->srcR.y, text->geometry->destR.h, text->geometry->srcR.h, text->font->getH(), geometry->destR.y, geometry->destR.h);
		}
		updateCaretPosition();

		if (Mt_vector<int>::mousePos().intercept(geometry->destR))
		{
			if (!focused)
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
		}
		else if (window.event.type == SDL_MOUSEBUTTONDOWN && released)
		{
			switch (window.event.button.button)
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

		if (window.event.type == SDL_MOUSEBUTTONUP)
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

	void draw() override
	{
		return_if(!visible);

		Mt_lib::drawFillRectangle(window.renderer, geometry->destR, color.color);
		Mt_lib::drawRectangle(window.renderer, geometry->destR, frame_color.color);

		text->draw();

		if (focused && editable)
			caret->draw();
	}
};

#endif /* A09E3D2A_8B1E_4F55_AB69_136314F85BE0 */
