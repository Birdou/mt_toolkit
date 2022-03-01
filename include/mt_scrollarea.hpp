#ifndef E16839BE_0A2F_4866_9CD5_6C81E791C128
#define E16839BE_0A2F_4866_9CD5_6C81E791C128

#include "mt_core.hpp"

#include "mt_widget.hpp"
#include "mt_button.hpp"

#include <vector>

class Mt_scrollarea : public Mt_widget
{
private:
	std::vector<Mt_widget *> widgets;

	SDL_Rect scroll;
	float progress = 0.f;

	Mt_button *up = nullptr;
	Mt_button *down = nullptr;
	Mt_button *scrollbar = nullptr;

	bool anyhover;

	static const short scrollButtonWidth = 15;
	static const short scrollButtonHeight = 17;
	static const short padding = 2;
	SDL_Rect rect;

	Mt_scrollarea(Mt_window &window, int x, int y, int w, int h, int scroll_w, int scroll_h);
	Mt_scrollarea(const Mt_scrollarea &) = delete;

	void init();

	void confineObject(Mt_widget *widget);

public:
	static Mt_scrollarea &create(Mt_window &window, int x, int y, int w, int h, int scroll_w, int scroll_h);
	~Mt_scrollarea();

	void addWidget(Mt_widget &widget);

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* E16839BE_0A2F_4866_9CD5_6C81E791C128 */
