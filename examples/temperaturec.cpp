#include <iostream>

#include "../include/mt_application.hpp"
#include "../include/mt_textbox.hpp"
#include "../include/mt_textarea.hpp"
#include "../include/mt_checkbox.hpp"
#include "../include/mt_label.hpp"
#include "../include/mt_combobox.hpp"

double convert(const std::string &T1, const std::string &T1S, const std::string &T2S)
{
	std::stringstream stream(T1);
	double t;
	stream >> t;
	if (T1S == "Grau Celsius")
	{
		if (T2S == "Grau Fahrenheit")
		{
			return (t * (9 / 5.0f)) + 32;
		}
		else if (T2S == "Kelvin")
		{
			return t + 275.15;
		}
	}
	else if (T1S == "Grau Fahrenheit")
	{
		if (T2S == "Grau Celsius")
		{
			return (t - 32) * (5 / 9.0f);
		}
		else if (T2S == "Kelvin")
		{
			return (t - 32) * (5 / 9.0f) + 273.15;
		}
	}
	else if (T1S == "Kelvin")
	{
		if (T2S == "Grau Celsius")
		{
			return t - 273.15;
		}
		else if (T2S == "Grau Fahrenheit")
		{
			return ((t - 273.15) * (9 / 5.0f)) + 32;
		}
	}

	return t;
}

int main(int argc, char *argv[])
{
	Mt_application app(200, 200, "Application");
	app.renderColor = Mt_lib::color(255, 255, 255);
	app.init();

	Mt_textbox tbox1(app, app.getWidth() / 2, 20, 150, 25, 14, Mt_lib::color(0, 0, 0));
	Mt_comboBox cbox1(app, app.getWidth() / 2, 45, 150, 25, 14, Mt_lib::color(0, 0, 0));
	cbox1.textbox->editable = false;

	Mt_textbox tbox2(app, app.getWidth() / 2, 80, 150, 25, 14, Mt_lib::color(0, 0, 0));
	Mt_comboBox cbox2(app, app.getWidth() / 2, 105, 150, 25, 14, Mt_lib::color(0, 0, 0));
	cbox2.textbox->editable = false;

	auto convertFunction = [&tbox1, &tbox2, &cbox1, &cbox2]()
	{
		std::cout << "converting..." << std::endl;
		tbox2.str(std::to_string(convert(tbox1.str(), cbox1.textbox->str(), cbox2.textbox->str())));
	};

	cbox1.addOption("Grau Celsius", convertFunction);
	cbox1.addOption("Grau Fahrenheit", convertFunction);
	cbox1.addOption("Kelvin", convertFunction);

	cbox2.addOption("Grau Celsius", convertFunction);
	cbox2.addOption("Grau Fahrenheit", convertFunction);
	cbox2.addOption("Kelvin", convertFunction);

	tbox1.onTextModified = convertFunction;

	app.run();

	return 0;
}