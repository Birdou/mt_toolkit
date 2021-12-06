
#ifndef F7EA772C_EBFE_449F_B91A_03E0F017F457
#define F7EA772C_EBFE_449F_B91A_03E0F017F457

class Mt_geometry
{
public:
	enum center
	{
		none,
		top_left,
		top_center,
		top_right,
		middle_left,
		middle_center,
		middle_right,
		bottom_left,
		bottom_center,
		bottom_right
	};

private:
	int x = 0, y = 0, w = 0, h = 0;
	center anchor = top_left;

public:
	SDL_Rect destR, srcR;

	Mt_geometry()
	{
		anchor = none;
	}
	Mt_geometry(int x, int y) : x(x), y(y)
	{
	}
	Mt_geometry(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
	{
	}

	void setAnchor(Mt_geometry::center anchor)
	{
		this->anchor = anchor;
		posCenter();
	}

	int getX() const { return x; }
	void setX(int x)
	{
		this->x = x;
		posCenter();
	}
	int getY() const { return y; }
	void setY(int y)
	{
		this->y = y;
		posCenter();
	}
	int getW() const { return w; }
	void setW(int w)
	{
		this->w = w;
		posCenter();
	}
	int getH() const { return h; }
	void setH(int h)
	{
		this->h = h;
		posCenter();
	}

	void setGeometry(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;

		posCenter();
	}

	void normalize()
	{
		posCenter();
		srcR.x = srcR.y = 0;
		destR.w = srcR.w = w;
		destR.h = srcR.h = h;
	}

	void posCenter()
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
};

#endif /* F7EA772C_EBFE_449F_B91A_03E0F017F457 */
