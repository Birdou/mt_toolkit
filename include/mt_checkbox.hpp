#ifndef B8EAFDEA_B267_4EC6_896F_9F8F363AFC52
#define B8EAFDEA_B267_4EC6_896F_9F8F363AFC52

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_lib.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"
#include "mt_point.hpp"

class Mt_checkbox : public Mt_widget
{
private:
	// SDL_Rect checkDest;
	bool isChecked = false;
	bool pressed = false;

	Mt_label *check = nullptr;

	Mt_checkbox(Mt_window &window, int x, int y, int size);
	Mt_checkbox(const Mt_checkbox &) = delete;

	void init() override;

public:
	static Mt_checkbox &create(Mt_window &window, int x, int y, int size);

	~Mt_checkbox();

	float checked_size = .7f;

	bool checked();

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* B8EAFDEA_B267_4EC6_896F_9F8F363AFC52 */
