#ifndef C26CC945_6DCC_4AC9_AE1F_9F84A853E863
#define C26CC945_6DCC_4AC9_AE1F_9F84A853E863

#include <vector>
#include <algorithm>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_caret.hpp"
#include "mt_lib.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"

class Mt_textarea : public Mt_widget
{
private:
	Mt_label *input = nullptr;
	std::vector<Mt_label *> lines;

	size_t caretPos_x = 0;
	size_t caretPos_y = 0;

	Mt_color color;
	Mt_color frameColor;

	Mt_textarea(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
	{
		caret = &Mt_caret::create(*this);

		geometry->destR.x = x;
		geometry->destR.y = y;
		geometry->srcR.w = geometry->destR.w = w;
		geometry->srcR.h = geometry->destR.h = h;

		geometry->srcR.x = geometry->srcR.y = 0;

		input = newLine();

		pointCursor();

		color.color = normal_color;
		frameColor.color = frame_normal_color;
	}
	Mt_textarea(const Mt_textarea &) = delete;

public:
	static Mt_textarea &create(Mt_window &window, int x, int y, int w, int h) { return *(new Mt_textarea(window, x, y, w, h)); }

	~Mt_textarea()
	{
		Debug("Destroying textarea");
		for (auto line : lines)
			delete line;
	}

	SDL_Color textColor = {0, 0, 0, 255};

	SDL_Color normal_color = {255, 255, 255, 255};
	SDL_Color focused_color = {255, 255, 255, 255};
	SDL_Color hover_color = {255, 255, 255, 255};
	SDL_Color frame_normal_color = {122, 122, 122, 255};
	SDL_Color frame_focused_color = {0, 120, 215, 255};
	SDL_Color frame_hover_color = {23, 23, 23, 255};

	Mt_caret *caret = nullptr;

	bool editable = true;

	std::string str() const
	{
		std::stringstream stream;
		for (auto line : lines)
		{
			stream << line->text << std::endl;
		}
		return stream.str();
	}
	void str(const std::string &str)
	{
		lines.clear();
		input = nullptr;
		std::stringstream stream(str);
		while (!stream.eof())
		{
			std::string line;
			getline(stream, line);

			newLine(line);
		}
		input = lines.back();

		caretPos_x = input->text.length();
		caretPos_y = lines.size() - 1;
		input->update();
	}

	void pointCursor()
	{
		updateCaretPosition();
		if (caret->geometry->destR.y < geometry->destR.y)
		{
			geometry->srcR.y += geometry->destR.y - caret->geometry->destR.y;
		}
		else
		{
			int yh = geometry->destR.y + geometry->destR.h, cyh = caret->geometry->destR.y + caret->geometry->getH();
			if (cyh >= yh)
			{
				geometry->srcR.y -= cyh - yh;
			}
		}

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
		if (caretPos_x > 0)
		{
			int tlen = Mt_lib::substrWidth(window.renderer, input->font->getFont(), input->text, 0, caretPos_x);
			caret->geometry->destR.x = geometry->destR.x + tlen + geometry->srcR.x;
		}
		else
		{
			caret->geometry->destR.x = geometry->destR.x;
		}

		int vspace = input->font->getH();
		caret->geometry->destR.y = geometry->destR.y + (vspace * caretPos_y) + geometry->srcR.y;
		if (caret->geometry->destR.y + vspace <= geometry->destR.y || caret->geometry->destR.y >= geometry->destR.y + geometry->destR.h ||
			caret->geometry->destR.x + caret->geometry->getW() <= geometry->destR.x || caret->geometry->destR.x >= geometry->destR.x + geometry->destR.w)
		{
			caret->visible = false;
		}
		else
		{
			caret->visible = true;
			// Mt_lib::confine(caret->geometry->destR.y, caret->geometry->srcR.y, caret->geometry->destR.h, caret->geometry->srcR.h, vspace, geometry->destR.y, geometry->destR.h);
			//  Mt_lib::confine(caret->geometry->destR.x, caret->geometry->srcR.x, caret->geometry->destR.w, caret->geometry->srcR.w, caret->geometry->getW(), geometry->destR.x, geometry->destR.w);
			Mt_lib::confineX(caret->geometry, geometry->destR);
			Mt_lib::confineY(caret->geometry, geometry->destR);
		}
	}

	void updateLines()
	{
		int vspace = lines[0]->font->getH();

		for (size_t i = 0; i < lines.size(); ++i)
		{
			lines[i]->geometry->srcR.x = -geometry->srcR.x;

			lines[i]->geometry->destR.x = geometry->destR.x;
			lines[i]->geometry->destR.y = geometry->srcR.y + geometry->destR.y + (vspace * i);
			if (lines[i]->geometry->destR.y + vspace <= geometry->destR.y || lines[i]->geometry->destR.y >= geometry->destR.y + geometry->destR.h)
			{
				lines[i]->visible = false;
			}
			else
			{
				lines[i]->geometry->srcR.w = lines[i]->geometry->destR.w = std::min(geometry->srcR.x + lines[i]->geometry->getW(), geometry->destR.w);
				if (lines[i]->geometry->destR.w > 0)
				{
					lines[i]->visible = true;
					// Mt_lib::confine(lines[i]->geometry->destR.y, lines[i]->geometry->srcR.y, lines[i]->geometry->destR.h, lines[i]->geometry->srcR.h, vspace, geometry->destR.y, geometry->destR.h);
					Mt_lib::confineY(lines[i]->geometry, geometry->destR);
				}
			}
		}

		updateCaretPosition();
	}

	Mt_label *newLine(const std::string &content = "")
	{
		Mt_label *line = nullptr;
		if (lines.size() > 0)
		{
			line = *lines.insert(lines.begin() + (caretPos_y + 1), &Mt_label::create(*this));
		}
		else
		{
			line = *lines.insert(lines.begin(), &Mt_label::create(*this));
		}

		line->text = content;
		line->autoupdate = false;

		return line;
	}

	void handleEvent() override
	{
		return_if(!visible);

		if (window.event.type == SDL_WINDOWEVENT)
		{
			switch (window.event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			{
				onWindowResized();
				break;
			}
			}
		}
		if (focused)
		{
			switch (window.event.type)
			{
			case SDL_KEYDOWN:
			{
				switch (window.event.key.keysym.sym)
				{
				case SDLK_c:
					if (SDL_GetModState() & KMOD_CTRL)
					{
						SDL_SetClipboardText(str().c_str());
					}
					break;
				} // switch (window.event.key.keysym.sym)
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				const short pixels = 7;
				if (SDL_GetModState() & KMOD_SHIFT)
				{
					if (window.event.wheel.y > 0)
					{
						geometry->srcR.x = std::min(0, geometry->srcR.x + pixels);
					}
					else if (window.event.wheel.y < 0)
					{
						geometry->srcR.x -= pixels;
					}
				}
				else
				{
					if (window.event.wheel.y > 0)
					{
						geometry->srcR.y = std::min(0, geometry->srcR.y + pixels);
					}
					else if (window.event.wheel.y < 0)
					{
						geometry->srcR.y -= pixels;
					}
				}
				break;
			}
			} // switch (window.event.type)

			if (editable)
			{
				switch (window.event.type)
				{
				case SDL_KEYDOWN:
				{
					switch (window.event.key.keysym.sym)
					{
					case SDLK_UP:
						if (caretPos_y > 0)
						{
							caretPos_y--;
							input = lines[caretPos_y];
							caretPos_x = std::min(input->text.length(), caretPos_x);
							caret->geometry->destR.y = input->geometry->destR.y;
						}
						pointCursor();
						break;
					case SDLK_LEFT:
						if (caretPos_x > 0)
						{
							caretPos_x--;
						}
						pointCursor();
						break;
					case SDLK_DOWN:
						if (caretPos_y + 1 < lines.size())
						{
							caretPos_y++;
							input = lines[caretPos_y];
							caretPos_x = std::min(input->text.length(), caretPos_x);
							caret->geometry->destR.y = input->geometry->destR.y;
						}
						pointCursor();
						break;
					case SDLK_RIGHT:
						if (caretPos_x + 1 <= input->text.length())
						{
							caretPos_x++;
						}
						pointCursor();
						break;

					case SDLK_HOME:
						caretPos_x = 0;
						geometry->srcR.x = 0;
						pointCursor();
						break;
					case SDLK_END:
						caretPos_x = input->text.length();
						if (input->geometry->destR.w >= geometry->destR.w)
							geometry->srcR.x = input->geometry->destR.w - geometry->destR.w;
						pointCursor();
						break;

					case SDLK_BACKSPACE:
						if (caretPos_x == 0 && caretPos_y > 0)
						{
							caretPos_y--;
							caretPos_x = lines[caretPos_y]->text.length();
							lines[caretPos_y]->text += input->text;
							input = lines[caretPos_y];
							lines.erase(lines.begin() + caretPos_y + 1);

							input->update();
							onTextModified();
						}
						else if (input->text.length() > 0 && caretPos_x > 0)
						{
							caretPos_x--;
							input->text.erase(input->text.begin() + caretPos_x);

							input->update();
							onTextModified();
						}
						pointCursor();
						break;
					case SDLK_DELETE:
						if (caretPos_x == input->text.length() && caretPos_y < lines.size() - 1)
						{
							input->text += lines[caretPos_y + 1]->text;
							lines.erase(lines.begin() + caretPos_y + 1);

							input->update();
							onTextModified();
						}
						else if (input->text.length() > 0 && caretPos_x < input->text.length())
						{
							input->text.erase(input->text.begin() + caretPos_x);

							input->update();
							onTextModified();
						}
						pointCursor();
						break;

					case SDLK_RETURN:
					{
						std::string text(input->text.substr(caretPos_x));
						input->text.erase(caretPos_x);
						input->update();

						input = newLine(text);

						caretPos_x = 0;
						caretPos_y++;

						onTextModified();
						pointCursor();

						break;
					}
					case SDLK_TAB:
					{
						input->text.insert(caretPos_x, "    ");
						caretPos_x += 4;

						onTextModified();
						pointCursor();
						break;
					}
					case SDLK_c:
						if (SDL_GetModState() & KMOD_CTRL)
							SDL_SetClipboardText(str().c_str());
						break;
					case SDLK_v:
					{
						if (SDL_GetModState() & KMOD_CTRL)
						{
							const char *text = SDL_GetClipboardText();
							std::string ftext(text);
							Mt_lib::replaceAll(ftext, "\t", "    ");
							str(ftext);
							delete[] text;

							onTextModified();
							pointCursor();
						}
						break;
					}
					} // switch (window.event.key.keysym.sym)
					break;
				}
				case SDL_TEXTINPUT:
				{
					std::string text(window.event.text.text);
					if (!(SDL_GetModState() & KMOD_CTRL && (text[0] == 'c' || text[0] == 'C' || text[0] == 'v' || text[0] == 'V')))
					{
						input->text.insert(caretPos_x, text);
						input->update();

						caretPos_x += text.length();

						onTextModified();
						pointCursor();
					}
					break;
				}
				} // switch (window.event.type)
			}
		}

		if (Mt_vector<int>::mousePos().intercept(geometry->destR))
		{
			onHover();
			SetCursor(SDL_SYSTEM_CURSOR_IBEAM);
			if (window.hovering == nullptr)
			{
				window.hovering = this;
				color.fadeInto(&hover_color);
				frameColor.fadeInto(&frame_hover_color);
			}
			if (!focused)
			{
				if (window.event.type == SDL_MOUSEBUTTONDOWN)
				{
					switch (window.event.button.button)
					{
					case SDL_BUTTON_LEFT:
						onMouseDown();
						if (window.hovering == this)
						{
							onFocus();
							color.fadeInto(&focused_color);
							frameColor.fadeInto(&frame_focused_color);
							focused = true;
							released = false;
						}
						break;
					default:
						break;
					}
				}
			}
		}
		else if (window.event.type == SDL_MOUSEBUTTONDOWN && released)
		{
			SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			switch (window.event.button.button)
			{
			case SDL_BUTTON_LEFT:
				onLostFocus();
				color.fadeInto(&normal_color);
				frameColor.fadeInto(&frame_normal_color);
				focused = false;
				break;
			}
		}
		else if (!focused)
		{
			SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			onMouseLeave();
			if (window.hovering == this)
			{
				color.fadeInto(&normal_color);
				frameColor.fadeInto(&frame_normal_color);
				window.hovering = nullptr;
			}
		}
		else if (released)
		{
			SetCursor(SDL_SYSTEM_CURSOR_ARROW);
		}

		if (window.event.type == SDL_MOUSEBUTTONUP)
		{
			switch (window.event.button.button)
			{
			case SDL_BUTTON_LEFT:
				onMouseUp();
				released = true;
				break;
			default:
				break;
			}
		}
	}

	void update() override
	{
		return_if(!visible);

		color.update();
		frameColor.update();
		updateLines();

		for (auto label : lines)
			label->update();

		if (focused && editable)
			caret->update();
	}

	void draw() override
	{
		return_if(!visible);

		Mt_lib::drawFillRectangle(window.renderer, geometry->destR, color.color);
		Mt_lib::drawRectangle(window.renderer, geometry->destR, frameColor.color);

		for (auto label : lines)
			label->draw();

		if (focused && editable)
			caret->draw();
	}
};

#endif /* C26CC945_6DCC_4AC9_AE1F_9F84A853E863 */
