#ifndef BB5DE797_DB7E_4395_ACCD_25EE508CE326
#define BB5DE797_DB7E_4395_ACCD_25EE508CE326

#include <map>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_textinput.hpp"
#include "mt_button.hpp"

class Mt_comboBox : public Mt_widget
{
private:
	std::map<std::string, Mt_button *> options;

	bool show = false;

	Mt_comboBox(Mt_window &window, int x, int y, int w, int h);
	Mt_comboBox(const Mt_comboBox &) = delete;

	void init() override;

	void turnVisible();

public:
	static Mt_comboBox &create(Mt_window &window, int x, int y, int w, int h);

	~Mt_comboBox();

	Mt_textbox *textbox = nullptr;
	Mt_button *button = nullptr;

	void addOption(const std::string &string);

	void updatePosition();
	void updateOptions();

	std::string option();

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* BB5DE797_DB7E_4395_ACCD_25EE508CE326 */
