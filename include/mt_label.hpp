#ifndef DAB8A644_4736_40F5_A341_651834B4D009
#define DAB8A644_4736_40F5_A341_651834B4D009

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_point.hpp"
#include "mt_font.hpp"

class Mt_label : public Mt_widget
{
private:
	SDL_Texture *texture = nullptr;

	bool wrap = false;

	Mt_label(Mt_window &window, int x, int y, int w, int h);
	Mt_label(Mt_window &window, int x, int y);
	Mt_label(Mt_widget &widget);
	Mt_label(const Mt_label &) = delete;

	void init();

	std::string textRendered;
	Mt_color renderedColor;

	void setColorMod();

public:
	static Mt_label &create(Mt_window &window, int x, int y, int w, int h);
	static Mt_label &create(Mt_window &window, int x, int y);
	static Mt_label &create(Mt_widget &widget);

	~Mt_label();

	std::string text;
	bool autoupdate = true;

	void loadIcon(const std::string &path);

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* DAB8A644_4736_40F5_A341_651834B4D009 */
