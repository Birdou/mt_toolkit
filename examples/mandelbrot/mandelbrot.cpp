#include <iostream>
#include <sstream>
#include <thread>
#include <limits>

#include "mt_application.hpp"

#include "mt_button.hpp"
#include "mt_label.hpp"
#include "mt_checkbox.hpp"
#include "mt_bitmap.hpp"
#include "mt_vector.hpp"
#include "mt_textinput.hpp"

using ld = double;

int modifier = 7;
ld mapToReal(int x, int w, ld minR, ld maxR)
{
	return x * ((maxR - minR) / w) + minR;
}
ld mapToImaginary(int y, int h, ld minI, ld maxI)
{
	return y * ((maxI - minI) / h) + minI;
}
int findMandelbrot(ld cr, ld ci, int maxN)
{
	int i = 0;
	ld zr = 0., zi = 0.;
	while (i < maxN && zr * zr + zi * zi < 4.)
	{
		ld tmp = zr * zr - zi * zi + cr;
		zi = 2. * zr * zi + ci;
		zr = tmp;
		i++;
	}
	return i;
}
void calc(Mt_pixelgrid &bitmap, ld minR, ld maxR, ld minI, ld maxI, int maxN, int row, int column, size_t nElements)
{
	size_t elements = 0;
	for (size_t j = row; j < bitmap.height(); ++j)
	{
		for (size_t i = elements == 0 ? column : 0; i < bitmap.width(); ++i)
		{
			ld cr = mapToReal(i, bitmap.width(), minR, maxR);
			ld ci = mapToImaginary(j, bitmap.height(), minI, maxI);
			int n = findMandelbrot(cr, ci, maxN);

			int value = (n * modifier) % 360;
			bitmap.at(i, j).hsl(value, n == 1 ? 0 : 1, n == 360 ? 0 : .5);

			elements++;
			if (elements == nElements)
				return;
		}
	}
}
void updateGrid(Mt_pixelgrid &bitmap, ld minR, ld maxR, ld minI, ld maxI, int maxN = 360)
{
	std::vector<std::thread *> threads;
	const size_t tElements = bitmap.width() * bitmap.height();
	const size_t nElements = ceil(tElements / 4.);
	for (size_t i = 0, j = 0; i < tElements; i += nElements, ++j)
	{
		threads.emplace_back(new std::thread(calc,
											 std::ref(bitmap),
											 minR, maxR,
											 minI, maxI,
											 maxN,
											 i / bitmap.width(),
											 i % bitmap.width(),
											 nElements));
	}
	for (auto thread : threads)
	{
		thread->join();
		delete thread;
	}
}
ld getMaxI(ld minR, ld maxR, ld minI, size_t width, size_t height)
{
	return minI + (maxR - minR) * height / width;
}
void updateLabels(ld minR, ld maxR, ld minI, Mt_textbox &minRbox, Mt_textbox &maxRbox, Mt_textbox &minIbox)
{
	minRbox.str(std::to_string(minR));
	maxRbox.str(std::to_string(maxR));
	minIbox.str(std::to_string(minI));
}

int main(/*int argc, char *argv[]*/)
{
	Mt_application app("Application");
	app.window.setSize(600, 600);

	app.window.setIcon("assets/icon.png");

	int width = 512, height = 512;

	auto &bitmap = Mt_bitmap::create(app.window, app.window.width() / 2, app.window.height() / 2, 512, 512, width, height);
	bitmap.geometry->setAnchor(Mt_geometry::middle_center);
	bitmap.backgroundColor.color = {0, 0, 0, 255};

	ld minR, maxR, minI;
	minR = -2;
	maxR = .8;
	minI = -1.4;

	/* COOL POSITIONS
	minR = -0.74193916988147788481;
	maxR = -0.74193916944320801595;
	minI = 0.14308181879701325906;

	minR = -0.57416981044194772714;
	maxR = -0.5741698104369876052;
	minI = 0.54403553659617335468;
	*/

	ld maxI = getMaxI(minR, maxR, minI, width, height);

	auto &controlsWindow = Mt_window::create(app, "controls", 400, 150);
	controlsWindow.border = true;
	controlsWindow.setFlags(WINDOW_DRAGGABLE, WINDOW_BORDERLESS);

	auto &minRbox = Mt_textbox::create(controlsWindow, 10, 10, 250, 20);
	auto &maxRbox = Mt_textbox::create(controlsWindow, 10, 35, 250, 20);
	auto &minIbox = Mt_textbox::create(controlsWindow, 10, 60, 250, 20);

	updateGrid(bitmap, minR, maxR, minI, maxI);
	updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);

	auto &goBtn = Mt_button::create(controlsWindow, 280, 35, 40, 40);
	goBtn.label->text = "Go";
	goBtn.function = [&]()
	{
		minR = std::stod(minRbox.str());
		maxR = std::stod(maxRbox.str());
		minI = std::stod(minIbox.str());
		maxI = getMaxI(minR, maxR, minI, width, height);

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	Mt_label::create(controlsWindow, 10, 90).text = "File:";
	auto &filenameFild = Mt_textbox::create(controlsWindow, 40, 90, 150, 20);
	auto &saveWidth = Mt_textbox::create(controlsWindow, 10, 120, 80, 20);
	Mt_label::create(controlsWindow, 95, 120).text = "X";
	auto &saveHeight = Mt_textbox::create(controlsWindow, 110, 120, 80, 20);

	auto &modifierField = Mt_textbox::create(controlsWindow, 280, 90, 40, 20);
	modifierField.str(std::to_string(modifier));
	Mt_label::create(controlsWindow, 280, 110).text = "Color modifier";
	modifierField.onTextModified = [&]()
	{
		if (modifierField.str().empty())
		{
			modifier = 1;
		}
		else
		{
			modifier = std::stoi(modifierField.str());
		}
		updateGrid(bitmap, minR, maxR, minI, maxI);
	};

	auto &save = Mt_button::create(controlsWindow, 210, 100, 40, 40);
	save.label->text = "Save";
	save.function = [&]()
	{
		size_t width, height;

		if (saveWidth.str().empty() || saveHeight.str().empty())
		{
			/*TODO
			 - Handle message boxes
			*/
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aviso", "Informe as dimensões da imagem", controlsWindow.window);
			return;
		}
		if (filenameFild.str().empty())
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aviso", "Informe o nome do arquivo", controlsWindow.window);
			return;
		}

		try
		{
			width = std::stoi(saveWidth.str());
			height = std::stoi(saveHeight.str());
		}
		catch (const std::invalid_argument &e)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erro", "Valor inválido", controlsWindow.window);
			return;
		}

		auto img = Mt_pixelgrid(width, height);
		ld maxI = getMaxI(minR, maxR, minI, width, height);
		updateGrid(img, minR, maxR, minI, maxI);
		img.savePNG(filenameFild.str() + ".png");
	};

	auto &zoomIn = Mt_button::create(app.window, 375, 560, 20, 20);
	zoomIn.label->text = BLACK_UP_POINTING_TRIANGLE;
	zoomIn.onClicked = [&]()
	{
		ld deltaR = maxR - minR;
		if (deltaR < 0.0000000000001)
		{
			std::cout << "cannot be more acurate" << std::endl;
			// return;
		}
		ld value = (deltaR / 10) / 2;

		minR += value;
		maxR -= value;
		minI += ((maxI - minI) / 10) / 2;
		maxI = getMaxI(minR, maxR, minI, width, height);

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	int count = 0;

	auto &recordZoomOut = Mt_checkbox::create(app.window, 400, 570, 20);
	Mt_label::create(app.window, 425, 570).text = "Record zoom out";
	auto &zoomOut = Mt_button::create(app.window, 375, 580, 20, 20);
	zoomOut.label->text = BLACK_DOWN_POINTING_TRIANGLE;
	zoomOut.onClicked = [&]()
	{
		ld deltaR = maxR - minR;
		if (deltaR > 2)
			return;
		ld value = (maxR - minR) / 10;
		minR -= value;
		maxR += value;
		minI -= (maxI - minI) / 10;
		maxI = getMaxI(minR, maxR, minI, width, height);

		if (recordZoomOut.checked())
		{
			std::stringstream stream;
			stream << std::setw(3) << std::setfill('0') << count++;
			bitmap.savePNG("gallery/mandelbrot" + stream.str() + ".png");
		}

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	auto &up = Mt_button::create(app.window, app.window.width() / 2, 20, 20, 20);
	up.geometry->setAnchor(Mt_geometry::middle_center);
	up.label->text = BLACK_UP_POINTING_TRIANGLE;
	up.onClicked = [&]()
	{
		ld value = ((maxI - minI) / 10) / 2;
		minI -= value;
		maxI = getMaxI(minR, maxR, minI, width, height);

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};
	auto &down = Mt_button::create(app.window, app.window.width() / 2, 580, 20, 20);
	down.geometry->setAnchor(Mt_geometry::middle_center);
	down.label->text = BLACK_DOWN_POINTING_TRIANGLE;
	down.onClicked = [&]()
	{
		ld value = ((maxI - minI) / 10) / 2;
		minI += value;
		maxI = getMaxI(minR, maxR, minI, width, height);

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};
	auto &left = Mt_button::create(app.window, 20, app.window.height() / 2, 20, 20);
	left.geometry->setAnchor(Mt_geometry::middle_center);
	left.label->text = BLACK_LEFT_POINTING_TRIANGLE;
	left.onClicked = [&]()
	{
		ld value = ((maxR - minR) / 10) / 2;
		minR -= value;
		maxR -= value;

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};
	auto &right = Mt_button::create(app.window, 580, app.window.height() / 2, 20, 20);
	right.geometry->setAnchor(Mt_geometry::middle_center);
	right.label->text = BLACK_RIGHT_POINTING_TRIANGLE;
	right.onClicked = [&]()
	{
		ld value = ((maxR - minR) / 10) / 2;
		minR += value;
		maxR += value;

		updateGrid(bitmap, minR, maxR, minI, maxI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	return app();
}
