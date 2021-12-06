#include <iostream>

#include "mt_application.hpp"

#include "mt_button.hpp"
#include "mt_checkbox.hpp"
#include "mt_combobox.hpp"
#include "mt_textbox.hpp"
#include "mt_textarea.hpp"

int main(/*int argc, char *argv[]*/)
{
	Mt_application app(600, 400, "Application");
	app.targetFPS = -1;
	app.init();

	app.setIcon("assets/icon.png");

	Mt_comboBox cbox1(app, app.getW() / 2, app.getH() / 2, 200, 25);
	cbox1.geometry->setAnchor(Mt_geometry::middle_center);
	cbox1.addOption("opção 1", []() {});
	cbox1.addOption("opção 2", []() {});

	// Mt_widget::defaultFont = "assets/fonts/cour.ttf";
	// Mt_widget::defaultFontSize = 22;

	// Mt_textarea tarea1(app, app.getW() / 2, app.getH() / 2, 300, 300);
	// tarea1.geometry->setAnchor(Mt_geometry::middle_center);

	// Mt_textbox tbox1(app, 10, 30, 100, 20);

	// Mt_label label1(app, 0, 0);
	// label1.geometry->setAnchor(Mt_geometry::top_left); // This is the default, it doesn't need to be explicit.
	// label1.text = "TOP LEFT CORNER";

	// Mt_label label2(app, app.getW(), 0);
	// label2.geometry->setAnchor(Mt_geometry::top_right);
	// label2.text = "TOP RIGHT CORNER";

	// Mt_label label3(app, 0, app.getH());
	// label3.geometry->setAnchor(Mt_geometry::bottom_left);
	// label3.text = "BOTTOM LEFT CORNER";

	// Mt_label label4(app, app.getW(), app.getH());
	// label4.geometry->setAnchor(Mt_geometry::bottom_right);
	// label4.text = "BOTTOM RIGHT CORNER";

	// int fontSize = label4.font->getSize();
	// Mt_button btn1(app, 40, 60, 20, 20,
	// 			   [&]()
	// 			   {
	// 				   label4.font->setFont("assets/fonts/arial.ttf", ++fontSize);
	// 			   });
	// btn1.label->text = "+";
	// Mt_button btn2(app, 40, 100, 45, 40,
	// 			   [&]()
	// 			   {
	// 				   label4.font->setFont("assets/fonts/arial.ttf", --fontSize);
	// 			   });
	// btn2.label->text = "-";

	return app.run();
}