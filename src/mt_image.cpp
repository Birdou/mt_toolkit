
#include "mt_image.hpp"

Mt_image::Mt_image(Mt_window &window, int x, int y, const std::string &filename) : Mt_widget(window, x, y)
{
	loadImage(filename);
}

void Mt_image::loadImage(const std::string &filename)
{
	SDL_Surface *surface = nullptr;
	surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
		SDL_PrintError(Error);
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
	SDL_PrintIfError(Warn);
	texture = window.renderer->createTextureFromSurface(surface);

	SDL_FreeSurface(surface);
}