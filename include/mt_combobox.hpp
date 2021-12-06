#ifndef BB5DE797_DB7E_4395_ACCD_25EE508CE326
#define BB5DE797_DB7E_4395_ACCD_25EE508CE326

#include <vector>

#include "mt_application.hpp"
#include "mt_widget.hpp"
#include "mt_textbox.hpp"
#include "mt_button.hpp"

class Mt_comboBox : public Mt_widget
{
private:
	std::vector<std::pair<std::string, Mt_button *>> options;

	bool show = false;

public:
	Mt_textbox *textbox = nullptr;
	Mt_button *button = nullptr;
	Mt_comboBox(Mt_application &application, int x, int y, int w, int h) : Mt_widget(application, x, y, w, h)
	{
		textbox = new Mt_textbox(*this);
		textbox->geometry->normalize();

		button = new Mt_button(*this);
		button->label->text = "\u25BC";
		button->setFunction(
			[this]()
			{
				show = !show;
			});

		updatePosition();
	}
	~Mt_comboBox()
	{
		delete textbox;
		delete button;
	}

	void addOption(const std::string &string, std::function<void()> function)
	{
		Mt_button *button = new Mt_button(*this);

		button->label->text = string;
		button->setFunction(function);

		button->geometry->setW(geometry->getW());

		options.emplace_back(std::pair<std::string, Mt_button *>(string, button));
	}

	void updatePosition()
	{
		textbox->geometry->destR.x = geometry->destR.x;
		textbox->geometry->destR.y = geometry->destR.y;
		textbox->geometry->destR.w = geometry->getW() - geometry->getH();
		textbox->geometry->destR.h = geometry->getH();

		button->geometry->destR.x = geometry->destR.x + textbox->geometry->destR.w;
		button->geometry->destR.y = geometry->destR.y;
		button->geometry->destR.w = button->geometry->destR.h = geometry->getH();
	}
	void updateOptions()
	{
		int prev_y = geometry->destR.y + geometry->getH();
		for (size_t i = 0; i < options.size(); ++i)
		{
			auto button = options[i].second;

			button->geometry->destR.x = geometry->destR.x;
			button->geometry->destR.y = prev_y;

			button->fitH();
			button->geometry->normalize();

			prev_y = button->geometry->destR.y + button->geometry->destR.h;
		}
	}

	std::string option()
	{
		return textbox->str();
	}

	void handleEvents() override
	{
		return_if(!visible);

		textbox->handleEvents();
		button->handleEvents();
		if (show)
		{
			for (auto btn : options)
			{
				btn.second->handleEvents();
			}
		}
	}
	void update() override
	{
		return_if(!visible);

		textbox->update();
		button->update();

		updatePosition();
		updateOptions();

		if (show)
		{
			Mt_vector<int> mouse(Mt_vector<int>::mousePos());
			auto last = options.back().second;
			int x = geometry->destR.x;
			int y = geometry->destR.y;
			int w = geometry->destR.w;
			int h = (last->geometry->destR.y + last->geometry->destR.h) - y;
			if (mouse.x >= x + w || mouse.x < x ||
				mouse.y >= y + h || mouse.y < y)
			{
				if (application.event.type == SDL_MOUSEBUTTONDOWN)
				{
					switch (application.event.button.button)
					{
					case SDL_BUTTON_LEFT:
						show = false;
						break;
					}
				}
			}
			for (auto btn : options)
			{
				btn.second->update();
				if (btn.second->actioned())
				{
					application.hovering = nullptr;
					textbox->str(btn.first);
					show = false;
				}
			}
		}
	}
	void draw() override
	{
		return_if(!visible);

		textbox->draw();
		button->draw();
		if (show)
		{
			for (auto btn : options)
			{
				btn.second->draw();
			}
		}
	}
};

#endif /* BB5DE797_DB7E_4395_ACCD_25EE508CE326 */
