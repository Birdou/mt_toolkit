#ifndef FFA654A9_41EE_4A84_BF89_4157CBE82750
#define FFA654A9_41EE_4A84_BF89_4157CBE82750

#include <string>
#include <functional>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_lib.hpp"
#include "mt_point.hpp"
#include "mt_widget.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"

class Mt_button : public Mt_widget
{
private:
	bool pressed = false;
	int scale = 1;

	bool clicked = false;

	Uint32 lastPressed;

	Mt_button(Mt_widget& widget);
	Mt_button(Mt_window& window, int x, int y, int w, int h);
	Mt_button(const Mt_button&) = delete;

	void init();


public:
	static Mt_button& create(Mt_widget& widget);
	static Mt_button& create(Mt_window& window, int x, int y, int w, int h);

	~Mt_button();

	Mt_label* label = nullptr;

	std::function<void()> function = none;

	SDL_Point clickOffset;
	unsigned repeatInterval = 300;

	void updateTextPosition();

	bool actioned() const;

	void fitH(int padding = 4);

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* FFA654A9_41EE_4A84_BF89_4157CBE82750 */
