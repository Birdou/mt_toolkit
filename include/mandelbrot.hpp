#include <iostream>
#include <sstream>
#include <thread>
#include <limits>

#include "mt_application.hpp"
#include "mt_button.hpp"
#include "mt_label.hpp"
#include "mt_checkbox.hpp"
#include "mt_bitmap.hpp"
#include "mt_point.hpp"
#include "mt_textinput.hpp"

using ld = long double;

int modifier = 7;
int maxN = 768;
bool negative = false;
bool monochromatic = false;

void calc(Mt_pixelgrid &bitmap, const ld &minR, const ld &maxR, const ld &minI, const ld &maxI, int row, int column, size_t nElements)
{
	size_t elements = 0;
	for (size_t j = row; j < bitmap.height(); ++j)
	{
		for (size_t i = elements == 0 ? column : 0; i < bitmap.width(); ++i)
		{
			ld cr = i * ((maxR - minR) / bitmap.width()) + minR;
			ld ci = j * ((maxI - minI) / bitmap.height()) + minI;

			int n = 0;
			ld zr = 0.;
			ld zi = 0.;
			ld x2 = 0.;
			ld y2 = 0.;
			while (x2 + y2 <= 4. && n < maxN)
			{
				zi = 2. * zr * zi + ci;
				zr = x2 - y2 + cr;
				x2 = zr * zr;
				y2 = zi * zi;
				n++;
			}

			// int value = (n * modifier) % 360;
			// bitmap.at(i, j).hsv(value, 1, n == 360 ? 0 : 1);
			bitmap.at(i, j).hex((n * modifier) % 16777216);

			elements++;
			if (elements == nElements)
				return;
		}
	}
}
void updateGrid(Mt_pixelgrid &bitmap, ld minR, ld maxR, ld minI)
{
	std::vector<std::thread *> threads;
	const size_t tElements = bitmap.width() * bitmap.height();
	const size_t nElements = ceil(tElements / 4.);

	const ld maxI = minI + (maxR - minR) * bitmap.height() / bitmap.width();
	for (size_t i = 0, j = 0; i < tElements; i += nElements, ++j)
	{
		threads.emplace_back(new std::thread(calc,
											 std::ref(bitmap),
											 std::ref(minR), std::ref(maxR),
											 std::ref(minI), std::ref(maxI),
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
	return minI + (maxR - minR) * (ld)height / (ld)width;
}

void updateLabels(ld minR, ld maxR, ld minI, Mt_textbox &minRbox, Mt_textbox &maxRbox, Mt_textbox &minIbox)
{
	minRbox.str(std::to_string(minR));
	maxRbox.str(std::to_string(maxR));
	minIbox.str(std::to_string(minI));
}

int mandelbrot()
{
	Mt_application app("Application", 600, 600);
	app.window.setIcon("assets/icon.png");

	int width = 512, height = 512;

	auto &bitmap = Mt_bitmap::create(app.window, app.window.width() / 2, app.window.height() / 2, 512, 512, width, height);
	bitmap.geometry->setAnchor(middle_center);
	bitmap.backgroundColor = Mt_color(0, 0, 0, 255);

	ld minR, maxR, minI;
	minR = -2;
	maxR = .8;
	minI = -1.4;

	auto &controlsWindow = app.window.createChild("controls", "window1", 400, 200, BORDERLESS);
	controlsWindow.border = true;
	controlsWindow.setDraggable();

	auto &minRbox = Mt_textbox::create(controlsWindow, 10, 10, 250, 20);
	auto &maxRbox = Mt_textbox::create(controlsWindow, 10, 35, 250, 20);
	auto &minIbox = Mt_textbox::create(controlsWindow, 10, 60, 250, 20);

	updateGrid(bitmap, minR, maxR, minI);
	updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);

	auto &goBtn = Mt_button::create(controlsWindow, 280, 35, 40, 40);
	goBtn.label->text = "Go";
	goBtn.function = [&]()
	{
		minR = std::stod(minRbox.str());
		maxR = std::stod(maxRbox.str());
		minI = std::stod(minIbox.str());

		updateGrid(bitmap, minR, maxR, minI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	Mt_label::create(controlsWindow, 10, 90).text = "File:";
	auto &filenameFild = Mt_textbox::create(controlsWindow, 40, 90, 150, 20);
	auto &saveWidth = Mt_textbox::create(controlsWindow, 10, 120, 80, 20);
	Mt_label::create(controlsWindow, 95, 120).text = "X";
	auto &saveHeight = Mt_textbox::create(controlsWindow, 110, 120, 80, 20);

	auto &modifierField = Mt_textbox::create(controlsWindow, 280, 90, 40, 20);
	modifierField.str(std::to_string(modifier));
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
		updateGrid(bitmap, minR, maxR, minI);
	};
	Mt_label::create(controlsWindow, 280, 110).text = "Color modifier";
	auto &iterationsField = Mt_textbox::create(controlsWindow, 280, 140, 40, 20);
	iterationsField.str(std::to_string(maxN));
	iterationsField.onTextModified = [&]()
	{
		if (iterationsField.str().empty())
		{
			maxN = 1;
		}
		else
		{
			maxN = std::stoi(iterationsField.str());
		}
		updateGrid(bitmap, minR, maxR, minI);
	};
	Mt_label::create(controlsWindow, 280, 160).text = "N of iterations";

	auto &save = Mt_button::create(controlsWindow, 210, 100, 40, 40);
	save.label->text = "Save";
	save.function = [&]()
	{
		size_t width, height;

		if (saveWidth.str().empty() || saveHeight.str().empty())
		{
			controlsWindow.showSimpleMessageBox("Aviso", "Informe as dimensões da imagem", INFORMATION);
			return;
		}
		if (filenameFild.str().empty())
		{
			controlsWindow.showSimpleMessageBox("Aviso", "Informe o nome do arquivo", INFORMATION);
			return;
		}

		try
		{
			width = std::stoi(saveWidth.str());
			height = std::stoi(saveHeight.str());
		}
		catch (const std::invalid_argument &e)
		{
			controlsWindow.showSimpleMessageBox("Erro", "Valor inválido", ERROR);
			return;
		}

		auto img = Mt_pixelgrid(width, height);
		updateGrid(img, minR, maxR, minI);
		img.savePNG(filenameFild.str() + ".png");
	};

	auto &zoomIn = Mt_button::create(app.window, 375, 560, 20, 20);
	zoomIn.label->text = BLACK_UP_POINTING_TRIANGLE;
	zoomIn.onClicked = [&]()
	{
		ld deltaR = maxR - minR;
		if (deltaR < 0.0000000000001)
		{
			Warn("Can't be more accurate. Continuing to zoom in will result in undefined behaviors.");
			// return;
		}

		ld value = (deltaR / 10) / 2;
		ld maxI = getMaxI(minR, maxR, minI, bitmap.width(), bitmap.height());

		minR += value;
		maxR -= value;
		minI += ((maxI - minI) / 10) / 2;

		updateGrid(bitmap, minR, maxR, minI);
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
		if (deltaR > 8)
			return;

		ld value = (maxR - minR) / 10;
		ld maxI = getMaxI(minR, maxR, minI, bitmap.width(), bitmap.height());
		minR -= value;
		maxR += value;
		minI -= (maxI - minI) / 10;

		if (recordZoomOut.checked())
		{
			std::stringstream stream;
			stream << std::setw(3) << std::setfill('0') << count++;
			bitmap.savePNG("gallery/mandelbrot" + stream.str() + ".png");
		}

		updateGrid(bitmap, minR, maxR, minI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	auto &up = Mt_button::create(app.window, app.window.width() / 2, 20, 20, 20);
	up.geometry->setAnchor(middle_center);
	up.label->text = BLACK_UP_POINTING_TRIANGLE;
	up.onClicked = [&]()
	{
		ld maxI = getMaxI(minR, maxR, minI, bitmap.width(), bitmap.height());
		ld value = ((maxI - minI) / 10) / 2;
		minI -= value;

		updateGrid(bitmap, minR, maxR, minI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};
	auto &down = Mt_button::create(app.window, app.window.width() / 2, 580, 20, 20);
	down.geometry->setAnchor(middle_center);
	down.label->text = BLACK_DOWN_POINTING_TRIANGLE;
	down.onClicked = [&]()
	{
		ld maxI = getMaxI(minR, maxR, minI, bitmap.width(), bitmap.height());
		ld value = ((maxI - minI) / 10) / 2;
		minI += value;

		updateGrid(bitmap, minR, maxR, minI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};
	auto &left = Mt_button::create(app.window, 20, app.window.height() / 2, 20, 20);
	left.geometry->setAnchor(middle_center);
	left.label->text = BLACK_LEFT_POINTING_TRIANGLE;
	left.onClicked = [&]()
	{
		ld value = ((maxR - minR) / 10) / 2;
		minR -= value;
		maxR -= value;

		updateGrid(bitmap, minR, maxR, minI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};
	auto &right = Mt_button::create(app.window, 580, app.window.height() / 2, 20, 20);
	right.geometry->setAnchor(middle_center);
	right.label->text = BLACK_RIGHT_POINTING_TRIANGLE;
	right.onClicked = [&]()
	{
		ld value = ((maxR - minR) / 10) / 2;
		minR += value;
		maxR += value;

		updateGrid(bitmap, minR, maxR, minI);
		updateLabels(minR, maxR, minI, minRbox, maxRbox, minIbox);
	};

	return app();
}