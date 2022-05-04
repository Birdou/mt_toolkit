
#include "mt_geometry.hpp"

#include "mt_widget.hpp"

TOOLKIT_NAMESPACE::Geometry::Geometry()
{
	anchor = centers::none;
}
TOOLKIT_NAMESPACE::Geometry::Geometry(int x, int y) : x(x), y(y)
{
	destR.x = x;
	destR.y = y;
}
TOOLKIT_NAMESPACE::Geometry::Geometry(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
{
	destR.x = x;
	destR.y = y;
	destR.w = w;
	destR.h = h;
}
TOOLKIT_NAMESPACE::Geometry::~Geometry()
{
	// Debug("Destroying geometry");
}

void TOOLKIT_NAMESPACE::Geometry::setAnchor(centers::centers anchor)
{
	this->anchor = anchor;
	posCenter();
}

int TOOLKIT_NAMESPACE::Geometry::getX() const noexcept
{
	return x;
}
void TOOLKIT_NAMESPACE::Geometry::setX(int x)
{
	this->x = x;
	// posCenter();
}
int TOOLKIT_NAMESPACE::Geometry::getY() const noexcept
{
	return y;
}
void TOOLKIT_NAMESPACE::Geometry::setY(int y)
{
	this->y = y;
	// posCenter();
}
int TOOLKIT_NAMESPACE::Geometry::getW() const noexcept
{
	return w;
}
void TOOLKIT_NAMESPACE::Geometry::setW(int w)
{
	this->w = w;
	// posCenter();
}
int TOOLKIT_NAMESPACE::Geometry::getH() const noexcept
{
	return h;
}
void TOOLKIT_NAMESPACE::Geometry::setH(int h)
{
	this->h = h;
	// posCenter();
}

void TOOLKIT_NAMESPACE::Geometry::setGeometry(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	posCenter();
}

void TOOLKIT_NAMESPACE::Geometry::confineObject(Widget *widget)
{
	if (intercept(*widget->geometry))
	{
		widget->visible = false;
	}
	else
	{
		widget->visible = true;
		widget->geometry->confine(destR);
	}
}
void TOOLKIT_NAMESPACE::Geometry::confine(const SDL_Rect &box)
{
	confineX(box);
	confineY(box);
}

void TOOLKIT_NAMESPACE::Geometry::confineX(const SDL_Rect &box)
{
	srcR.x = 0;
	destR.w = srcR.w = w;
	if (destR.x >= box.x && destR.x + w < box.x + box.w)
	{
		srcR.x = 0;
		destR.w = srcR.w = w;
	}
	else
	{
		if (destR.x < box.x && destR.x + w > box.x)
		{
			destR.w = srcR.w = (destR.x + w) - box.x;
			srcR.x = box.x - destR.x;
			destR.x = box.x;
		}
		if (destR.x < box.x + box.w && destR.x + w > box.x + box.w)
		{
			destR.w = srcR.w = (box.x + box.w) - destR.x;
		}
	}
}

void TOOLKIT_NAMESPACE::Geometry::confineY(const SDL_Rect &box)
{
	srcR.y = 0;
	destR.h = srcR.h = h;
	if (destR.y >= box.y && destR.y + h < box.y + box.h)
	{
		srcR.y = 0;
		destR.h = srcR.h = h;
	}
	else
	{
		if (destR.y < box.y && destR.y + h > box.y)
		{
			destR.h = srcR.h = (destR.y + h) - box.y;
			srcR.y = box.y - destR.y;
			destR.y = box.y;
		}
		if (destR.y < box.y + box.h && destR.y + h > box.y + box.h)
		{
			destR.h = srcR.h = (box.y + box.h) - destR.y;
		}
	}
}
bool TOOLKIT_NAMESPACE::Geometry::intercept(const Geometry &geometry) const
{
	return (geometry.destR.x + geometry.getW() <= this->destR.x ||
			geometry.destR.y + geometry.getH() <= this->destR.y ||
			geometry.destR.x >= this->destR.x + this->destR.w ||
			geometry.destR.y >= this->destR.y + this->destR.h);
}
void TOOLKIT_NAMESPACE::Geometry::normalize()
{
	posCenter();
	srcR.x = srcR.y = 0;
	destR.w = srcR.w = w;
	destR.h = srcR.h = h;
}

void TOOLKIT_NAMESPACE::Geometry::posCenter()
{
	switch (anchor)
	{
	case centers::none:
		break;
	case centers::top_left:
		destR.x = x;
		destR.y = y;
		break;
	case centers::top_center:
		destR.x = x - (w / 2);
		destR.y = y;
		break;
	case centers::top_right:
		destR.x = x - w;
		destR.y = y;
		break;
	case centers::middle_left:
		destR.x = x;
		destR.y = y - (h / 2);
		break;
	case centers::middle_center:
		destR.x = x - (w / 2);
		destR.y = y - (h / 2);
		break;
	case centers::middle_right:
		destR.x = x - w;
		destR.y = y - (h / 2);
		break;
	case centers::bottom_left:
		destR.x = x;
		destR.y = y - h;
		break;
	case centers::bottom_center:
		destR.x = x - (w / 2);
		destR.y = y - h;
		break;
	case centers::bottom_right:
		destR.x = x - w;
		destR.y = y - h;
		break;
	}
}

void TOOLKIT_NAMESPACE::Geometry::adjustCenter()
{
	switch (anchor)
	{
	case centers::none:
		break;
	case centers::top_left:
		destR.x = destR.x;
		destR.y = destR.y;
		break;
	case centers::top_center:
		destR.x = destR.x - (w / 2);
		destR.y = destR.y;
		break;
	case centers::top_right:
		destR.x = destR.x - w;
		destR.y = destR.y;
		break;
	case centers::middle_left:
		destR.x = destR.x;
		destR.y = destR.y - (h / 2);
		break;
	case centers::middle_center:
		destR.x = destR.x - (w / 2);
		destR.y = destR.y - (h / 2);
		break;
	case centers::middle_right:
		destR.x = destR.x - w;
		destR.y = destR.y - (h / 2);
		break;
	case centers::bottom_left:
		destR.x = destR.x;
		destR.y = destR.y - h;
		break;
	case centers::bottom_center:
		destR.x = destR.x - (w / 2);
		destR.y = destR.y - h;
		break;
	case centers::bottom_right:
		destR.x = destR.x - w;
		destR.y = destR.y - h;
		break;
	}
}