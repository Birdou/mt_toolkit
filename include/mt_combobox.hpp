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

	int x, y, w, h;

public:
	Mt_textbox *textbox = nullptr;
	Mt_button *button = nullptr;
	Mt_comboBox(Mt_application &application, int x, int y, int w, int h, int fontSize, SDL_Color textColor) : Mt_widget(application), x(x), y(y), w(w), h(h)
	{
		int tbox_x = x - (h / 2);
		int btn_x = x + ((w - h) / 2);

		textbox = new Mt_textbox(application, tbox_x, y, w - h, h, fontSize, textColor);
		button = new Mt_button(application, btn_x, y, h, h, "\u25BC",
							   [this]()
							   {
								   show = !show;
							   });
	}
	~Mt_comboBox()
	{
		delete textbox;
		delete button;
	}

	void addOption(const std::string &string, std::function<void()> function)
	{
		int ny = y + (h / 2);
		if (options.size() != 0)
		{
			auto last = options.back().second;
			ny = last->getH() + last->getY();
		}
		Mt_button *button = new Mt_button(application, x, 0, w, h, string, function);
		button->fitH(10);
		button->setY(ny);
		options.emplace_back(std::pair<std::string, Mt_button *>(string, button));
	}
	std::string option()
	{
		return textbox->str();
	}

	void handleEvents() override
	{
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
		textbox->update();
		button->update();
		if (show)
		{
			Mt_vector<int> mouse(Mt_vector<int>::mousePos());
			auto first = options.front().second;
			auto last = options.back().second;
			int x = first->getX();
			int y = first->getY();
			int w = first->getW();
			int h = (last->getY() + last->getH()) - y;
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
		textbox->draw();
		button->draw();
		if (show)
		{
			for (auto btn : options)
			{
				btn.second->draw();
			}
		}
		//Mt_lib::drawRectangle(application.renderer, destR, Mt_lib::color(255, 0, 0));
	}
};

#endif /* BB5DE797_DB7E_4395_ACCD_25EE508CE326 */
