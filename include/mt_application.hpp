#ifndef B8D83158_3027_49A9_BAA8_0A7468DAD697
#define B8D83158_3027_49A9_BAA8_0A7468DAD697

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <cstdlib>
#include <sstream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define eRESET "[0m"
#define eBOLD "[1m"
#define eDIM "[2m"
#define eUNDERLINED "[4m"
#define eBLINK "[5m"
#define eREVERSE "[7m"
#define eHIDDEN "[8m"

#define fRESET "[39m"
#define fBLACK "[30m"
#define fRED "[31m"
#define fGREEN "[32m"
#define fYELLOW "[33m"
#define fBLUE "[34m"
#define fMAGENTA "[35m"
#define fCYAN "[36m"
#define fLIGHT_GRAY "[37m"
#define fDARK_GRAY "[90m"
#define fLIGHT_RED "[91m"
#define fLIGHT_GREEN "[92m"
#define fLIGHT_YELLOW "[93m"
#define fLIGHT_BLUE "[94m"
#define fLIGHT_MAGENTA "[95m"
#define fLIGHT_CYAN "[96m"
#define fWHITE "[97m"

#define bRESET "[49m"
#define bBLACK "[40m"
#define bRED "[41m"
#define bGREEN "[42m"
#define bYELLOW "[43m"
#define bBLUE "[44m"
#define bMAGENTA "[45m"
#define bCYAN "[46m"
#define bLIGHT_GRAY "[47m"
#define bDARK_GRAY "[100m"
#define bLIGHT_RED "[101m"
#define bLIGHT_GREEN "[102m"
#define bLIGHT_YELLOW "[103m"
#define bLIGHT_BLUE "[104m"
#define bLIGHT_MAGENTA "[105m"
#define bLIGHT_CYAN "[106m"
#define bWHITE "[107m"

#define aRESET eRESET << bRESET << fRESET

#define colorspace(command, color) \
	std::cout << color;            \
	command;                       \
	std::cout << aRESET;

#define colorstream(stream, color) \
	color << stream << aRESET

#ifdef DEBUG
#define Debug(x) std::cout << colorstream("[DEBUG] " << __PRETTY_FUNCTION__ << ": " << x, fDARK_GRAY) << std::endl
#define DebugFrame(renderer, destR, color) drawRectangle(renderer, destR, color)
#else
#define Debug(x)
#define DebugFrame(renderer, destR, color)
#endif
#define Log(x) std::cout << "[LOG] " << __FUNCTION__ << ": " << x << std::endl
#define Warn(x) std::cout << colorstream("[WARN] " << __FUNCTION__ << ": " << x, fLIGHT_YELLOW) << std::endl
#define Error(x) std::cout << colorstream("[ERROR] " << __FUNCTION__ << ": " << x, fRED) << std::endl
#define Critical(x) std::cout << colorstream("[CRITICAL] " << __FUNCTION__ << ": " << x, bWHITE << fRED) << std::endl

#define return_if(x) \
	if (x)           \
	{                \
		return;      \
	}

#define BLACK_UP_POINTING_TRIANGLE "\u25B2"
#define BLACK_DOWN_POINTING_TRIANGLE "\u25BC"
#define BLACK_LEFT_POINTING_TRIANGLE "\u25C0"
#define BLACK_RIGHT_POINTING_TRIANGLE "\u25B6"

template <typename... Args>
using Event = std::function<void(Args...)>;

class Mt_window;

class Mt_application
{
private:
	bool running = false;

	std::map<std::pair<std::string, int>, TTF_Font *> fonts;

	Uint32 fStart = 0;
	Uint32 frameTime = 0;

	unsigned frameDelay;

	const unsigned short targetFPS = 60;

public:
	std::map<std::string, Mt_window *> windows;
	SDL_Event event;

	TTF_Font *getFont(const std::string &path, int fontSize);

	Mt_application(const std::string &);
	~Mt_application();

	Mt_window &window;

	int operator()();
	int run();
	void stop();
};

#endif /* B8D83158_3027_49A9_BAA8_0A7468DAD697 */
