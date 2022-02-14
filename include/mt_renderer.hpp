#ifndef FB734E9A_4C58_40C9_B7B9_3D14FEEC12E2
#define FB734E9A_4C58_40C9_B7B9_3D14FEEC12E2

#include "mt_core.hpp"

#include <functional>

using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;

class Mt_font;
class Mt_geometry;
class Mt_RGBA;

class Mt_renderer
{
private:
	SDL_Renderer *renderer = nullptr;

public:
	Mt_renderer(SDL_Window *window);
	~Mt_renderer();

	void clear();
	void setDrawColor(const Mt_RGBA &color);
	void present();
	SDL_Texture *createTextureFromSurface(SDL_Surface *surface);

	int substrWidth(TTF_Font *font, const std::string &string, size_t from, size_t length = std::string::npos);

	SDL_Texture *loadTexture(const std::string &texture);

	void drawTexture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
	void drawRectangle(const SDL_Rect &dest, const Mt_RGBA &color);
	void drawFillRectangle(const SDL_Rect &dest, const Mt_RGBA &color);

	SDL_Texture *renderText(const std::string &text, Mt_font *font, Mt_geometry *geometry, DrawFunction TTF_RenderFunction);
	SDL_Texture *renderWrapped(const std::string &text, Mt_font *font, Mt_geometry *geometry, Uint32 wrapLenght, DrawFunctionWrapped TTF_RenderFunction);
};

#endif /* FB734E9A_4C58_40C9_B7B9_3D14FEEC12E2 */
