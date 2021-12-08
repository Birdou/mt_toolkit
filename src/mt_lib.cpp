
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
		Debug(texture << ": " << SDL_GetError());
		SDL_ClearError();
	}
	return tex;
}

TTF_Font *Mt_lib::loadFont(const std::string &path, int fontSize)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);
	if (font)
	{
		return font;
	}
	else
	{
		Error("Invalid font. (did you enter the correct path?)");
		return nullptr;
	}
}

void Mt_lib::drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
	if (!texture)
	{
		drawFillRectangle(renderer, *dest, color(255, 0, 0, 122));
		Debug("invalid texture");
	}
	else
	{
		SDL_RenderCopy(renderer, texture, src, dest);
	}

	DebugFrame(renderer, *dest, color(255, 0, 0));
}

void Mt_lib::drawRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color)
{
	SDL_Color renderColor;
	SDL_GetRenderDrawColor(renderer, &renderColor.r, &renderColor.g, &renderColor.b, &renderColor.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}
void Mt_lib::drawFillRectangle(SDL_Renderer *renderer, const SDL_Rect &dest, const SDL_Color &color)
{
	SDL_Color renderColor;
	SDL_GetRenderDrawColor(renderer, &renderColor.r, &renderColor.g, &renderColor.b, &renderColor.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest);
	SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}

SDL_Texture *Mt_lib::renderText(SDL_Renderer *renderer, const std::string &text, Mt_font *font, Mt_geometry *geometry, DrawFunction TTF_RenderFunction)
{
	TTF_Font *ttf = font->getFont();
	if (ttf == nullptr)
	{
		Warn("Invalid font.");
		return nullptr;
	}

	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(ttf, text.c_str(), font->color);
	if (textSurface != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
		if (!texture)
		{
			Error("Unable to create texture from rendered text! SDL Error: " << SDL_GetError());
			return nullptr;
		}

		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		geometry->setW(w);
		geometry->setH(h);
	}
	return texture;
}

SDL_Texture *Mt_lib::renderWrapped(SDL_Renderer *renderer, const std::string &text, Mt_font *font, Mt_geometry *geometry, Uint32 wrapLenght, DrawFunctionWrapped TTF_RenderFunction)
{
	TTF_Font *ttf = font->getFont();
	if (ttf == nullptr)
	{
		Debug("Invalid font.");
		return nullptr;
	}

	SDL_Texture *texture = nullptr;
	SDL_Surface *textSurface = TTF_RenderFunction(ttf, text.c_str(), font->color, wrapLenght);
	if (textSurface != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
		if (texture == NULL)
		{
			Error("Unable to create texture from rendered text! SDL Error: " << SDL_GetError());
			return nullptr;
		}

		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		geometry->setW(w);
		geometry->setH(h);
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

void Mt_lib::confineX(Mt_geometry *geometry, const SDL_Rect &box)
{
	if (geometry->destR.x >= box.x && geometry->destR.x + geometry->getW() < box.x + box.w)
	{
		geometry->srcR.x = 0;
		geometry->destR.w = geometry->srcR.w = geometry->getW();
	}
	else if (geometry->destR.x < box.x && geometry->destR.x + geometry->getW() > box.x)
	{
		geometry->destR.w = geometry->srcR.w = (geometry->destR.x + geometry->getW()) - box.x;
		geometry->srcR.x = box.x - geometry->destR.x;
		geometry->destR.x = box.x;
	}
	else if (geometry->destR.x < box.x + box.w && geometry->destR.x + geometry->getW() > box.x + box.w)
	{
		geometry->destR.w = geometry->srcR.w = (box.x + box.w) - geometry->destR.x;
	}
}
void Mt_lib::confineY(Mt_geometry *geometry, const SDL_Rect &box)
{
	if (geometry->destR.y >= box.y && geometry->destR.y + geometry->getH() < box.y + box.h)
	{
		geometry->srcR.y = 0;
		geometry->destR.h = geometry->srcR.h = geometry->getH();
	}
	else if (geometry->destR.y < box.y && geometry->destR.y + geometry->getH() > box.y)
	{
		geometry->destR.h = geometry->srcR.h = (geometry->destR.y + geometry->getH()) - box.y;
		geometry->srcR.y = box.y - geometry->destR.y;
		geometry->destR.y = box.y;
	}
	else if (geometry->destR.y < box.y + box.h && geometry->destR.y + geometry->getH() > box.y + box.h)
	{
		geometry->destR.h = geometry->srcR.h = (box.y + box.h) - geometry->destR.y;
	}
}