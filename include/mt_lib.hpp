#ifndef A7276862_9029_49C2_B946_EC319AF33AAE
#define A7276862_9029_49C2_B946_EC319AF33AAE

#include <string>
#include <functional>
#include <cstring>

#include "mt_application.hpp"

#ifdef DEBUG
#define Debug(x) std::cout << x << std::endl
#define DebugFrame(renderer, destR, color) drawRectangle(renderer, destR, color)
#else
#define Debug(x)
#define DebugFrame(renderer, destR, color)
#endif

namespace Mt_lib
{
	SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	SDL_Rect rect(int x, int y, int w, int h);

	int substrWidth(SDL_Renderer *renderer, TTF_Font *font, const std::string &string, size_t from, size_t length = std::string::npos);

	SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &texture);

	TTF_Font *loadFont(const std::string &path, int fontSize);

	void drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
	void drawFillRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color);
	void drawRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color);

	SDL_Texture *renderText(SDL_Renderer *renderer, const std::string &text, TTF_Font *font, SDL_Rect &src, std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)> TTF_RenderFunction);
	SDL_Texture *renderGlyph(SDL_Renderer *renderer, const Uint16 *text, TTF_Font *font, SDL_Rect &src, std::function<SDL_Surface *(TTF_Font *, const Uint16 *, SDL_Color)> TTF_GlyphRenderFunction);
	SDL_Texture *renderWrapped(SDL_Renderer *renderer, const std::string &text, TTF_Font *font, Uint32 wrapLenght, SDL_Rect &src, std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)> TTF_RenderFunction);

	void replaceAll(std::string &str, const std::string &from, const std::string &to);

	void confine(int &destCoord, int &srcCoord, int &destSize, int &srcSize, int maxSize, int boxCoordinate, int boxSize);
}

#endif /* A7276862_9029_49C2_B946_EC319AF33AAE */
