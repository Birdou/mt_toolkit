#ifndef C861D52D_8451_4817_948F_7B9BF1D961DC
#define C861D52D_8451_4817_948F_7B9BF1D961DC

#include <iostream>

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
#define DebugFrame(destR, color) drawRectangle(destR, color)
#else
#define Debug(x)
#define DebugFrame(destR, color)
#endif
#define Log(x) std::cout << "[LOG] " << __FILE__ << ":" << __LINE__ << ": " << x << std::endl
#define Warn(x) std::cout << colorstream("[WARN] " << __FILE__ << ":" << __LINE__ << ": " << x, fLIGHT_YELLOW) << std::endl
#define Error(x) std::cout << colorstream("[ERROR] " << __FILE__ << ":" << __LINE__ << ": " << x, fRED) << std::endl
#define Critical(x) std::cout << colorstream("[CRITICAL] " << __FILE__ << ":" << __LINE__ << ": " << x, bWHITE << fRED) << std::endl

#define return_if(x) \
	if (x)           \
	{                \
		return;      \
	}

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define SDL_PrintIfError(alert)          \
	if (strcmp(SDL_GetError(), "") != 0) \
		alert(SDL_GetError());           \
	SDL_ClearError();

#define SDL_PrintError(alert) \
	alert(SDL_GetError());    \
	SDL_ClearError();

#ifdef _WIN32
#include <windows.h>
#define REQUIRES_ELEVATION                            \
	if (system("net session >nul 2>&1") != 0)         \
	{                                                 \
		SHELLEXECUTEINFO ShExecInfo;                  \
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); \
		ShExecInfo.fMask = 0;                         \
		ShExecInfo.hwnd = NULL;                       \
		ShExecInfo.lpVerb = "runas";                  \
		ShExecInfo.lpFile = argv[0];                  \
		ShExecInfo.lpParameters = "";                 \
		ShExecInfo.lpDirectory = NULL;                \
		ShExecInfo.nShow = SW_SHOW;                   \
		ShExecInfo.hInstApp = NULL;                   \
                                                      \
		ShellExecuteEx(&ShExecInfo);                  \
                                                      \
		return 0;                                     \
	}
#define powershell(command) system(std::string("powershell -Command \" " + std::string(command) + " \"").c_str())
#else
#define REQUIRES_ELEVATION
#endif

#define TOOLKIT_NAMESPACE itk

#endif /* C861D52D_8451_4817_948F_7B9BF1D961DC */
