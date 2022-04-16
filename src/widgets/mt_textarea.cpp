
#include "widgets/mt_textarea.hpp"

#include <iomanip>

TOOLKIT_NAMESPACE::Textarea::Textarea(Window &window, int x, int y, int w, int h) : Textbox(window, x, y, w, h)
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
void TOOLKIT_NAMESPACE::Textarea::updateCaretPosition()
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
void TOOLKIT_NAMESPACE::Textarea::pointCursor()
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

void TOOLKIT_NAMESPACE::Textarea::updateLines()
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

TOOLKIT_NAMESPACE::Textarea &TOOLKIT_NAMESPACE::Textarea::create(Window &window, int x, int y, int w, int h)
{
	TOOLKIT_NAMESPACE::Textarea *tarea = new TOOLKIT_NAMESPACE::Textarea(window, x, y, w, h);
	// window.widgets.emplace_back(tarea);
	window.add(*tarea);
	return *tarea;
}

TOOLKIT_NAMESPACE::Textarea::~Textarea()
{
	Debug("Destroying textarea...");

	for (auto line : lines)
		delete line;

	input = nullptr;

	Debug("Done.");
}

std::string TOOLKIT_NAMESPACE::Textarea::str() const
{
	std::stringstream stream;
	for (auto line : lines)
	{
		stream << line->text << std::endl;
	}
	return stream.str();
}
void TOOLKIT_NAMESPACE::Textarea::str(const std::string &str)
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

TOOLKIT_NAMESPACE::Label *TOOLKIT_NAMESPACE::Textarea::newLine(const std::string &content)
{
	Label *line = *lines.insert(lines.begin() + (caretPos_y + 1), &Label::create(*this));

	line->text = content;
	line->autoupdate = false;

	return line;
}

void TOOLKIT_NAMESPACE::Textarea::update()
{
	return_if(!visible);

	handleMouse();
	updateLines();

	for (auto label : lines)
		label->update();

	if (focused && editable)
		caret->update();
}

void TOOLKIT_NAMESPACE::Textarea::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

	for (auto label : lines)
		label->draw();

	if (focused && editable)
		caret->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}