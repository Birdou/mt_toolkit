
#include "widgets/mt_scrollarea.hpp"

#include <algorithm>

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Scrollarea::counter;

TOOLKIT_NAMESPACE::Scrollarea::Scrollarea(TOOLKIT_NAMESPACE::Window &window, int x, int y, int w, int h, int scroll_w, int scroll_h) : Box(window, x, y, w, h)
{

	init();
	up = &Button::create(*this);
	up->font = up->label->font = std::shared_ptr<Font>(new Font(window.getApplication(), defaultFont, defaultFontSize));
	up->geometry->setW(scrollButtonWidth);
	up->geometry->setH(scrollButtonHeight);
	up->geometry->normalize();
	up->repeatInterval = 120;
	up->setScheme(BUTTON_COLOR_SCHEME);
	up->label->loadIcon("assets/icons/uparrow.png");
	up->onClicked = [&]()
	{
		scroll.y -= 27;
		progress = scroll.y / (float)scroll.h;
		progress = std::fmax(0.f, progress);
		progress = std::fmin(progress, 1.f);
		scroll.y = progress * scroll.h;
	};

	down = &Button::create(*this);
	down->font = down->label->font = std::shared_ptr<Font>(new Font(window.getApplication(), defaultFont, defaultFontSize));
	down->geometry->setW(scrollButtonWidth);
	down->geometry->setH(scrollButtonHeight);
	down->geometry->normalize();
	down->repeatInterval = 120;
	down->setScheme(BUTTON_COLOR_SCHEME);
	down->label->loadIcon("assets/icons/downarrow.png");
	down->onClicked = [&]()
	{
		scroll.y += 27;
		progress = scroll.y / (float)scroll.h;
		progress = std::fmax(0.f, progress);
		progress = std::fmin(progress, 1.f);
		scroll.y = progress * scroll.h;
	};

	scroll.w = scroll_w;
	scroll.h = scroll_h;

	scrollbar = &Button::create(*this);
	scrollbar->geometry->setW(scrollButtonWidth);
	scrollbar->geometry->setH(((h / (float)(h + scroll_h)) * (h - (scrollButtonHeight * 2))) - (padding * 2));
	scrollbar->geometry->normalize();
	scrollbar->repeatInterval = 0;
	scrollbar->setScheme(BAR_COLOR_SCHEME);
	scrollbar->onClicked = [&]()
	{
		const int difY = geometry->destR.y - geometry->srcR.y;
		const int minpos = difY + scrollButtonHeight + padding;
		// TODO: Procurar por uma equação que melhor represente maxpos
		const int maxpos = difY + geometry->getH() - scrollButtonHeight - scrollbar->geometry->getH() - (padding * 2);

		int y;
		SDL_GetMouseState(nullptr, &y);

		int pos = y - minpos - scrollbar->clickOffset.y;
		progress = pos / (float)(maxpos - minpos);
		progress = std::fmax(0.f, progress);
		progress = std::fmin(progress, 1.f);
		scroll.y = progress * scroll.h;
	};
}

TOOLKIT_NAMESPACE::Scrollarea &TOOLKIT_NAMESPACE::Scrollarea::create(TOOLKIT_NAMESPACE::Window &window, int x, int y, int w, int h, int scroll_w, int scroll_h)
{
	TOOLKIT_NAMESPACE::Scrollarea *scroll = new TOOLKIT_NAMESPACE::Scrollarea(window, x, y, w, h, scroll_w, scroll_h);
	// window.widgets.emplace_back(scroll);
	window.add(*scroll);
	return *scroll;
}

TOOLKIT_NAMESPACE::Scrollarea::~Scrollarea()
{
	Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

	delete up;
	delete down;
	delete scrollbar;
}

void TOOLKIT_NAMESPACE::Scrollarea::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	up->handleEvent();
	down->handleEvent();
	scrollbar->handleEvent();

	if (hoverScroll && !anyhover)
	{
		switch (window.event.type)
		{
		case SDL_MOUSEWHEEL:
			// if (SDL_GetModState() & KMOD_SHIFT)
			// {
			// 	if (window.event.wheel.y > 0)
			// 	{
			// 		text_x = std::min(0, text_x + pixels);
			// 	}
			// 	else if (window.event.wheel.y < 0)
			// 	{
			// 		text_x -= pixels;
			// 	}
			// }
			// else
			// {
			if (window.event.wheel.y > 0)
			{
				up->onClicked();
			}
			else if (window.event.wheel.y < 0)
			{
				down->onClicked();
			}
			//}
			break;
		}
	}

	for (auto widget : widgets)
		widget->handleEvent();
}

void TOOLKIT_NAMESPACE::Scrollarea::update()
{
	const int difX = geometry->destR.x - geometry->srcR.x;
	const int difY = geometry->destR.y - geometry->srcR.y;
	const int minpos = difY + scrollButtonHeight;
	const int maxpos = difY + geometry->getH() - scrollButtonHeight - scrollbar->geometry->getH() - (padding * 2);

	anyhover = false;
	for (auto widget : widgets)
	{
		widget->geometry->destR.x = widget->geometry->getX() - scroll.x + difX;
		widget->geometry->destR.y = widget->geometry->getY() - scroll.y + difY;
		widget->geometry->adjustCenter();
		anyhover += widget->isHoverScrollable();
		confineObject(widget);
		widget->update();
	}
	hoverScroll += anyhover;

	up->geometry->destR.x = difX + geometry->getW() - up->geometry->getW() - padding - 1;
	up->geometry->destR.y = difY + padding;
	confineObject(up);
	up->update();

	down->geometry->destR.x = difX + geometry->getW() - down->geometry->getW() - padding - 1;
	down->geometry->destR.y = difY + geometry->getH() - down->geometry->getH() - padding;
	confineObject(down);
	down->update();

	scrollbar->geometry->destR.x = difX + geometry->getW() - scrollbar->geometry->getW() - padding - 1;
	scrollbar->geometry->destR.y = minpos + ((maxpos - minpos) * progress) + padding;
	confineObject(scrollbar);
	scrollbar->update();

	if (Point::mousePos().intercept(geometry->destR))
	{
		hoverScroll = true;
		onHover();
	}
	else
	{
		hoverScroll = false;
	}
}

void TOOLKIT_NAMESPACE::Scrollarea::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

	SDL_Rect r = {geometry->destR.x + geometry->getW() - up->geometry->getW() - padding - 1,
				  geometry->destR.y + padding,
				  scrollbar->geometry->getW() + 1,
				  geometry->destR.h - (padding * 2)};

	for (auto widget : widgets)
		widget->draw();

	window.renderer->drawFillRectangle(r, {240, 240, 240, 255});
	up->draw();
	down->draw();
	scrollbar->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}