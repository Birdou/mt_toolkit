
#include "mt_lib.hpp"

SDL_Color Mt_lib::color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color color = {r, g, b, a};
	return color;
}

SDL_Rect Mt_lib::rect(int x, int y, int w, int h)
{
	SDL_Rect rect = {x, y, w, h};
	return rect;
}

int Mt_lib::substrWidth(SDL_Renderer *renderer, TTF_Font *font, const std::string &string, size_t from, size_t length)
{
	SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, string.substr(from, length).c_str(), color(0, 0, 0));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, textSurface);

	int twidth;
	SDL_QueryTexture(tex, nullptr, nullptr, &twidth, nullptr);

	SDL_DestroyTexture(tex);
	SDL_FreeSurface(textSurface);

	return twidth;
}

SDL_Texture *Mt_lib::loadTexture(SDL_Renderer *renderer, const std::string &texture)
{
	SDL_Surface *tmpSurface = IMG_Load(texture.c_str());
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if (strcmp(SDL_GetError(), "") != 0)
	{
		std::cout << "LOADTEXTURE:: " << texture << " : " << SDL_GetError() << std::endl;
		SDL_ClearError();
	}
	return tex;
}

TTF_Font *Mt_lib::loadFont(const std::string &path, int fontSize)
{
	return TTF_OpenFont(path.c_str(), fontSize);
}

void Mt_lib::drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
	if (texture == nullptr)
	{
		std::cout << "Mt_lib::drawTexture: invalid texture" << std::endl;
	}
	else
	{
		SDL_RenderCopy(renderer, texture, src, dest);
	}

	DebugFrame(renderer, *dest, color(255, 0, 0));
}

void Mt_lib::drawFillRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color)
{
	SDL_Color renderColor;
	SDL_GetRenderDrawColor(renderer, &renderColor.r, &renderColor.g, &renderColor.b, &renderColor.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}

void Mt_lib::drawRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color)
{
	SDL_Color renderColor;
	SDL_GetRenderDrawColor(renderer, &renderColor.r, &renderColor.g, &renderColor.b, &renderColor.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}

SDL_Texture *Mt_lib::renderText(SDL_Renderer *renderer, const std::string &text, TTF_Font *font, SDL_Rect &src, std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)> TTF_RenderFunction)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(font, text.c_str(), Mt_lib::color(0, 0, 0));
	if (textSurface != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);

		SDL_QueryTexture(texture, nullptr, nullptr, &src.w, &src.h);
	}
	return texture;
}

SDL_Texture *Mt_lib::renderWrapped(SDL_Renderer *renderer, const std::string &text, TTF_Font *font, Uint32 wrapLenght, SDL_Rect &src, std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)> TTF_RenderFunction)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(font, text.c_str(), Mt_lib::color(0, 0, 0), wrapLenght);
	if (textSurface != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);

		SDL_QueryTexture(texture, nullptr, nullptr, &src.w, &src.h);
	}
	return texture;
}

void Mt_lib::replaceAll(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void Mt_lib::confine(int &destCoord, int &srcCoord, int &destSize, int &srcSize, int maxSize, int boxCoordinate, int boxSize)
{
	if (destCoord >= boxCoordinate && destCoord + maxSize < boxCoordinate + boxSize)
	{
		srcCoord = 0;
		destSize = srcSize = maxSize;
	}
	else if (destCoord < boxCoordinate && destCoord + maxSize > boxCoordinate)
	{
		destSize = srcSize = (destCoord + maxSize) - boxCoordinate;
		srcCoord = boxCoordinate - destCoord;
		destCoord = boxCoordinate;
	}
	else if (destCoord < boxCoordinate + boxSize && destCoord + maxSize > boxCoordinate + boxSize)
	{
		destSize = srcSize = (boxCoordinate + boxSize) - destCoord;
	}
}
