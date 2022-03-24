
#include "mt_geometry.hpp"

Mt_geometry::Mt_geometry()
{
	anchor = none;
}
Mt_geometry::Mt_geometry(int x, int y) : x(x), y(y)
{
	destR.x = x;
	destR.y = y;
}
Mt_geometry::Mt_geometry(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
{
	destR.x = x;
	destR.y = y;
	destR.w = w;
	destR.h = h;
}
Mt_geometry::~Mt_geometry()
{
	Debug("Destroying geometry");
}

void Mt_geometry::setAnchor(center anchor)
{
	this->anchor = anchor;
	posCenter();
}

int Mt_geometry::getX() const noexcept { return x; }
void Mt_geometry::setX(int x)
{
	this->x = x;
	// posCenter();
}
int Mt_geometry::getY() const noexcept { return y; }
void Mt_geometry::setY(int y)
{
	this->y = y;
	// posCenter();
}
int Mt_geometry::getW() const noexcept { return w; }
void Mt_geometry::setW(int w)
{
	this->w = w;
	posCenter();
}
int Mt_geometry::getH() const noexcept { return h; }
void Mt_geometry::setH(int h)
{
	this->h = h;
	posCenter();
}

void Mt_geometry::setGeometry(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	posCenter();
}

void Mt_geometry::confine(const SDL_Rect& box)
{
	confineX(box);
	confineY(box);
}

void Mt_geometry::confineX(const SDL_Rect& box)
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

void Mt_geometry::confineY(const SDL_Rect& box)
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
bool Mt_geometry::intercept(const Mt_geometry& geometry) const
{
	return (geometry.destR.x + geometry.getW() <= this->destR.x ||
		geometry.destR.y + geometry.getH() <= this->destR.y ||
		geometry.destR.x >= this->destR.x + this->destR.w ||
		geometry.destR.y >= this->destR.y + this->destR.h);
}
void Mt_geometry::normalize()
{
	posCenter();
	srcR.x = srcR.y = 0;
	destR.w = srcR.w = w;
	destR.h = srcR.h = h;
}

void Mt_geometry::posCenter()
{
	switch (anchor)
	{
	case none:
		break;
	case top_left:
		destR.x = x;
		destR.y = y;
		break;
	case top_center:
		destR.x = x - (w / 2);
		destR.y = y;
		break;
	case top_right:
		destR.x = x - w;
		destR.y = y;
		break;
	case middle_left:
		destR.x = x;
		destR.y = y - (h / 2);
		break;
	case middle_center:
		destR.x = x - (w / 2);
		destR.y = y - (h / 2);
		break;
	case middle_right:
		destR.x = x - w;
		destR.y = y - (h / 2);
		break;
	case bottom_left:
		destR.x = x;
		destR.y = y - h;
		break;
	case bottom_center:
		destR.x = x - (w / 2);
		destR.y = y - h;
		break;
	case bottom_right:
		destR.x = x - w;
		destR.y = y - h;
		break;
	}
}

void Mt_geometry::adjustCenter()
{
	switch (anchor)
	{
	case none:
		break;
	case top_left:
		destR.x = destR.x;
		destR.y = destR.y;
		break;
	case top_center:
		destR.x = destR.x - (w / 2);
		destR.y = destR.y;
		break;
	case top_right:
		destR.x = destR.x - w;
		destR.y = destR.y;
		break;
	case middle_left:
		destR.x = destR.x;
		destR.y = destR.y - (h / 2);
		break;
	case middle_center:
		destR.x = destR.x - (w / 2);
		destR.y = destR.y - (h / 2);
		break;
	case middle_right:
		destR.x = destR.x - w;
		destR.y = destR.y - (h / 2);
		break;
	case bottom_left:
		destR.x = destR.x;
		destR.y = destR.y - h;
		break;
	case bottom_center:
		destR.x = destR.x - (w / 2);
		destR.y = destR.y - h;
		break;
	case bottom_right:
		destR.x = destR.x - w;
		destR.y = destR.y - h;
		break;
	}
}