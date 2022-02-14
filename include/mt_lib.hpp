#ifndef A7276862_9029_49C2_B946_EC319AF33AAE
#define A7276862_9029_49C2_B946_EC319AF33AAE

#include "mt_application.hpp"

#include <string>

class Mt_geometry;

namespace Mt_lib
{
	TTF_Font *loadFont(const std::string &path, int fontSize);

	void replaceAll(std::string &str, const std::string &from, const std::string &to);

	void confineX(Mt_geometry *geometry, const SDL_Rect &box);
	void confineY(Mt_geometry *geometry, const SDL_Rect &box);
}

#endif /* A7276862_9029_49C2_B946_EC319AF33AAE */
