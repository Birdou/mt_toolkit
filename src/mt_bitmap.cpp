
#include "mt_bitmap.hpp"

// ANCHOR PIXELGRID CLASS
void Mt_pixelgrid::updateSurface()
{
	unsigned char *pixels = (unsigned char *)surface->pixels;
	for (size_t y = 0; y < h; ++y)
	{
		for (size_t x = 0; x < w; ++x)
		{
			pixels[4 * (y * surface->w + x) + 2] = data[y][x].r; // 2103
			pixels[4 * (y * surface->w + x) + 1] = data[y][x].g; // 2103
			pixels[4 * (y * surface->w + x) + 0] = data[y][x].b; // 2103
			pixels[4 * (y * surface->w + x) + 3] = 255;			 // 2103
		}
	}
}

Mt_pixelgrid::Mt_pixelgrid()
{
}

void Mt_pixelgrid::init()
{
}
Mt_pixelgrid::Mt_pixelgrid(size_t w, size_t h)
{
	alloc(w, h);
}
Mt_pixelgrid::~Mt_pixelgrid()
{
	erase();
}
Mt_RGBA &Mt_pixelgrid::at(size_t x, size_t y)
{
	return data[y][x];
}
const Mt_RGBA &Mt_pixelgrid::get(size_t x, size_t y) const
{
	return data[y][x];
}

void Mt_pixelgrid::alloc(size_t width, size_t height)
{
	erase();
	w = width;
	h = height;

	data = new Mt_RGBA *[height];
	for (size_t i = 0; i < height; ++i)
		data[i] = new Mt_RGBA[width];
	surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if (surface == nullptr)
	{
		SDL_PrintError(Error);
	}
}

void Mt_pixelgrid::erase()
{
	if (w == 0 || h == 0)
		return;

	for (size_t i = 0; i < h; ++i)
		delete[] data[i];
	delete[] data;
	SDL_FreeSurface(surface);

	w = 0;
	h = 0;

	data = nullptr;
}

size_t Mt_pixelgrid::width() const noexcept
{
	return w;
}
size_t Mt_pixelgrid::height() const noexcept
{
	return h;
}

void Mt_pixelgrid::savePPMA(const std::string &file)
{
	std::ofstream save;
	save.open(file);

	save << "P3\r";
	save << w << " " << h << "\r";
	save << "255\r";

	for (size_t i = 0; i < h; ++i) // o laço superior é relacionado à altura devido a ordem de gravação.
		for (size_t j = 0; j < w; ++j)
			save << data[i][j].r << " " << data[i][j].g << " " << data[i][j].b << "\r";

	save.close();
}
void Mt_pixelgrid::savePPMB(const std::string &file)
{
	std::ofstream save;
	save.open(file, std::ios_base::binary);

	save << "P6\r";
	save << w << " " << h << "\r";
	save << "255\r";

	for (size_t i = 0; i < h; ++i) // o laço superior é relacionado à altura devido a ordem de gravação.
		for (size_t j = 0; j < w; ++j)
			save << (unsigned char)data[i][j].r << (unsigned char)data[i][j].g << (unsigned char)data[i][j].b;

	save.close();
}
void Mt_pixelgrid::saveJPG(const std::string &file)
{
	updateSurface();
	if (IMG_SaveJPG(surface, file.c_str(), 100) < 0)
	{
		SDL_PrintError(Error);
	}
}
void Mt_pixelgrid::savePNG(const std::string &file)
{
	updateSurface();
	if (IMG_SavePNG(surface, file.c_str()) < 0)
	{
		SDL_PrintError(Error);
	}
}
void Mt_pixelgrid::saveBMP(const std::string &file)
{
	updateSurface();
	if (SDL_SaveBMP(surface, file.c_str()) < 0)
	{
		SDL_PrintError(Error);
	}
}

// ANCHOR BITMAP CLASS
Mt_bitmap::Mt_bitmap(Mt_window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h) : Mt_widget(window, x, y, w, h)
{
	alloc(map_w, map_h);
}
Mt_bitmap::Mt_bitmap(Mt_window &window, int x, int y, size_t w, size_t h) : Mt_widget(window, x, y, w, h)
{
	alloc(w, h);
}

void Mt_bitmap::init()
{
}

Mt_bitmap &Mt_bitmap::create(Mt_window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h)
{
	Mt_bitmap *bitmap = new Mt_bitmap(window, x, y, w, h, map_w, map_h);
	window.widgets.emplace_back(bitmap);
	return *bitmap;
}
Mt_bitmap &Mt_bitmap::create(Mt_window &window, int x, int y, size_t w, size_t h)
{
	Mt_bitmap *bitmap = new Mt_bitmap(window, x, y, w, h);
	window.widgets.emplace_back(bitmap);
	return *bitmap;
}

Mt_bitmap::~Mt_bitmap()
{
	Debug("Destroying bitmap...");
	erase();
}

Mt_RGBA &Mt_bitmap::at(size_t x, size_t y)
{
	render = true;
	return data[y][x];
}

void Mt_bitmap::alloc(size_t width, size_t height)
{
	erase();
	geometry->srcR.w = w = width;
	geometry->srcR.h = h = height;

	data = new Mt_RGBA *[height];
	for (size_t i = 0; i < height; ++i)
		data[i] = new Mt_RGBA[width];
	surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if (surface == nullptr)
	{
		SDL_PrintError(Error);
	}
}

void Mt_bitmap::erase()
{
	if (w == 0 || h == 0)
		return;

	SDL_FreeSurface(surface);
	if (tex != nullptr)
		SDL_DestroyTexture(tex);
	SDL_PrintIfError(Warn);

	geometry->srcR.w = w = 0;
	geometry->srcR.h = h = 0;

	data = nullptr;
}

void Mt_bitmap::handleEvent()
{
	HANDLE_WINDOW_EVENTS;

	return_if(!visible);
}

void Mt_bitmap::update()
{
	return_if(!visible);

	if (Mt_point::mousePos().intercept(geometry->destR))
	{
		if (window.hovering == nullptr)
		{
			window.hovering = this;
		}
		if (window.hovering == this)
		{
			onHover();
			if (pressed)
			{
				onClicked();

				if (window.event.type == SDL_MOUSEBUTTONUP && window.event.button.button == SDL_BUTTON_LEFT)
				{
					onMouseUp();

					pressed = false;
				}
			}
			else
			{
				if (window.event.type == SDL_MOUSEBUTTONDOWN && window.event.button.button == SDL_BUTTON_LEFT)
				{
					onMouseDown();
					onClicked();

					pressed = true;
				}
			}
		}
	}
	else
	{
		if (window.hovering == this)
		{
			pressed = false;
			window.hovering = nullptr;
		}
	}

	if (render)
	{
		if (tex != NULL)
			SDL_DestroyTexture(tex);
		SDL_PrintIfError(Warn);

		updateSurface();
		tex = window.renderer->createTextureFromSurface(surface);

		render = false;
	}
}

void Mt_bitmap::draw()
{
	return_if(!visible);

	window.renderer->drawFillRectangle(geometry->destR, backgroundColor);
	window.renderer->drawRectangle(geometry->destR, borderColor);

	window.renderer->drawTexture(tex, &geometry->srcR, &geometry->destR);
}
