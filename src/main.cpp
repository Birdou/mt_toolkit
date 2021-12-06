#include <iostream>

#include "mt_application.hpp"

#include "mt_button.hpp"
#include "mt_checkbox.hpp"
#include "mt_combobox.hpp"
#include "mt_textbox.hpp"
#include "mt_textarea.hpp"

void foo()
{
	for (int i = 0; i < 10; ++i)
		std::cout << "oi ";
	std::cout << std::endl;
}

int main(/*int argc, char *argv[]*/)
{
	Mt_application app;
	app.init();

	auto window = app.createWindow("Main window", 600, 400);
	window->setIcon("assets/icon.png");

	Mt_label label1(*window, 0, 0);
	label1.text = "OLÁ MUNDO";

	Mt_comboBox cbox1(*window, window->getW() / 2, window->getH() / 2, 200, 25);
	cbox1.geometry->setAnchor(Mt_geometry::middle_center);
	cbox1.addOption("opção 1", []() {});
	cbox1.addOption("opção 2", []() {});

	auto window2 = app.createWindow("Second window", 600, 400);
	Mt_label label2(*window2, 0, 0);
	label2.text = "OLÁ MUNDORRR";

	return app();
}