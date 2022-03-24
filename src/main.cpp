#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "mt_button.hpp"
#include "mt_label.hpp"
#include "mt_textinput.hpp"
#include "mt_bitmap.hpp"
#include "mt_combobox.hpp"
#include "mt_checkbox.hpp"
#include "mt_scrollarea.hpp"

using namespace std::chrono;
int main(int argc, char* argv[])
{
	Mt_application app("Application", 600, 600);

	auto& tbox = Mt_textbox::create(*app.window, app.window->width() / 2, 70, 200, 25);
	// auto& tarea = Mt_textbox::create(*app.window, app.window->width() / 2, app.window->height(), 200, 200);
	// tarea.geometry->setAnchor(bottom_center);

	auto& createWindow = Mt_button::create(*app.window, 10, 10, 100, 40);
	createWindow.label->text = "Botão";
	createWindow.font->setFont("assets/fonts/cour.ttf", 19);

	createWindow.function = [&]() {
		auto& wind = app.window->createChild(tbox.str(), tbox.str(), 250, 200);
		auto& label = Mt_label::create(wind, 10, 10, 200, 80);
		label.text = "Olá";
	};

	auto& createWindow2 = Mt_button::create(*app.window, 10, 80, 100, 40);
	createWindow2.label->text = "Botão 2";
	createWindow2.function = [&]() {
		//app.window->getChildById(tbox.str()).destroy();
		for (auto c : tbox.str())
			std::cout << (int)c << " ";
		std::cout << std::endl;
	};


	return app();
}
