#include <iostream>

#include "mt_button.hpp"
#include "mt_label.hpp"
#include "mt_textinput.hpp"
#include "mt_bitmap.hpp"
#include "mt_combobox.hpp"
#include "mt_checkbox.hpp"
#include "mt_scrollarea.hpp"

int main(/*int argc, char *argv[]*/)
{
	Mt_application app("Application", 600, 600);

	auto &label = Mt_label::create(app.window, 10, 10);
	label.text = "Olá mundo";
	label.font->color.hex(White);
	label.backgroundColor.hex(Black).opaque();

	auto &btn1 = Mt_button::create(app.window, 10, 40, 50, 20);
	btn1.label->text = "Botão";

	auto &tbox = Mt_textbox::create(app.window, 10, 70, 100, 20);
	auto &tarea = Mt_textarea::create(app.window, 10, 100, 100, 100);
	auto &chkbox = Mt_checkbox::create(app.window, 80, 30, 20);
	auto &cbox = Mt_comboBox::create(app.window, 10, 220, 150, 20);
	cbox.addOption("Opção 1");

	auto &bitmap = Mt_bitmap::create(app.window, 100, 300, 50, 50);
	for (size_t j = 0; j < bitmap.height(); ++j)
		for (size_t i = 0; i < bitmap.width(); ++i)
			bitmap.at(i, j).bw((i + j) * 2);
	bitmap.geometry->setAnchor(middle_center);

	auto &mainscrollArea = Mt_scrollarea::create(app.window, 10, 10, 500, 550, 0, 400);
	mainscrollArea.backgroundColor.hex(Moccasin);

	auto &scrollArea = Mt_scrollarea::create(app.window, 10, 50, 200, 500, 0, 250);
	scrollArea.backgroundColor.hex(AliceBlue);
	scrollArea.addWidget(label);
	scrollArea.addWidget(btn1);
	scrollArea.addWidget(tbox);
	scrollArea.addWidget(tarea);
	scrollArea.addWidget(chkbox);
	scrollArea.addWidget(cbox);
	scrollArea.addWidget(bitmap);

	mainscrollArea.addWidget(scrollArea);

	return app();
}
