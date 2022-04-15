
#include "mt_textinput.hpp"

#include <iomanip>

// ANCHOR TEXT BOX CLASS
Mt_textbox::Mt_textbox(Mt_widget& widget) : Mt_widget(widget)
{
	init();
}
Mt_textbox::Mt_textbox(Mt_window& window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	init();
}
Mt_textbox& Mt_textbox::create(Mt_widget& widget)
{
	Mt_textbox* tbox = new Mt_textbox(widget);
	// widget.window.widgets.emplace_back(tbox);
	return *tbox;
}
Mt_textbox& Mt_textbox::create(Mt_window& window, int x, int y, int w, int h)
{
	Mt_textbox* tbox = new Mt_textbox(window, x, y, w, h);
	// window.widgets.emplace_back(tbox);:
	window.add(*tbox);
	return *tbox;
}

void Mt_textbox::init()
{
	caret = &Mt_caret::create(*this);

	input = &Mt_label::create(*this);
	input->autoupdate = false;

	scheme = UI_TEXTINPUT_COLOR_SCHEME;

	currentBackgroundColor = scheme.background.normal;
	currentBorderColor = scheme.border.normal;
}

void Mt_textbox::updateCaretPosition()
{
	if (caretPos_x > 0)
	{
		if (!password)
		{
			int tlen = window.renderer->substrWidth(input->font->getFont(), input->text, 0, caretPos_x);
			caret->geometry->destR.x = geometry->destR.x + tlen + text_x;
		}
		else
		{
			int tlen = window.renderer->substrWidth(input->font->getFont(), "\u25CF");
			caret->geometry->destR.x = geometry->destR.x + (tlen * caretPos_x);
		}
	}
	else
	{
		caret->geometry->destR.x = geometry->destR.x + 5;
	}

	int vspace = input->font->getH();
	caret->geometry->destR.y = input->geometry->destR.y - input->geometry->srcR.y;
	if (caret->geometry->destR.y + vspace <= geometry->destR.y || caret->geometry->destR.y >= geometry->destR.y + geometry->destR.h ||
		caret->geometry->destR.x + caret->geometry->getW() <= geometry->destR.x || caret->geometry->destR.x >= geometry->destR.x + geometry->destR.w)
	{
		caret->visible = false;
	}
	else
	{
		caret->visible = true;
		caret->geometry->confineX(geometry->destR);
		caret->geometry->confineY(geometry->destR);
	}
}

void Mt_textbox::pointCursor()
{
	updateCaretPosition();

	int xw = geometry->destR.x + geometry->destR.w;
	int cxw = caret->geometry->destR.x + caret->geometry->destR.w;
	if (input->geometry->getW() > geometry->destR.w && cxw > xw)
	{
		text_x -= cxw - xw;
	}
	else if (caret->geometry->destR.x - 25 < geometry->destR.x && text_x < 0)
	{
		text_x += caret->geometry->destR.x - geometry->destR.x;
	}
	text_x = std::min(0, text_x);

	updateCaretPosition();
}

Mt_textbox::~Mt_textbox()
{
	Debug("Destroying textinput...");
	if (caret)
		delete caret;
	if (input)
		delete input;
	Debug("Done.");
}

std::string Mt_textbox::str() const
{
	return input->text;
}
void Mt_textbox::str(const std::string& text)
{
	input->text = text;

	caretPos_x = input->text.length();

	onTextModified();
}

void Mt_textbox::handleMouse()
{
	hoverScroll = false;
	if (window.hovering == this)
		window.hovering = nullptr;

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		onHover();
		hoverScroll = true;
		if (window.hovering == nullptr)
		{
			window.hovering = this;
			SetCursor(SDL_SYSTEM_CURSOR_IBEAM);
			fadeToHover();
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
						if (!SDL_IsTextInputActive())
						{
							Debug("Start text input");
							SDL_StartTextInput();
						}
						fadeToFocused();
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
	else if (window.event.type == SDL_MOUSEBUTTONDOWN && released && focused)
	{
		SetCursor(SDL_SYSTEM_CURSOR_ARROW);
		switch (window.event.button.button)
		{
		case SDL_BUTTON_LEFT:
			onLostFocus();
			if (!dynamic_cast<Mt_textbox*>(window.hovering))
			{
				Debug("Stop text input");
				SDL_StopTextInput();
			}
			fadeToNormal();
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
			fadeToNormal();
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

void Mt_textbox::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);

	if (window.hovering == this && window.event.type == SDL_MOUSEWHEEL)
		wheel();

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
}

void Mt_textbox::update()
{
	return_if(!visible);

	handleMouse();
	if (password)
	{
		std::string text = input->text;
		std::stringstream stream;
		for (size_t i = 0; i < text.length(); i++)
			stream << "\u25CF";

		str(stream.str());
		input->update();
		str(text);
	}
	else
	{
		input->update();
	}

	if (focused && editable)
		caret->update();

	input->geometry->srcR.x = -text_x;

	input->geometry->destR.x = geometry->destR.x;
	input->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + ((geometry->getH() - input->geometry->getH()) / 2);

	input->geometry->srcR.w = input->geometry->destR.w = std::min(text_x + input->geometry->getW(), geometry->destR.w);
	if (input->geometry->destR.w > 0)
	{
		input->visible = true;
		input->geometry->confineY(geometry->destR);
	}

	updateCaretPosition();
}

void Mt_textbox::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

	input->draw();

	if (focused && editable)
		caret->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}

// ANCHOR TEXT AREA CLASS
Mt_textarea::Mt_textarea(Mt_window& window, int x, int y, int w, int h) : Mt_textbox(window, x, y, w, h)
{
	lines.emplace_back(input);
	wheel = [this]()
	{
		const short pixels = 7;
		if (SDL_GetModState() & KMOD_SHIFT)
		{
			if (this->window.event.wheel.y > 0)
			{
				text_x = std::min(0, text_x + pixels);
			}
			else if (this->window.event.wheel.y < 0)
			{
				text_x -= pixels;
			}
		}
		else
		{
			if (this->window.event.wheel.y > 0)
			{
				text_y = std::min(0, text_y + pixels);
			}
			else if (this->window.event.wheel.y < 0)
			{
				text_y -= pixels;
			}
		}
	};
	enter = [this]()
	{
		std::string text(input->text.substr(caretPos_x));
		input->text.erase(caretPos_x);
		input->update();

		input = newLine(text);

		caretPos_x = 0;
		caretPos_y++;

		onTextModified();
		pointCursor();
	};
	backspace = [this]()
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
	};
	del = [this]()
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
	};
	up = [this]()
	{
		if (caretPos_y > 0)
		{
			caretPos_y--;
			input = lines[caretPos_y];
			caretPos_x = std::min(input->text.length(), caretPos_x);
			caret->geometry->destR.y = input->geometry->destR.y;
		}
		pointCursor();
	};
	down = [this]()
	{
		if (caretPos_y + 1 < lines.size())
		{
			caretPos_y++;
			input = lines[caretPos_y];
			caretPos_x = std::min(input->text.length(), caretPos_x);
			caret->geometry->destR.y = input->geometry->destR.y;
		}
		pointCursor();
	};
	tab = [this]()
	{
		input->text.insert(caretPos_x, "    ");
		caretPos_x += 4;

		onTextModified();
		pointCursor();
	};
	pointCursor();
}
void Mt_textarea::updateCaretPosition()
{
	if (caretPos_x > 0)
	{
		int tlen = window.renderer->substrWidth(input->font->getFont(), input->text, 0, caretPos_x);
		caret->geometry->destR.x = geometry->destR.x + tlen + text_x;
	}
	else
	{
		caret->geometry->destR.x = geometry->destR.x;
	}

	int vspace = input->font->getH();
	caret->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + (vspace * caretPos_y) + text_y;
	if (caret->geometry->destR.y + vspace <= geometry->destR.y || caret->geometry->destR.y >= geometry->destR.y + geometry->destR.h ||
		caret->geometry->destR.x + caret->geometry->getW() <= geometry->destR.x || caret->geometry->destR.x >= geometry->destR.x + geometry->destR.w)
	{
		caret->visible = false;
	}
	else
	{
		caret->visible = true;
		caret->geometry->confineX(geometry->destR);
		caret->geometry->confineY(geometry->destR);
	}
}
void Mt_textarea::pointCursor()
{
	updateCaretPosition();
	if (caretPos_y == 0)
	{
		text_y = 0;
	}
	else if (caret->geometry->destR.y < geometry->destR.y)
	{
		text_y += geometry->destR.y - caret->geometry->destR.y;
	}
	else
	{
		int yh = geometry->destR.y + geometry->destR.h;
		int cyh = caret->geometry->destR.y + caret->geometry->getH();
		if (cyh >= yh)
		{
			text_y -= cyh - yh;
		}
	}

	int xw = geometry->destR.x + geometry->destR.w;
	int cxw = caret->geometry->destR.x + caret->geometry->destR.w;
	if (input->geometry->getW() > geometry->destR.w && cxw > xw)
	{
		text_x -= cxw - xw;
	}
	else if (caret->geometry->destR.x - 25 < geometry->destR.x && text_x < 0)
	{
		text_x += caret->geometry->destR.x - geometry->destR.x;
	}
	text_x = std::min(0, text_x);
	updateCaretPosition();
}

void Mt_textarea::updateLines()
{
	int vspace = lines[0]->font->getH();

	for (size_t i = 0; i < lines.size(); ++i)
	{
		lines[i]->geometry->srcR.x = -text_x;

		lines[i]->geometry->destR.x = geometry->destR.x;
		lines[i]->geometry->destR.y = text_y + (geometry->destR.y - geometry->srcR.y) + (vspace * i);
		if (lines[i]->geometry->destR.y + vspace <= geometry->destR.y || lines[i]->geometry->destR.y >= geometry->destR.y + geometry->destR.h)
		{
			lines[i]->visible = false;
		}
		else
		{
			lines[i]->geometry->srcR.w = lines[i]->geometry->destR.w = std::min(text_x + lines[i]->geometry->getW(), geometry->destR.w);
			if (lines[i]->geometry->destR.w > 0)
			{
				lines[i]->visible = true;
				lines[i]->geometry->confineY(geometry->destR);
			}
		}
	}

	updateCaretPosition();
}


Mt_textarea& Mt_textarea::create(Mt_window& window, int x, int y, int w, int h)
{
	Mt_textarea* tarea = new Mt_textarea(window, x, y, w, h);
	// window.widgets.emplace_back(tarea);
	window.add(*tarea);
	return *tarea;
}

Mt_textarea::~Mt_textarea()
{
	Debug("Destroying textarea...");

	for (auto line : lines)
		delete line;

	input = nullptr;

	Debug("Done.");
}

std::string Mt_textarea::str() const
{
	std::stringstream stream;
	for (auto line : lines)
	{
		stream << line->text << std::endl;
	}
	return stream.str();
}
void Mt_textarea::str(const std::string& str)
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

Mt_label* Mt_textarea::newLine(const std::string& content)
{
	Mt_label* line = *lines.insert(lines.begin() + (caretPos_y + 1), &Mt_label::create(*this));

	line->text = content;
	line->autoupdate = false;

	return line;
}

void Mt_textarea::update()
{
	return_if(!visible);

	handleMouse();
	updateLines();

	for (auto label : lines)
		label->update();

	if (focused && editable)
		caret->update();
}

void Mt_textarea::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

	for (auto label : lines)
		label->draw();

	if (focused && editable)
		caret->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}