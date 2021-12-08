#ifndef A7276862_9029_49C2_B946_EC319AF33AAE
#define A7276862_9029_49C2_B946_EC319AF33AAE

#include <string>
#include <functional>
#include <cstring>

#include "mt_application.hpp"
#include "mt_geometry.hpp"
#include "mt_font.hpp"

using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;

namespace Mt_lib
{
	SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	SDL_Rect rect(int x, int y, int w, int h);

	int substrWidth(SDL_Renderer *renderer, TTF_Font *font, const std::string &string, size_t from, size_t length = std::string::npos);

	SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &texture);

	TTF_Font *loadFont(const std::string &path, int fontSize);

	void drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);

	void drawRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color);
	void drawFillRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color);

	SDL_Texture *renderText(SDL_Renderer *renderer, const std::string &text, Mt_font *font, Mt_geometry *geometry, DrawFunction TTF_RenderFunction);
	SDL_Texture *renderWrapped(SDL_Renderer *renderer, const std::string &text, Mt_font *font, Mt_geometry *geometry, Uint32 wrapLenght, DrawFunctionWrapped TTF_RenderFunction);

	void replaceAll(std::string &str, const std::string &from, const std::string &to);

	void confine(int &destCoord, int &srcCoord, int &destSize, int &srcSize, int maxSize, int boxCoordinate, int boxSize);

	void confineX(Mt_geometry *geometry, const SDL_Rect &box);
	void confineY(Mt_geometry *geometry, const SDL_Rect &box);
}

#endif /* A7276862_9029_49C2_B946_EC319AF33AAE */
