
#include "mt_renderer.hpp"

#include "mt_font.hpp"
#include "mt_geometry.hpp"

using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;

TOOLKIT_NAMESPACE::Renderer::Renderer(SDL_Window *window)
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
TOOLKIT_NAMESPACE::Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
}

void TOOLKIT_NAMESPACE::Renderer::clear()
{
	if (SDL_RenderClear(renderer) < 0)
	{
		SDL_PrintError(Error);
	}
}
void TOOLKIT_NAMESPACE::Renderer::setDrawColor(const TOOLKIT_NAMESPACE::RGBA &color)
{
	renderColor = color;
	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
	{
		SDL_PrintError(Error);
	}
}
void TOOLKIT_NAMESPACE::Renderer::present()
{
	SDL_RenderPresent(renderer);
}
SDL_Texture *TOOLKIT_NAMESPACE::Renderer::createTextureFromSurface(SDL_Surface *surface)
{
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
	if (tex == nullptr)
	{
		SDL_PrintError(Error);
	}
	return tex;
}

int TOOLKIT_NAMESPACE::Renderer::substrWidth(TTF_Font *font, const std::string &string, size_t from, size_t length)
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

void TOOLKIT_NAMESPACE::Renderer::drawTexture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
	DebugFrame(*dest, TOOLKIT_NAMESPACE::RGBA(255, 0, 0));
	if (dest->w <= 0 || dest->h <= 0)
		return;

	if (SDL_RenderCopy(renderer, texture, src, dest) < 0)
	{
		SDL_PrintError(Error);
		drawFillRectangle(*dest, TOOLKIT_NAMESPACE::RGBA(255, 0, 0, 122));
	}
}

void TOOLKIT_NAMESPACE::Renderer::drawRectangle(const SDL_Rect &dest, const TOOLKIT_NAMESPACE::RGBA &color)
{
	if (color.a == 0)
		return;

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}
void TOOLKIT_NAMESPACE::Renderer::drawFillRectangle(const SDL_Rect &dest, const TOOLKIT_NAMESPACE::RGBA &color)
{
	if (color.a == 0)
		return;

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}

SDL_Texture *TOOLKIT_NAMESPACE::Renderer::renderText(const std::string &text, TTF_Font *font, Geometry *geometry, DrawFunction TTF_RenderFunction)
{
	if (font == nullptr)
	{
		Warn("Invalid font.");
		return nullptr;
	}

	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(font, text.c_str(), {255, 255, 255, 255});
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

SDL_Texture *TOOLKIT_NAMESPACE::Renderer::renderWrapped(const std::string &text, TTF_Font *font, Geometry *geometry, Uint32 wrapLenght, DrawFunctionWrapped TTF_RenderFunction)
{
	if (font == nullptr)
	{
		Error("Invalid font.");
		return nullptr;
	}

	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(font, text.c_str(), {255, 255, 255, 255}, wrapLenght);
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