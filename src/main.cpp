#include <iostream>

#include "mt_application.hpp"

#include "mt_button.hpp"
#include "mt_label.hpp"
#include "mt_checkbox.hpp"
#include "mt_combobox.hpp"
#include "mt_textbox.hpp"
#include "mt_textarea.hpp"

int main(/*int argc, char *argv[]*/)
{
	Mt_application app("Application");

	app.window.setIcon("assets/icon.png");

	auto &window = Mt_window::create(app, "Dialog 01", 150, 50);
	window.setFlags(WINDOW_BORDERLESS, WINDOW_DRAGGABLE, WINDOW_ALWAYS_ON_TOP, WINDOW_HIDDEN);
	window.border = true;

	auto &btn1 = Mt_button::create(app.window, app.window.getW() / 2, app.window.getH() / 2, 100, 20);
	btn1.function = [&]()
	{
		window.show();
	};
	btn1.label->text = "Show dialog";
	btn1.geometry->setAnchor(Mt_geometry::middle_center);

	auto &btn2 = Mt_button::create(window, window.getW() / 2, window.getH() / 2, 20, 20);
	btn2.function = [&]()
	{
		window.hide();
	};
	btn2.label->text = "OK";
	btn2.geometry->setAnchor(Mt_geometry::middle_center);

	return app();
}