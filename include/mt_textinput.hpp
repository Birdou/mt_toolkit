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

class Mt_textinput : public Mt_widget
{
protected:
	Mt_label *input = nullptr;
	Mt_caret *caret = nullptr;

	size_t caretPos_x = 0;
	size_t caretPos_y = 0;

	Mt_textinput(Mt_widget &widget) : Mt_widget(widget)
	{
		init();
	}
	Mt_textinput(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
	{
		init();
	}

	void init()
	{
		caret = &Mt_caret::create(*this);

		input = &Mt_label::create(*this);
		input->autoupdate = false;

		backgroundColor.color = normal_color;
		borderColor.color = frame_normal_color;
	}

	virtual void updateCaretPosition()
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
			Mt_lib::confineX(caret->geometry, geometry->destR);
			Mt_lib::confineY(caret->geometry, geometry->destR);
		}
	}

	virtual void pointCursor()
	{
		updateCaretPosition();

		int xw = geometry->destR.x + geometry->destR.w;
		int cxw = caret->geometry->destR.x + caret->geometry->destR.w;
		if (input->geometry->getW() > geometry->destR.w && cxw > xw)
		{
			geometry->srcR.x -= cxw - xw;
		}
		else if (caret->geometry->destR.x - 25 < geometry->destR.x && geometry->srcR.x < 0)
		{
			geometry->srcR.x += caret->geometry->destR.x - geometry->destR.x;
		}
		geometry->srcR.x = std::min(0, geometry->srcR.x);

		updateCaretPosition();
	};

	virtual void copy()
	{
		if (SDL_GetModState() & KMOD_CTRL)
		{
			SDL_SetClipboardText(str().c_str());
		}
	};
	virtual void paste()
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
	};
	virtual void wheel()
	{
	}
	virtual void enter()
	{
	}
	virtual void backspace()
	{
		if (input->text.size() > 0 && caretPos_x > 0)
		{
			caretPos_x--;
			input->geometry->srcR.x -= Mt_lib::substrWidth(window.renderer, font->getFont(), input->text, caretPos_x, 1);
			if (input->geometry->srcR.x < 0)
				input->geometry->srcR.x = 0;
			input->text.erase(input->text.begin() + caretPos_x);

			onTextModified();
			pointCursor();
		}
	};
	virtual void del()
	{
		if (input->text.size() > 0 && caretPos_x < input->text.length())
		{
			input->text.erase(input->text.begin() + caretPos_x);

			onTextModified();
			pointCursor();
		}
	};
	virtual void up()
	{
	}
	virtual void left()
	{
		if (caretPos_x > 0)
		{
			caretPos_x--;
		}
		pointCursor();
	};
	virtual void right()
	{
		if (caretPos_x + 1 <= input->text.length())
		{
			caretPos_x++;
		}
		pointCursor();
	};
	virtual void down()
	{
	}
	virtual void end()
	{
		caretPos_x = input->text.length();
		if (input->geometry->destR.w >= geometry->destR.w)
			geometry->srcR.x = input->geometry->destR.w - geometry->destR.w;
		pointCursor();
	};
	virtual void home()
	{
		caretPos_x = 0;
		geometry->srcR.x = 0;
		pointCursor();
	};
	virtual void tab()
	{
	}
	virtual void textInput()
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
	}

public:
	bool editable = true;

	SDL_Color normal_color = {255, 255, 255, 255};
	SDL_Color focused_color = {255, 255, 255, 255};
	SDL_Color hover_color = {255, 255, 255, 255};
	SDL_Color frame_normal_color = {122, 122, 122, 255};
	SDL_Color frame_focused_color = {0, 120, 215, 255};
	SDL_Color frame_hover_color = {23, 23, 23, 255};

	virtual ~Mt_textinput()
	{
		Debug("Destroying textinput...");
		if (caret)
			delete caret;
		if (input)
			delete input;
		Debug("Done.");
	}

	virtual std::string str() const
	{
		return input->text;
	}
	virtual void str(const std::string &text)
	{
		input->text = text;

		caretPos_x = input->text.length();

		onTextModified();
	}

	void handleEvent() override
	{
		HANDLE_WINDOW_EVENTS;

		return_if(!visible);

		if (focused)
		{
			switch (window.event.type)
			{
			case SDL_KEYDOWN:
				switch (window.event.key.keysym.sym)
				{
				case SDLK_c:
					copy();
					break;
				}
				break;
			case SDL_MOUSEWHEEL:
				wheel();
				break;
			}

			if (editable)
			{
				switch (window.event.type)
				{
				case SDL_KEYDOWN:
					switch (window.event.key.keysym.sym)
					{
					case SDLK_UP:
						up();
						break;
					case SDLK_LEFT:
						left();
						break;
					case SDLK_DOWN:
						down();
						break;
					case SDLK_RIGHT:
						right();
						break;
					case SDLK_HOME:
						home();
						break;
					case SDLK_END:
						end();
						break;
					case SDLK_BACKSPACE:
						backspace();
						break;
					case SDLK_DELETE:
						del();
						break;
					case SDLK_RETURN:
						enter();
						break;
					case SDLK_TAB:
						tab();
						break;
					case SDLK_v:
						paste();
						break;
					}
					break;
				case SDL_TEXTINPUT:
					textInput();
					break;
				}
			}
		}

		if (Mt_vector<int>::mousePos().intercept(geometry->destR))
		{
			onHover();
			SetCursor(SDL_SYSTEM_CURSOR_IBEAM);
			if (window.hovering == nullptr)
			{
				window.hovering = this;
				backgroundColor.fadeInto(&hover_color);
				borderColor.fadeInto(&frame_hover_color);
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
							backgroundColor.fadeInto(&focused_color);
							borderColor.fadeInto(&frame_focused_color);
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
				backgroundColor.fadeInto(&normal_color);
				borderColor.fadeInto(&frame_normal_color);
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
				backgroundColor.fadeInto(&normal_color);
				borderColor.fadeInto(&frame_normal_color);
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

		backgroundColor.update();
		borderColor.update();
		input->update();

		if (focused && editable)
			caret->update();

		input->geometry->srcR.x = -geometry->srcR.x;

		input->geometry->destR.x = geometry->destR.x;
		input->geometry->destR.y = geometry->srcR.y + geometry->destR.y;

		input->geometry->srcR.w = input->geometry->destR.w = std::min(geometry->srcR.x + input->geometry->getW(), geometry->destR.w);
		if (input->geometry->destR.w > 0)
		{
			input->visible = true;
			Mt_lib::confineY(input->geometry, geometry->destR);
		}

		updateCaretPosition();
	}

	void draw() override
	{
		return_if(!visible);

		Mt_lib::drawFillRectangle(window.renderer, geometry->destR, backgroundColor.color);
		Mt_lib::drawRectangle(window.renderer, geometry->destR, borderColor.color);

		input->draw();

		if (focused && editable)
			caret->draw();
	}
};

class Mt_textarea : public Mt_textinput
{
private:
	std::vector<Mt_label *> lines;

	Mt_textarea(Mt_window &window, int x, int y, int w, int h) : Mt_textinput(window, x, y, w, h)
	{
		lines.emplace_back(input);
		pointCursor();
	}
	Mt_textarea(const Mt_textarea &) = delete;

	void pointCursor() override
	{
		updateCaretPosition();
		if (caretPos_y == 0)
		{
			geometry->srcR.y = 0;
		}
		else if (caret->geometry->destR.y < geometry->destR.y)
		{
			geometry->srcR.y += geometry->destR.y - caret->geometry->destR.y;
		}
		else
		{
			int yh = geometry->destR.y + geometry->destR.h;
			int cyh = caret->geometry->destR.y + caret->geometry->getH();
			if (cyh >= yh)
			{
				geometry->srcR.y -= cyh - yh;
			}
		}

		int xw = geometry->destR.x + geometry->destR.w;
		int cxw = caret->geometry->destR.x + caret->geometry->destR.w;
		if (input->geometry->getW() > geometry->destR.w && cxw > xw)
		{
			geometry->srcR.x -= cxw - xw;
		}
		else if (caret->geometry->destR.x - 25 < geometry->destR.x && geometry->srcR.x < 0)
		{
			geometry->srcR.x += caret->geometry->destR.x - geometry->destR.x;
		}
		geometry->srcR.x = std::min(0, geometry->srcR.x);
		updateCaretPosition();
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
					Mt_lib::confineY(lines[i]->geometry, geometry->destR);
				}
			}
		}

		updateCaretPosition();
	}

	void wheel() override
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
	}
	void enter() override
	{
		std::string text(input->text.substr(caretPos_x));
		input->text.erase(caretPos_x);
		input->update();

		input = newLine(text);

		caretPos_x = 0;
		caretPos_y++;

		onTextModified();
		pointCursor();
	}
	void backspace() override
	{
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
	}
	void del() override
	{
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
	}
	void up() override
	{
		if (caretPos_y > 0)
		{
			caretPos_y--;
			input = lines[caretPos_y];
			caretPos_x = std::min(input->text.length(), caretPos_x);
			caret->geometry->destR.y = input->geometry->destR.y;
		}
		pointCursor();
	}
	void down() override
	{
		if (caretPos_y + 1 < lines.size())
		{
			caretPos_y++;
			input = lines[caretPos_y];
			caretPos_x = std::min(input->text.length(), caretPos_x);
			caret->geometry->destR.y = input->geometry->destR.y;
		}
		pointCursor();
	}
	void tab() override
	{
		input->text.insert(caretPos_x, "    ");
		caretPos_x += 4;

		onTextModified();
		pointCursor();
	}

public:
	static Mt_textarea &create(Mt_window &window, int x, int y, int w, int h) { return *(new Mt_textarea(window, x, y, w, h)); }

	~Mt_textarea()
	{
		Debug("Destroying textarea...");

		for (auto line : lines)
			delete line;

		input = nullptr;

		Debug("Done.");
	}

	std::string str() const override
	{
		std::stringstream stream;
		for (auto line : lines)
		{
			stream << line->text << std::endl;
		}
		return stream.str();
	}
	void str(const std::string &str) override
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

	Mt_label *newLine(const std::string &content = "")
	{
		Mt_label *line = *lines.insert(lines.begin() + (caretPos_y + 1), &Mt_label::create(*this));

		line->text = content;
		line->autoupdate = false;

		return line;
	}

	void update() override
	{
		return_if(!visible);

		backgroundColor.update();
		borderColor.update();
		updateLines();

		for (auto label : lines)
			label->update();

		if (focused && editable)
			caret->update();
	}

	void draw() override
	{
		return_if(!visible);

		Mt_lib::drawFillRectangle(window.renderer, geometry->destR, backgroundColor.color);
		Mt_lib::drawRectangle(window.renderer, geometry->destR, borderColor.color);

		for (auto label : lines)
			label->draw();

		if (focused && editable)
			caret->draw();
	}
};

class Mt_textbox : public Mt_textinput
{
private:
	Mt_textbox(Mt_widget &widget) : Mt_textinput(widget)
	{
	}
	Mt_textbox(Mt_window &window, int x, int y, int w, int h) : Mt_textinput(window, x, y, w, h)
	{
	}
	Mt_textbox(const Mt_textbox &) = delete;

public:
	static Mt_textbox &create(Mt_widget &widget) { return *(new Mt_textbox(widget)); }
	static Mt_textbox &create(Mt_window &window, int x, int y, int w, int h) { return *(new Mt_textbox(window, x, y, w, h)); }

	~Mt_textbox()
	{
		Debug("Destroying textbox...");
	}
};

#endif /* C26CC945_6DCC_4AC9_AE1F_9F84A853E863 */
