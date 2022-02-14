
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

int Mt_geometry::getX() const { return x; }
void Mt_geometry::setX(int x)
{
	this->x = x;
	// posCenter();
}
int Mt_geometry::getY() const { return y; }
void Mt_geometry::setY(int y)
{
	this->y = y;
	// posCenter();
}
int Mt_geometry::getW() const { return w; }
void Mt_geometry::setW(int w)
{
	this->w = w;
	posCenter();
}
int Mt_geometry::getH() const { return h; }
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
		destR.y = y;
		destR.x = x;
		break;
	case top_center:
		destR.y = y;
		destR.x = x - (w / 2);
		break;
	case top_right:
		destR.y = y;
		destR.x = x - w;
		break;
	case middle_left:
		destR.y = y - (h / 2);
		destR.x = x;
		break;
	case middle_center:
		destR.y = y - (h / 2);
		destR.x = x - (w / 2);
		break;
	case middle_right:
		destR.y = y - (h / 2);
		destR.x = x - w;
		break;
	case bottom_left:
		destR.y = y - h;
		destR.x = x;
		break;
	case bottom_center:
		destR.y = y - h;
		destR.x = x - (w / 2);
		break;
	case bottom_right:
		destR.y = y - h;
		destR.x = x - w;
		break;
	}
}