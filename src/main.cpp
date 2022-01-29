#include <iostream>

#include "mt_application.hpp"
#include "mt_window.hpp"

#include "mt_button.hpp"
#include "mt_label.hpp"
#include "mt_checkbox.hpp"
#include "mt_bitmap.hpp"
#include "mt_vector.hpp"
#include "mt_textinput.hpp"

int main(/*int argc, char *argv[]*/)
{
	Mt_application app("Application");
	app.window.setSize(600, 600);

	app.window.setIcon("assets/icon.png");

	return app();
}
