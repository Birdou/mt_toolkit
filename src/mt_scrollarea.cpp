
#include "mt_scrollarea.hpp"

#include <algorithm>

Mt_scrollarea::Mt_scrollarea(Mt_window& window, int x, int y, int w, int h, int scroll_w, int scroll_h) : Mt_widget(window, x, y, w, h)
{
	init();
	up = &Mt_button::create(*this);
	up->font = up->label->font = std::shared_ptr<Mt_font>(new Mt_font(window.getApplication(), defaultFont, defaultFontSize));
	up->geometry->setW(scrollButtonWidth);
	up->geometry->setH(scrollButtonHeight);
	up->geometry->normalize();
	up->repeatInterval = 120;
	up->setScheme(BUTTON_COLOR_SCHEME);
	up->label->loadIcon("assets/icons/uparrow.png");
	up->onClicked = [&]() {
		scroll.y -= 27;
		progress = scroll.y / (float)scroll.h;
		progress = std::fmax(0.f, progress);
		progress = std::fmin(progress, 1.f);
		scroll.y = progress * scroll.h;
	};

	down = &Mt_button::create(*this);
	down->font = down->label->font = std::shared_ptr<Mt_font>(new Mt_font(window.getApplication(), defaultFont, defaultFontSize));
	down->geometry->setW(scrollButtonWidth);
	down->geometry->setH(scrollButtonHeight);
	down->geometry->normalize();
	down->repeatInterval = 120;
	down->setScheme(BUTTON_COLOR_SCHEME);
	down->label->loadIcon("assets/icons/downarrow.png");
	down->onClicked = [&]() {
		scroll.y += 27;
		progress = scroll.y / (float)scroll.h;
		progress = std::fmax(0.f, progress);
		progress = std::fmin(progress, 1.f);
		scroll.y = progress * scroll.h;
	};

	scroll.w = scroll_w;
	scroll.h = scroll_h;

	scrollbar = &Mt_button::create(*this);
	scrollbar->geometry->setW(scrollButtonWidth);
	scrollbar->geometry->setH(((h / (float)(h + scroll_h)) * (h - (scrollButtonHeight * 2))) - (padding * 2));
	scrollbar->geometry->normalize();
	scrollbar->repeatInterval = 0;
	scrollbar->setScheme(BAR_COLOR_SCHEME);
	scrollbar->onClicked = [&]() {
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

void Mt_scrollarea::init()
{
	backgroundColor = Mt_RGBA(0, 0, 0);
	borderColor = Mt_RGBA(130, 145, 144);
}

Mt_scrollarea& Mt_scrollarea::create(Mt_window& window, int x, int y, int w, int h, int scroll_w, int scroll_h)
{
	return *new Mt_scrollarea(window, x, y, w, h, scroll_w, scroll_h);
}

Mt_scrollarea::~Mt_scrollarea()
{
	Debug("Destroying scrollarea...");

	delete up;
	delete down;
	delete scrollbar;

	for (auto widget : widgets)
		if (!widget->getParent())
			delete widget;

	Debug("Done.");
}

void Mt_scrollarea::addWidget(Mt_widget& widget)
{
	window.widgets.erase(std::remove_if(window.widgets.begin(), window.widgets.end(),
		[&](const Mt_widget* ptr) {
			if (&widget == ptr)
			{
				widgets.emplace_back(&widget);
				return true;
			}
			return false;
		}));
}

void Mt_scrollarea::confineObject(Mt_widget* widget)
{

	//if (widget->geometry->destR.x + widget->geometry->getW() <= geometry->destR.x ||
	//	widget->geometry->destR.y + widget->geometry->getH() <= geometry->destR.y ||
	//	widget->geometry->destR.x >= geometry->destR.x + geometry->destR.w ||
	//	widget->geometry->destR.y >= geometry->destR.y + geometry->destR.h)
	if (geometry->intercept(*widget->geometry))
	{
		widget->visible = false;
	}
	else
	{
		widget->visible = true;
		widget->geometry->confine(geometry->destR);
	}
}

void Mt_scrollarea::handleEvent()
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

void Mt_scrollarea::update()
{
	const int difX = geometry->destR.x - geometry->srcR.x;
	const int difY = geometry->destR.y - geometry->srcR.y;
	const int minpos = difY + scrollButtonHeight;
	const int maxpos = difY + geometry->getH() - scrollButtonHeight - scrollbar->geometry->getH() - (padding * 2);

	anyhover = false;
	for (auto widget : widgets)
	{
		widget->update();
		widget->geometry->destR.x = widget->geometry->getX() - scroll.x + difX;
		widget->geometry->destR.y = widget->geometry->getY() - scroll.y + difY;
		widget->geometry->adjustCenter();
		anyhover += widget->isHoverScrollable();
		confineObject(widget);
	}
	hoverScroll += anyhover;

	up->update();
	up->geometry->destR.x = difX + geometry->getW() - up->geometry->getW() - padding - 1;
	up->geometry->destR.y = difY + padding;
	confineObject(up);

	down->update();
	down->geometry->destR.x = difX + geometry->getW() - down->geometry->getW() - padding - 1;
	down->geometry->destR.y = difY + geometry->getH() - down->geometry->getH() - padding;
	confineObject(down);

	scrollbar->update();
	scrollbar->geometry->destR.x = difX + geometry->getW() - scrollbar->geometry->getW() - padding - 1;
	scrollbar->geometry->destR.y = minpos + ((maxpos - minpos) * progress) + padding;
	confineObject(scrollbar);

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		hoverScroll = true;
		onHover();
	}
	else
	{
		hoverScroll = false;
	}
}

void Mt_scrollarea::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	SDL_Rect r = { geometry->destR.x + geometry->getW() - up->geometry->getW() - padding - 1,
				  geometry->destR.y + padding,
				  scrollbar->geometry->getW() + 1,
				  geometry->destR.h - (padding * 2) };
	window.renderer->drawFillRectangle(r, { 240, 240, 240, 255 });

	up->draw();
	down->draw();
	scrollbar->draw();

	for (auto widget : widgets)
		widget->draw();
}