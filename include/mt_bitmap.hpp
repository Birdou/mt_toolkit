#ifndef F485EFCF_8079_4F33_91F7_A134EA3822B3
#define F485EFCF_8079_4F33_91F7_A134EA3822B3

#include "mt_application.hpp"

#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_vector.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

class Mt_pixelgrid
{
protected:
	SDL_Surface *surface = nullptr;
	Mt_RGBA **data = nullptr;
	size_t w = 0, h = 0;

	void updateSurface();

	Mt_pixelgrid();

	void init();

public:
	Mt_pixelgrid(size_t w, size_t h);
	~Mt_pixelgrid();

	virtual Mt_RGBA &at(size_t x, size_t y);
	virtual const Mt_RGBA &get(size_t x, size_t y) const;

	virtual void alloc(size_t width, size_t height);
	virtual void erase();

	size_t width() const noexcept;
	size_t height() const noexcept;

	void savePPMA(const std::string &file);
	void savePPMB(const std::string &file);
	void saveJPG(const std::string &file);
	void savePNG(const std::string &file);
	void saveBMP(const std::string &file);
};

class Mt_bitmap : public Mt_widget, public Mt_pixelgrid
{
private:
	SDL_Texture *tex = nullptr;

	bool render = false;
	bool pressed = false;

	Mt_bitmap(Mt_window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h);
	Mt_bitmap(Mt_window &window, int x, int y, size_t w, size_t h);
	Mt_bitmap(const Mt_bitmap &);

public:
	static Mt_bitmap &create(Mt_window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h);
	static Mt_bitmap &create(Mt_window &window, int x, int y, size_t w, size_t h);

	~Mt_bitmap();

	Mt_RGBA &at(size_t x, size_t y) override;

	void alloc(size_t width, size_t height) override;

	void erase() override;

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* F485EFCF_8079_4F33_91F7_A134EA3822B3 */
