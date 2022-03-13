
#include "mt_textinput.hpp"

Mt_textinput::Mt_textinput(Mt_widget &widget) : Mt_widget(widget)
{
	init();
}
Mt_textinput::Mt_textinput(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	init();
}

void Mt_textinput::init()
{
	caret = &Mt_caret::create(*this);

	input = &Mt_label::create(*this);
	input->autoupdate = false;

	scheme = UI_TEXTINPUT_COLOR_SCHEME;

	backgroundColor = scheme.background.normal;
	borderColor = scheme.border.normal;
}

void Mt_textinput::updateCaretPosition()
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

void Mt_textinput::pointCursor()
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

void Mt_textinput::copy()
{
	if (SDL_GetModState() & KMOD_CTRL)
	{
		SDL_SetClipboardText(str().c_str());
	}
}
void Mt_textinput::paste()
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
}
void Mt_textinput::wheel()
{
}
void Mt_textinput::enter()
{
}
void Mt_textinput::backspace()
{
	if (input->text.size() > 0 && caretPos_x > 0)
	{
		caretPos_x--;
		input->geometry->srcR.x -= window.renderer->substrWidth(font->getFont(), input->text, caretPos_x, 1);
		if (input->geometry->srcR.x < 0)
			input->geometry->srcR.x = 0;
		input->text.erase(input->text.begin() + caretPos_x);

		onTextModified();
		pointCursor();
	}
}
void Mt_textinput::del()
{
	if (input->text.size() > 0 && caretPos_x < input->text.length())
	{
		input->text.erase(input->text.begin() + caretPos_x);

		onTextModified();
		pointCursor();
	}
}
void Mt_textinput::up()
{
}
void Mt_textinput::left()
{
	if (caretPos_x > 0)
	{
		caretPos_x--;
	}
	pointCursor();
}
void Mt_textinput::right()
{
	if (caretPos_x + 1 <= input->text.length())
	{
		caretPos_x++;
	}
	pointCursor();
}
void Mt_textinput::down()
{
}
void Mt_textinput::end()
{
	caretPos_x = input->text.length();
	if (input->geometry->destR.w >= geometry->destR.w)
		text_x = input->geometry->destR.w - geometry->destR.w;
	pointCursor();
}
void Mt_textinput::home()
{
	caretPos_x = 0;
	text_x = 0;
	pointCursor();
}
void Mt_textinput::tab()
{
}
void Mt_textinput::textInput()
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
Mt_textinput::~Mt_textinput()
{
	Debug("Destroying textinput...");
	if (caret)
		delete caret;
	if (input)
		delete input;
	Debug("Done.");
}

std::string Mt_textinput::str() const
{
	return input->text;
}
void Mt_textinput::str(const std::string &text)
{
	input->text = text;

	caretPos_x = input->text.length();

	onTextModified();
}

void Mt_textinput::handleMouse()
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
			backgroundColor.fadeInto(scheme.background.hover);
			borderColor.fadeInto(scheme.border.hover);
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
						backgroundColor.fadeInto(scheme.background.focused);
						borderColor.fadeInto(scheme.border.focused);
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
			backgroundColor.fadeInto(scheme.background.normal);
			borderColor.fadeInto(scheme.border.normal);
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
			backgroundColor.fadeInto(scheme.background.normal);
			borderColor.fadeInto(scheme.border.normal);
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

void Mt_textinput::handleEvent()
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

void Mt_textinput::update()
{
	return_if(!visible);

	handleMouse();
	input->update();

	if (focused && editable)
		caret->update();

	input->geometry->srcR.x = -text_x;

	input->geometry->destR.x = geometry->destR.x;
	input->geometry->destR.y = text_y + (geometry->destR.y - geometry->srcR.y);

	input->geometry->srcR.w = input->geometry->destR.w = std::min(text_x + input->geometry->getW(), geometry->destR.w);
	if (input->geometry->destR.w > 0)
	{
		input->visible = true;
		input->geometry->confineY(geometry->destR);
	}

	updateCaretPosition();
}

void Mt_textinput::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	input->draw();

	if (focused && editable)
		caret->draw();
}

// ANCHOR TEXT BOX CLASS
Mt_textbox::Mt_textbox(Mt_widget &widget) : Mt_textinput(widget)
{
}
Mt_textbox::Mt_textbox(Mt_window &window, int x, int y, int w, int h) : Mt_textinput(window, x, y, w, h)
{
}

Mt_textbox &Mt_textbox::create(Mt_widget &widget)
{
	return *(new Mt_textbox(widget));
}
Mt_textbox &Mt_textbox::create(Mt_window &window, int x, int y, int w, int h)
{
	return *(new Mt_textbox(window, x, y, w, h));
}

Mt_textbox::~Mt_textbox()
{
	Debug("Destroying textbox...");
}

// ANCHOR TEXT AREA CLASS
Mt_textarea::Mt_textarea(Mt_window &window, int x, int y, int w, int h) : Mt_textinput(window, x, y, w, h)
{
	lines.emplace_back(input);
	pointCursor();
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

void Mt_textarea::wheel()
{
	const short pixels = 7;
	if (SDL_GetModState() & KMOD_SHIFT)
	{
		if (window.event.wheel.y > 0)
		{
			text_x = std::min(0, text_x + pixels);
		}
		else if (window.event.wheel.y < 0)
		{
			text_x -= pixels;
		}
	}
	else
	{
		if (window.event.wheel.y > 0)
		{
			text_y = std::min(0, text_y + pixels);
		}
		else if (window.event.wheel.y < 0)
		{
			text_y -= pixels;
		}
	}
}
void Mt_textarea::enter()
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
void Mt_textarea::backspace()
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
void Mt_textarea::del()
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
void Mt_textarea::up()
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
void Mt_textarea::down()
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
void Mt_textarea::tab()
{
	input->text.insert(caretPos_x, "    ");
	caretPos_x += 4;

	onTextModified();
	pointCursor();
}
Mt_textarea &Mt_textarea::create(Mt_window &window, int x, int y, int w, int h) { return *(new Mt_textarea(window, x, y, w, h)); }

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
void Mt_textarea::str(const std::string &str)
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

Mt_label *Mt_textarea::newLine(const std::string &content)
{
	Mt_label *line = *lines.insert(lines.begin() + (caretPos_y + 1), &Mt_label::create(*this));

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

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	for (auto label : lines)
		label->draw();

	if (focused && editable)
		caret->draw();
}