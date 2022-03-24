#ifndef B8D83158_3027_49A9_BAA8_0A7468DAD697
#define B8D83158_3027_49A9_BAA8_0A7468DAD697

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <cstdlib>
#include <sstream>
#include <mutex>

#include "mt_renderer.hpp"

#define BLACK_UP_POINTING_TRIANGLE "\u25B2"
#define BLACK_DOWN_POINTING_TRIANGLE "\u25BC"
#define BLACK_LEFT_POINTING_TRIANGLE "\u25C0"
#define BLACK_RIGHT_POINTING_TRIANGLE "\u25B6"

class Mt_window;
class Mt_application
{
private:
	bool running = false;

	std::map<std::pair<std::string, int>, TTF_Font*> fonts;
	std::vector<std::thread*> coroutines;

	Uint32 fStart = 0;
	Uint32 frameTime = 0;

	unsigned frameDelay;

	const unsigned short targetFPS = 60;

public:
	Mt_application(const std::string& title, int width, int height, int flags = 0);
	~Mt_application();

	std::mutex mutex;

	SDL_Event event;

	std::unique_ptr<Mt_window> window;

	template <typename T, typename... Args>
	void newCoroutine(T function, Args... args)
	{
		coroutines.emplace_back(new std::thread(function, args...));
	}

	TTF_Font* getFont(const std::string& path, int fontSize);

	bool isRunning() const noexcept { return running; }

	int operator()();
	int run();
	void stop();
};

#endif /* B8D83158_3027_49A9_BAA8_0A7468DAD697 */
