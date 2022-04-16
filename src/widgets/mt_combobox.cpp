
#include "widgets/mt_combobox.hpp"

TOOLKIT_NAMESPACE::ComboBox::ComboBox(Window &window, int x, int y, int w, int h) : Widget(window, x, y, w, h)
{
	textbox = &Textbox::create(*this);
	textbox->geometry->setW(w - 17);
	textbox->geometry->setH(h);
	textbox->geometry->normalize();

	button = &Button::create(*this);
	button->geometry->setH(h);
	button->geometry->setW(17);
	button->geometry->normalize();
	button->label->text = "\u25BC";
	button->function = [this]()
	{
		turnVisible();
	};

	updatePosition();
}
void TOOLKIT_NAMESPACE::ComboBox::init()
{
}
void TOOLKIT_NAMESPACE::ComboBox::turnVisible()
{
	show = !show;
	for (auto btn : options)
		btn.second->visible = show;
}
TOOLKIT_NAMESPACE::ComboBox &TOOLKIT_NAMESPACE::ComboBox::create(Window &window, int x, int y, int w, int h)
{
	return *(new TOOLKIT_NAMESPACE::ComboBox(window, x, y, w, h));
}

TOOLKIT_NAMESPACE::ComboBox::~ComboBox()
{
	Debug("Destroying comboBox...");

	delete textbox;
	delete button;

	Debug("Done.");
}
void TOOLKIT_NAMESPACE::ComboBox::addOption(const std::string &string)
{
	auto button = &Button::create(*this);

	button->label->text = string;

	button->geometry->setW(geometry->getW());
	button->visible = show;

	options.emplace(string, button);
}

void TOOLKIT_NAMESPACE::ComboBox::updatePosition()
{
	textbox->geometry->destR.x = (geometry->destR.x - geometry->srcR.x);
	textbox->geometry->destR.y = (geometry->destR.y - geometry->srcR.y);
	textbox->geometry->confine(geometry->destR);

	button->geometry->destR.x = (geometry->destR.x - geometry->srcR.x) + textbox->geometry->getW();
	button->geometry->destR.y = (geometry->destR.y - geometry->srcR.y);
	button->geometry->confine(geometry->destR);
}

void TOOLKIT_NAMESPACE::ComboBox::updateOptions()
{
	int prev_y = geometry->destR.y + geometry->getH();
	for (auto option : options)
	{
		auto &button = option.second;

		button->geometry->destR.x = geometry->destR.x;
		button->geometry->destR.y = prev_y;

		button->fitH();
		button->geometry->normalize();

		prev_y = button->geometry->destR.y + button->geometry->destR.h;
	}
}

std::string TOOLKIT_NAMESPACE::ComboBox::option()
{
	return textbox->str();
}

void TOOLKIT_NAMESPACE::ComboBox::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);

	textbox->handleEvent();
	button->handleEvent();

	if (show)
	{
		for (auto btn : options)
			btn.second->handleEvent();
	}
}

void TOOLKIT_NAMESPACE::ComboBox::update()
{
	return_if(!visible);

	textbox->update();
	button->update();

	updatePosition();
	updateOptions();

	if (show)
	{
		for (auto btn : options)
			btn.second->update();
	}

	if (show && !options.empty())
	{
		Point mouse(Point::mousePos());
		auto &last = std::prev(options.end())->second;
		int x = geometry->destR.x;
		int y = geometry->destR.y;
		int w = geometry->destR.w;
		int h = (last->geometry->destR.y + last->geometry->destR.h) - y;
		if (mouse.x >= x + w || mouse.x < x ||
			mouse.y >= y + h || mouse.y < y)
		{
			if (window.event.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (window.event.button.button)
				{
				case SDL_BUTTON_LEFT:
					show = false;
					break;
				}
			}
		}
		for (auto btn : options)
		{
			if (btn.second->actioned())
			{
				window.hovering = nullptr;
				textbox->str(btn.first);
				turnVisible();
			}
		}
	}
}

void TOOLKIT_NAMESPACE::ComboBox::draw()
{
	return_if(!visible);

	textbox->draw();
	button->draw();

	if (show)
	{
		for (auto btn : options)
			btn.second->draw();
	}
}