
#include "mt_containers.hpp"

#include <algorithm>

//ANCHOR CONTAINER CLASS
Mt_container::Mt_container(Mt_window& window, int x, int y, int w, int h) : Mt_widget(window, x, y, w, h)
{
	init();
}
Mt_container::~Mt_container()
{

}
void Mt_container::init()
{
	currentBackgroundColor.hex(White).a = 0;
	currentBorderColor = Mt_RGBA(130, 145, 144);
}

void Mt_container::confineObject(Mt_widget* widget)
{
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

//ANCHOR FLEX CLASS
Mt_flex::Mt_flex(Mt_window& window, int x, int y, int w, int h) : Mt_container(window, x, y, w, h)
{}

Mt_flex& Mt_flex::create(Mt_window& window, int x, int y, int w, int h)
{
	Mt_flex* flex = new Mt_flex(window, x, y, w, h);
	//window.widgets.emplace_back(flex);
	window.add(*flex);
	return *flex;
}
Mt_flex::~Mt_flex()
{
	Debug("Destroying flexbox...");

	for (auto row : rows)
		delete row;

	Debug("Done.");
}
Mt_flex::Mt_row& Mt_flex::createRow()
{
	Mt_row* row = new Mt_row(*this);
	//row->parent = this;
	rows.emplace_back(row);
	return *row;
}
Mt_flex::Mt_row& Mt_flex::at(size_t index)
{
	if (index >= rows.size())
	{
		Error("Linha inexistente '" << index << "'");
		throw std::out_of_range("A classe não conseguiu encontrar a linha especificada.");
	}
	return *rows[index];
}
Mt_flex::Mt_row& Mt_flex::operator[](size_t index)
{
	return at(index);
}
void Mt_flex::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);

	for (auto& row : rows)
		row->handleEvent();
}

void Mt_flex::updateGrid()
{
	const int griddifX = geometry->destR.x - geometry->srcR.x;
	const int griddifY = geometry->destR.y - geometry->srcR.y;
	const int gridTPadding = 2 * padding;

	int lastRowEdgePos = griddifY + padding;
	anyhover = false;
	for (auto& row : rows)
	{
		int maxH = 0;
		int maxColPadding = 0;
		int totalSpan = 0;
		for (auto col : row->cols)
		{
			maxH = std::max(maxH, col->widget->geometry->getH() + (2 * col->padding));
			maxColPadding = std::max(maxColPadding, col->padding);
			totalSpan += col->colspan;
		}

		const int rowTPadding = 2 * row->padding;

		row->geometry->destR.x = griddifX + padding;
		row->geometry->destR.y = lastRowEdgePos;
		row->geometry->setW(geometry->getW() - gridTPadding);
		row->geometry->setH(maxH + rowTPadding);

		lastRowEdgePos += row->geometry->getH();

		geometry->confineObject(row);
		if (!row->visible)
			continue;

		const int rowdifX = row->geometry->destR.x - row->geometry->srcR.x;
		const int rowdifY = row->geometry->destR.y - row->geometry->srcR.y;


		const double space = (1 / (double)totalSpan) * (row->geometry->getW() - rowTPadding);

		int currentSpan = 0;
		const int currentRowY = rowdifY + row->padding;
		for (auto& col : row->cols)
		{
			col->geometry->destR.x = rowdifX + (int)(currentSpan * space) + row->padding;
			col->geometry->destR.y = currentRowY;
			col->geometry->setW((int)(space * col->colspan));
			col->geometry->setH(maxH);

			row->geometry->confineObject(col);
			const int coldifX = col->geometry->destR.x - col->geometry->srcR.x;
			const int coldifY = col->geometry->destR.y - col->geometry->srcR.y;

			switch (col->alignment)
			{
			case center::none:
				break;
			case center::top_left:
				col->widget->geometry->destR.x = coldifX + col->padding;
				col->widget->geometry->destR.y = coldifY + col->padding;
				break;
			case center::top_center:
				col->widget->geometry->destR.x = coldifX + ((col->geometry->destR.w / 2.) - (col->widget->geometry->getW() / 2.));
				col->widget->geometry->destR.y = coldifY + col->padding;
				break;
			case center::top_right:
				col->widget->geometry->destR.x = coldifX + (col->geometry->destR.w - col->widget->geometry->destR.w) - col->padding;
				col->widget->geometry->destR.y = coldifY + col->padding;
				break;
			case center::middle_left:
				col->widget->geometry->destR.x = coldifX + col->padding;
				col->widget->geometry->destR.y = coldifY + ((col->geometry->destR.h / 2.) - (col->widget->geometry->getH() / 2.));
				break;
			case center::middle_center:
				col->widget->geometry->destR.x = coldifX + ((col->geometry->destR.w / 2.) - (col->widget->geometry->getW() / 2.));
				col->widget->geometry->destR.y = coldifY + ((col->geometry->destR.h / 2.) - (col->widget->geometry->getH() / 2.));
				break;
			case center::middle_right:
				col->widget->geometry->destR.x = coldifX + (col->geometry->destR.w - col->widget->geometry->destR.w) - col->padding;
				col->widget->geometry->destR.y = coldifY + ((geometry->destR.h / 2.) - (col->widget->geometry->getH() / 2.));
				break;
			case center::bottom_left:
				col->widget->geometry->destR.x = coldifX + col->padding;
				col->widget->geometry->destR.y = coldifY + (col->geometry->destR.h - col->widget->geometry->destR.h) - col->padding;
				break;
			case center::bottom_center:
				col->widget->geometry->destR.x = coldifX + ((col->geometry->destR.w / 2.) - (col->widget->geometry->getW() / 2.));
				col->widget->geometry->destR.y = coldifY + (col->geometry->destR.h - col->widget->geometry->destR.h) - col->padding;
				break;
			case center::bottom_right:
				col->widget->geometry->destR.x = coldifX + (col->geometry->destR.w - col->widget->geometry->destR.w) - col->padding;
				col->widget->geometry->destR.y = coldifY + (geometry->destR.h - col->widget->geometry->destR.h) - col->padding;
				break;
			}

			col->geometry->confineObject(col->widget);
			col->widget->update();
			anyhover += col->widget->isHoverScrollable();

			currentSpan += col->colspan;

			col->update();
		}
		row->update();
	}
	hoverScroll += anyhover;
	geometry->setH(lastRowEdgePos - griddifY + padding);

	lastVirtualX = griddifX;
	lastVirtualY = griddifY;
	gupdate = false;
}

void Mt_flex::update()
{
	return_if(!visible);

	updateGrid();
	// const int griddifX = geometry->destR.x - geometry->srcR.x;
	// const int griddifY = geometry->destR.y - geometry->srcR.y;

	// if (griddifX != lastVirtualX || griddifY != lastVirtualY || gupdate)
	// {
	// 	updateGrid();
	// }
	// else
	// {
	// 	for (auto& row : rows)
	// 	{
	// 		for (auto col : row->cols)
	// 		{
	// 			col->widget->update();
	// 		}
	// 	}
	// }

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		fadeToHover();
	}
	else
	{
		fadeToNormal();
	}

}
void Mt_flex::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	for (auto& row : rows)
		row->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}

//ANCHOR FLEX::ROW CLASS
void Mt_flex::Mt_row::init()
{
	geometry->setAnchor(center::none);
	currentBackgroundColor.hex(White).a = 0;
	currentBorderColor.hex(White).a = 0;
}
Mt_flex::Mt_row::Mt_row(Mt_flex& grid) : Mt_widget(grid), grid(grid)
{
	init();
}
Mt_flex::Mt_row::~Mt_row()
{
	for (auto col : cols)
		delete col;
}
void Mt_flex::Mt_row::addWidget(Mt_widget& widget, size_t colspan)
{
	grid.add(widget);
	Mt_cel* col = new Mt_cel(*this, &widget, colspan);
	cols.emplace_back(col);
	widget.update();
	grid.gupdate = true;
}
Mt_flex::Mt_row::Mt_cel& Mt_flex::Mt_row::at(size_t id)
{
	if (id >= cols.size())
	{
		Error("Coluna inexistente '" << id << "'");
		throw std::out_of_range("A classe não conseguiu encontrar a coluna especificada.");
	}
	return *cols[id];
}
Mt_flex::Mt_row::Mt_cel& Mt_flex::Mt_row::operator[](size_t id)
{
	return at(id);
}

void Mt_flex::Mt_row::handleEvent()
{
	return_if(cols.empty() || !visible);

	for (auto col : cols)
		col->handleEvent();
}
void Mt_flex::Mt_row::update()
{
	return_if(!visible);
	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		fadeToHover();
	}
	else
	{
		fadeToNormal();
	}
}
void Mt_flex::Mt_row::draw()
{
	return_if(cols.empty() || !visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	for (auto col : cols)
		col->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}

//ANCHOR FLEX::ROW::CEL CLASS
void Mt_flex::Mt_row::Mt_cel::init()
{
	//geometry->setAnchor(center::none);
	currentBackgroundColor.hex(White).a = 0;
	currentBorderColor.hex(White).a = 0;
}
Mt_flex::Mt_row::Mt_cel::Mt_cel(Mt_row& row, Mt_widget* widget, size_t colspan) : Mt_widget(row), row(row), widget(widget), colspan(colspan)
{
	init();
}
Mt_flex::Mt_row::Mt_cel::~Mt_cel()
{

}
void Mt_flex::Mt_row::Mt_cel::handleEvent()
{
	return_if(!visible);
	widget->handleEvent();
}
void Mt_flex::Mt_row::Mt_cel::update()
{
	return_if(!visible);
	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		fadeToHover();
	}
	else
	{
		fadeToNormal();
	}
}
void Mt_flex::Mt_row::Mt_cel::draw()
{
	return_if(!visible);
	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	//if (widget != nullptr)
	widget->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}

//ANCHOR BOX CLASS
Mt_box::Mt_box(Mt_window& window, int x, int y, int w, int h) : Mt_container(window, x, y, w, h)
{
	init();
}
Mt_box& Mt_box::create(Mt_window& window, int x, int y, int w, int h)
{
	Mt_box* scroll = new Mt_box(window, x, y, w, h);
	window.add(*scroll);
	return *scroll;
}
Mt_box::~Mt_box()
{
	Debug("Destroying box.");
}

void Mt_box::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	for (auto widget : widgets)
		widget->handleEvent();
}
void Mt_box::update()
{
	const int difX = geometry->destR.x - geometry->srcR.x;
	const int difY = geometry->destR.y - geometry->srcR.y;

	anyhover = false;
	for (auto widget : widgets)
	{
		widget->geometry->destR.x = widget->geometry->getX() + difX;
		widget->geometry->destR.y = widget->geometry->getY() + difY;
		widget->geometry->adjustCenter();
		anyhover += widget->isHoverScrollable();
		confineObject(widget);
		widget->update();
	}

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		onHover();
	}

}
void Mt_box::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
	for (auto widget : widgets)
		widget->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}

//ANCHOR SCROLL CLASS
Mt_scrollarea::Mt_scrollarea(Mt_window& window, int x, int y, int w, int h, int scroll_w, int scroll_h) : Mt_box(window, x, y, w, h)
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
	up->onClicked = [&]()
	{
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

	scrollbar = &Mt_button::create(*this);
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

Mt_scrollarea& Mt_scrollarea::create(Mt_window& window, int x, int y, int w, int h, int scroll_w, int scroll_h)
{
	Mt_scrollarea* scroll = new Mt_scrollarea(window, x, y, w, h, scroll_w, scroll_h);
	//window.widgets.emplace_back(scroll);
	window.add(*scroll);
	return *scroll;
}

Mt_scrollarea::~Mt_scrollarea()
{
	Debug("Destroying scrollarea...");

	delete up;
	delete down;
	delete scrollbar;

	Debug("Done.");
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

	window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);

	SDL_Rect r = { geometry->destR.x + geometry->getW() - up->geometry->getW() - padding - 1,
				  geometry->destR.y + padding,
				  scrollbar->geometry->getW() + 1,
				  geometry->destR.h - (padding * 2) };


	for (auto widget : widgets)
		widget->draw();

	window.renderer->drawFillRectangle(r, { 240, 240, 240, 255 });
	up->draw();
	down->draw();
	scrollbar->draw();
	window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}