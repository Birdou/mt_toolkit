
#include "mt_combobox.hpp"

Mt_comboBox::Mt_comboBox(Mt_window &window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	textbox = &Mt_textbox::create(*this);
	textbox->geometry->normalize();

	button = &Mt_button::create(*this);
	button->label->text = "\u25BC";
	button->function = [this]()
	{
		turnVisible();
	};

	updatePosition();
}
Mt_comboBox::Mt_comboBox(const Mt_comboBox &) = delete;

void Mt_comboBox::turnVisible()
{
	show = !show;
	for (auto btn : options)
		btn.second->visible = show;
}
Mt_comboBox &Mt_comboBox::create(Mt_window &window, int x, int y, int w, int h)
{
	return *(new Mt_comboBox(window, x, y, w, h));
}

Mt_comboBox::~Mt_comboBox()
{
	Debug("Destroying comboBox...");

	delete textbox;
	delete button;

	Debug("Done.");
}
void Mt_comboBox::addOption(const std::string &string)
{
	auto button = &Mt_button::create(*this);

	button->label->text = string;

	button->geometry->setW(geometry->getW());
	button->visible = show;

	options.emplace(string, button);
}

void Mt_comboBox::updatePosition()
{
	textbox->geometry->destR.x = geometry->destR.x;
	textbox->geometry->destR.y = geometry->destR.y;
	textbox->geometry->destR.w = geometry->getW() - geometry->getH();
	textbox->geometry->destR.h = geometry->getH();

	button->geometry->destR.x = geometry->destR.x + textbox->geometry->destR.w;
	button->geometry->destR.y = geometry->destR.y;
	button->geometry->destR.w = button->geometry->destR.h = geometry->getH();
}
void Mt_comboBox::updateOptions()
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

std::string Mt_comboBox::option()
{
	return textbox->str();
}

void Mt_comboBox::handleEvent()
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

void Mt_comboBox::update()
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
		Mt_vector<int> mouse(Mt_vector<int>::mousePos());
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

void Mt_comboBox::draw()
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