#include <iostream>

#include "mt_application.hpp"

//#include "mt_button.hpp"
#include "mt_checkbox.hpp"
//#include "mt_combobox.hpp"
//#include "mt_textbox.hpp"
#include "mt_textarea.hpp"

int main(/*int argc, char *argv[]*/)
{
	Mt_application app(600, 400, "Application");
	app.init();

	app.setIcon("assets/icon.png");

	Mt_textarea tbox1(app, app.getWidth() / 2, app.getHeight() / 2, 300, 300);
	tbox1.geometry.setAnchor(Mt_geometry::middle_center);

	Mt_checkbox chk1(app, 40, 40, 20);
	chk1.geometry.setAnchor(Mt_geometry::middle_right);

	Mt_label label1(app, 42, 40);
	label1.text = "sim ou não";
	label1.geometry.setAnchor(Mt_geometry::middle_left);

	app.run();

	return 0;
}