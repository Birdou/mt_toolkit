
#include "mt_renderer.hpp"

#include "mt_color.hpp"
#include "mt_font.hpp"
#include "mt_geometry.hpp"

using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;

Mt_renderer::Mt_renderer(SDL_Window *window)
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_PrintError(Error);
	}
	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
	{
		SDL_PrintError(Warn);
	}
}
Mt_renderer::~Mt_renderer()
{
	SDL_DestroyRenderer(renderer);
}

void Mt_renderer::clear()
{
	if (SDL_RenderClear(renderer) < 0)
	{
		SDL_PrintError(Error);
	}
}
void Mt_renderer::setDrawColor(const Mt_RGBA &color)
{
	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
	{
		SDL_PrintError(Error);
	}
}
void Mt_renderer::present()
{
	SDL_RenderPresent(renderer);
}
SDL_Texture *Mt_renderer::createTextureFromSurface(SDL_Surface *surface)
{
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
	if (tex == nullptr)
	{
		SDL_PrintError(Error);
	}
	return tex;
}

int Mt_renderer::substrWidth(TTF_Font *font, const std::string &string, size_t from, size_t length)
{
	if (font == nullptr)
	{
		Error("Invalid font");
		return -1;
	}
	SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, string.substr(from, length).c_str(), {0, 0, 0, 0});
	SDL_Texture *tex = createTextureFromSurface(textSurface);

	int twidth;
	SDL_QueryTexture(tex, nullptr, nullptr, &twidth, nullptr);
	SDL_DestroyTexture(tex);

	SDL_FreeSurface(textSurface);

	return twidth;
}

void Mt_renderer::drawTexture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
	if (SDL_RenderCopy(renderer, texture, src, dest) < 0)
	{
		SDL_PrintError(Error);
		drawFillRectangle(*dest, Mt_RGBA(255, 0, 0, 122));
	}
	DebugFrame(*dest, Mt_RGBA(255, 0, 0));
}

void Mt_renderer::drawRectangle(const SDL_Rect &dest, const Mt_RGBA &color)
{
	Mt_RGBA renderColor;
	SDL_GetRenderDrawColor(renderer, &renderColor.r, &renderColor.g, &renderColor.b, &renderColor.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}
void Mt_renderer::drawFillRectangle(const SDL_Rect &dest, const Mt_RGBA &color)
{
	Mt_RGBA renderColor;
	SDL_GetRenderDrawColor(renderer, &renderColor.r, &renderColor.g, &renderColor.b, &renderColor.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}

SDL_Texture *Mt_renderer::renderText(const std::string &text, Mt_font *font, Mt_geometry *geometry, DrawFunction TTF_RenderFunction)
{
	TTF_Font *ttf = font->getFont();
	if (ttf == nullptr)
	{
		Warn("Invalid font.");
		return nullptr;
	}

	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(ttf, text.c_str(), font->color);
	if (textSurface == nullptr)
	{
		SDL_PrintError(Error);
	}

	texture = createTextureFromSurface(textSurface);
	SDL_FreeSurface(textSurface);

	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	geometry->setW(w);
	geometry->setH(h);

	return texture;
}

SDL_Texture *Mt_renderer::renderWrapped(const std::string &text, Mt_font *font, Mt_geometry *geometry, Uint32 wrapLenght, DrawFunctionWrapped TTF_RenderFunction)
{
	TTF_Font *ttf = font->getFont();
	if (ttf == nullptr)
	{
		Error("Invalid font.");
		return nullptr;
	}

	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(ttf, text.c_str(), font->color, wrapLenght);
	if (textSurface == nullptr)
	{
		SDL_PrintError(Error);
	}

	texture = createTextureFromSurface(textSurface);
	SDL_FreeSurface(textSurface);

	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	geometry->setW(w);
	geometry->setH(h);

	return texture;
}