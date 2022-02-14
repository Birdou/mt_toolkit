
#include "mt_lib.hpp"

#include "mt_geometry.hpp"

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

void Mt_lib::replaceAll(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void Mt_lib::confineX(Mt_geometry *geometry, const SDL_Rect &box)
{
	if (geometry->destR.x >= box.x && geometry->destR.x + geometry->getW() < box.x + box.w)
	{
		geometry->srcR.x = 0;
		geometry->destR.w = geometry->srcR.w = geometry->getW();
	}
	if (geometry->destR.x < box.x && geometry->destR.x + geometry->getW() > box.x)
	{
		geometry->destR.w = geometry->srcR.w = (geometry->destR.x + geometry->getW()) - box.x;
		geometry->srcR.x = box.x - geometry->destR.x;
		geometry->destR.x = box.x;
	}
	if (geometry->destR.x < box.x + box.w && geometry->destR.x + geometry->getW() > box.x + box.w)
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
	if (geometry->destR.y < box.y && geometry->destR.y + geometry->getH() > box.y)
	{
		geometry->destR.h = geometry->srcR.h = (geometry->destR.y + geometry->getH()) - box.y;
		geometry->srcR.y = box.y - geometry->destR.y;
		geometry->destR.y = box.y;
	}
	if (geometry->destR.y < box.y + box.h && geometry->destR.y + geometry->getH() > box.y + box.h)
	{
		geometry->destR.h = geometry->srcR.h = (box.y + box.h) - geometry->destR.y;
	}
}