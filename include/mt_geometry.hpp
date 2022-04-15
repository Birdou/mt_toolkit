
#ifndef F7EA772C_EBFE_449F_B91A_03E0F017F457
#define F7EA772C_EBFE_449F_B91A_03E0F017F457

#include "mt_application.hpp"

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
class Mt_widget;
class Mt_geometry
{
public:
private:
	int x = 0, y = 0, w = 0, h = 0;
	center anchor = top_left;

public:
	float scale = 1.f;
	SDL_Rect destR, srcR;

	Mt_geometry();
	Mt_geometry(int x, int y);
	Mt_geometry(int x, int y, int w, int h);
	~Mt_geometry();

	void setAnchor(center anchor);

	int getX() const noexcept;
	void setX(int x);
	int getY() const noexcept;
	void setY(int y);
	int getW() const noexcept;
	void setW(int w);
	int getH() const noexcept;
	void setH(int h);

	void setGeometry(int x, int y, int w, int h);
	void confineObject(Mt_widget* widget);
	void confine(const SDL_Rect& box);
	void confineX(const SDL_Rect& box);
	void confineY(const SDL_Rect& box);

	bool intercept(const Mt_geometry& geometry)const;

	void normalize();

	void posCenter();
	void adjustCenter();
};

#endif /* F7EA772C_EBFE_449F_B91A_03E0F017F457 */
