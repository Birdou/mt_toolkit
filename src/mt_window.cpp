
#include "mt_window.hpp"

#include <algorithm>

#include "mt_widget.hpp"
#include "mt_util.hpp"
#include "mt_point.hpp"

TOOLKIT_NAMESPACE::Parent::~Parent()
{
	for (auto widget : widgets)
		delete widget;
}
const std::vector<TOOLKIT_NAMESPACE::Widget *> &TOOLKIT_NAMESPACE::Parent::get() const noexcept
{
	return widgets;
}
void TOOLKIT_NAMESPACE::Parent::add(TOOLKIT_NAMESPACE::Widget &widget)
{
	if (widget.parent != nullptr)
	{
		auto &_widgets = widget.parent->widgets;
		_widgets.erase(std::remove_if(_widgets.begin(), _widgets.end(),
									  [this, &widget](const TOOLKIT_NAMESPACE::Widget *ptr)
									  {
										  if (&widget == ptr)
										  {
											  Debug("Moving object " << widget.getId());
											  widget.parent = this;
											  widgets.emplace_back(&widget);
											  return true;
										  }
										  return false;
									  }));
	}
	else
	{
		widget.parent = this;
		widgets.emplace_back(&widget);
	}
}

TOOLKIT_NAMESPACE::Window::Window(TOOLKIT_NAMESPACE::Application &application, const std::string &title, int w, int h, int flags) : application(application), title(title)
{
	rect = {0, 0, w, h};

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
	windowID = SDL_GetWindowID(window);

	SDL_GetWindowSize(window, &rect.w, &rect.h);

	renderer = new Renderer(window);

	active = true;
}

TOOLKIT_NAMESPACE::Window::Window(TOOLKIT_NAMESPACE::Window &parentWindow, const std::string &title, int w, int h, int flags) : application(parentWindow.application), title(title)
{
	rect = {0, 0, w, h};

	int posx, posy;
	parentWindow.getPostition(posx, posy);
	int x = posx + (parentWindow.rect.w / 2) - (w / 2);
	int y = posy + (parentWindow.rect.h / 2) - (h / 2);

	window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
	windowID = SDL_GetWindowID(window);

	renderer = new Renderer(window);

	active = true;
}

void TOOLKIT_NAMESPACE::Window::setDraggable(bool draggable)
{
	this->draggable = draggable;
	if (draggable)
		SDL_SetWindowHitTest(window, hitTest, (void *)this);
	else
		SDL_SetWindowHitTest(window, NULL, NULL);
}
bool TOOLKIT_NAMESPACE::Window::isDraggable()
{
	return draggable;
}

TOOLKIT_NAMESPACE::Window::messageBox TOOLKIT_NAMESPACE::Window::createMessageBox()
{
	return messageBox(*this);
}
void TOOLKIT_NAMESPACE::Window::showSimpleMessageBox(const char *title, const char *message, int flags)
{
	SDL_ShowSimpleMessageBox(flags, title, message, window);
}

void TOOLKIT_NAMESPACE::Window::getPostition(int &x, int &y)
{
	SDL_GetWindowPosition(window, &x, &y);
}
TOOLKIT_NAMESPACE::Window::~Window()
{
	Debug("Destroying window...");

	for (auto window : windows)
		delete window.second;

	delete renderer;

	if (window)
		SDL_DestroyWindow(window);

	Debug("Done.");
}

bool TOOLKIT_NAMESPACE::Window::isActive() const
{
	return active;
}

TOOLKIT_NAMESPACE::Window &TOOLKIT_NAMESPACE::Window::createChild(const std::string &title, const std::string &id, int width, int height, int flags)
{
	auto find = windows.find(id);
	if (find != windows.end())
	{
		Error("Already exists an active window with this id '" << id << "'");
		return *find->second;
	}
	TOOLKIT_NAMESPACE::Window *window = new TOOLKIT_NAMESPACE::Window(*this, title, width, height, flags);
	windows.emplace(id, window);
	return *window;
}
TOOLKIT_NAMESPACE::Window &TOOLKIT_NAMESPACE::Window::getChildById(const std::string &id)
{
	if (windows.find(id) == windows.end())
		throw window_not_found();
	return *windows[id];
}

void TOOLKIT_NAMESPACE::Window::hide()
{
	if (window)
		SDL_HideWindow(window);
	shown = false;
}

void TOOLKIT_NAMESPACE::Window::show()
{
	if (window)
		SDL_ShowWindow(window);
	shown = true;
}

void TOOLKIT_NAMESPACE::Window::destroy()
{
	active = false;
}

void TOOLKIT_NAMESPACE::Window::setSize(int w, int h)
{
	if (w <= 0 || h <= 0)
		return;

	if (window)
		SDL_SetWindowSize(window, w, h);

	int x, y;
	SDL_GetWindowPosition(window, &x, &y);

	rect.w = w;
	rect.h = h;
}

void TOOLKIT_NAMESPACE::Window::setIcon(const char *file)
{
	SDL_Surface *surf = IMG_Load(file);
	if (surf == nullptr)
	{
		SDL_PrintError(Error);
	}
	SDL_SetWindowIcon(window, surf);
	SDL_FreeSurface(surf);
}
void TOOLKIT_NAMESPACE::Window::setTitle(const char *title)
{
	SDL_SetWindowTitle(window, title);
}
int TOOLKIT_NAMESPACE::Window::height() const
{
	return rect.h;
}
int TOOLKIT_NAMESPACE::Window::width() const
{
	return rect.w;
}

#define RESIZE_BORDER 5
SDL_HitTestResult SDLCALL TOOLKIT_NAMESPACE::Window::hitTest(SDL_Window *, const SDL_Point *pt, void *data)
{
	Window *window = (Window *)data;
	Point vec(*pt);

	for (auto &widget : window->widgets)
		if (widget->visible && vec.intercept(widget->geometry->destR))
			return SDL_HITTEST_NORMAL;

	int w = window->width();
	int h = window->height();

	if (vec.x < RESIZE_BORDER && vec.y < RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_TOPLEFT;
	else if (vec.x > RESIZE_BORDER && vec.x < w - RESIZE_BORDER && vec.y < RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_TOP;
	else if (vec.x > w - RESIZE_BORDER && vec.y < RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_TOPRIGHT;
	else if (vec.x > w - RESIZE_BORDER && vec.y > RESIZE_BORDER && vec.y < h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_RIGHT;
	else if (vec.x > w - RESIZE_BORDER && vec.y > h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
	else if (vec.x < w - RESIZE_BORDER && vec.x > RESIZE_BORDER && vec.y > h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_BOTTOM;
	else if (vec.x < RESIZE_BORDER && vec.y > h - RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_BOTTOMLEFT;
	else if (vec.x < RESIZE_BORDER && vec.y < h - RESIZE_BORDER && vec.y > RESIZE_BORDER)
		return SDL_HITTEST_RESIZE_LEFT;

	return SDL_HITTEST_DRAGGABLE;
}

TOOLKIT_NAMESPACE::Application &TOOLKIT_NAMESPACE::Window::getApplication() const
{
	return application;
}

void TOOLKIT_NAMESPACE::Window::handleEvents()
{
	for (auto &window : windows)
		window.second->handleEvents();

	return_if(!shown);

	this->event = application.event;

	if (event.window.windowID == windowID)
	{
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				if (!persistent)
				{
					hide();
					if (destroyOnClose)
						active = false;
				}
				break;
			case SDL_WINDOWEVENT_RESIZED:
				rect.w = event.window.data1;
				rect.h = event.window.data2;
				break;
			}
		}

		for (auto widget : widgets)
			widget->handleEvent();
	}
}

void TOOLKIT_NAMESPACE::Window::update()
{
	// refresh windows
	for (auto it = windows.begin(); it != windows.end();)
	{
		if (!it->second->isActive())
		{
			delete it->second;
			it = windows.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto &window : windows)
		window.second->update();

	return_if(!shown);

	// refresh widgets
	for (auto it = widgets.begin(); it != widgets.end();)
	{
		if (!(*it)->isActive())
		{
			delete (*it);
			it = widgets.erase(it);
		}
		else
		{
			it++;
		}
	}

	// update
	for (auto widget : widgets)
		widget->update();
}

void TOOLKIT_NAMESPACE::Window::draw()
{
	for (auto &window : windows)
		window.second->draw();

	return_if(!shown);

	renderer->clear();
	renderer->setDrawColor(backgroundColor);

	for (auto widget : widgets)
		widget->draw();

	if (border)
		renderer->drawRectangle(rect, borderColor);

	renderer->present();
}