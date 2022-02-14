#ifndef E16839BE_0A2F_4866_9CD5_6C81E791C128
#define E16839BE_0A2F_4866_9CD5_6C81E791C128

#include "mt_core.hpp"

#include "mt_widget.hpp"
#include "mt_button.hpp"

#include <vector>

class Mt_scrollarea : public Mt_widget
{
private:
	std::vector<Mt_widget *> widgets;

	SDL_Rect scroll;

	Mt_button *up = nullptr;
	Mt_button *down = nullptr;
	Mt_button *scrollbar = nullptr;

	Mt_scrollarea(Mt_window &window, int x, int y, int w, int h, int scroll_w, int scroll_h) : Mt_widget(window, x, y, w, h)
	{
		init();
		up = &Mt_button::create(*this);
		up->geometry->setW(15);
		up->geometry->setH(17);

		up->geometry->normalize();
		up->label->text = BLACK_UP_POINTING_TRIANGLE;
		up->onClicked = [&]()
		{
			if (scroll.y > 0)
				scroll.y -= 1;
			progress = scroll.y / (float)scroll.h;
		};

		down = &Mt_button::create(*this);
		down->geometry->setW(15);
		down->geometry->setH(17);

		down->geometry->normalize();
		down->label->text = BLACK_DOWN_POINTING_TRIANGLE;
		down->onClicked = [&]()
		{
			if (scroll.y < scroll.h)
				scroll.y += 1;
			progress = scroll.y / (float)scroll.h;
		};

		scroll.w = scroll_w;
		scroll.h = scroll_h;

		scrollbar = &Mt_button::create(*this);
		scrollbar->geometry->setW(15);
		scrollbar->geometry->setH((h / (float)(h + scroll_h)) * (h - (17 * 2)));
		scrollbar->geometry->normalize();
		scrollbar->geometry->destR.y = (geometry->destR.y - geometry->srcR.y) + 17;
		scrollbar->onClicked = [&]()
		{
			if (window.event.type == SDL_MOUSEMOTION)
			{
				updateScrollBar();
			}
		};
	}

	float progress = 0.f;
	void updateScrollBar()
	{
		const int difY = geometry->destR.y - geometry->srcR.y;
		const int minpos = difY + 17;
		const int maxpos = difY + geometry->getH() - 17 - scrollbar->geometry->getH();
		const int y = (minpos + (maxpos - minpos) * progress) + window.event.motion.yrel;

		int pos = y - minpos;
		progress = pos / (float)(maxpos - minpos);
		progress = std::fmax(0, progress);
		progress = std::fmin(progress, 1);
		scroll.y = progress * scroll.h;
	}

	Mt_scrollarea(const Mt_scrollarea &) = delete;

	void init()
	{
		backgroundColor = Mt_RGBA(0, 0, 0, 0);
		borderColor = Mt_RGBA(130, 145, 144);
	}

public:
	static Mt_scrollarea &create(Mt_window &window, int x, int y, int w, int h, int scroll_w, int scroll_h) { return *new Mt_scrollarea(window, x, y, w, h, scroll_w, scroll_h); }

	void addWidget(Mt_widget &widget)
	{
		widgets.emplace_back(&widget);
	}

	void handleEvent() override
	{
		up->handleEvent();
		down->handleEvent();
		scrollbar->handleEvent();
	} // TODO SCROLL BAR

	void confineObject(Mt_widget *widget)
	{
		if (widget->geometry->destR.x + widget->geometry->getW() <= geometry->destR.x ||
			widget->geometry->destR.y + widget->geometry->getH() <= geometry->destR.y ||
			widget->geometry->destR.x >= geometry->destR.x + geometry->destR.w ||
			widget->geometry->destR.y >= geometry->destR.y + geometry->destR.h)
		{
			widget->visible = false;
		}
		else
		{
			widget->visible = true;
			Mt_lib::confineX(widget->geometry, geometry->destR);
			Mt_lib::confineY(widget->geometry, geometry->destR);
		}
	}

	void update() override
	{
		up->update();
		down->update();
		scrollbar->update();

		const int difX = geometry->destR.x - geometry->srcR.x;
		const int difY = geometry->destR.y - geometry->srcR.y;

		up->geometry->destR.x = difX + geometry->getW() - up->geometry->getW();
		up->geometry->destR.y = difY;
		confineObject(up);
		down->geometry->destR.x = difX + geometry->getW() - down->geometry->getW();
		down->geometry->destR.y = difY + geometry->getH() - down->geometry->getH();
		confineObject(down);

		const int minpos = difY + 17;
		const int maxpos = difY + geometry->getH() - 17 - scrollbar->geometry->getH();

		scrollbar->geometry->destR.x = difX + geometry->getW() - scrollbar->geometry->getW();
		scrollbar->geometry->destR.y = minpos + (maxpos - minpos) * progress;
		confineObject(scrollbar);

		std::cout << progress << std::endl;

		for (auto widget : widgets)
		{
			widget->geometry->destR.x = widget->geometry->getX() - scroll.x + difX;
			widget->geometry->destR.y = widget->geometry->getY() - scroll.y + difY;
			confineObject(widget);
		}
	}
	void draw() override
	{
		return_if(!visible);

		window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
		window.renderer->drawRectangle(geometry->destR, borderColor);

		up->draw();
		down->draw();
		scrollbar->draw();
	}
};

#endif /* E16839BE_0A2F_4866_9CD5_6C81E791C128 */
