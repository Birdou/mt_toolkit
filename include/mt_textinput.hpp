#ifndef C26CC945_6DCC_4AC9_AE1F_9F84A853E863
#define C26CC945_6DCC_4AC9_AE1F_9F84A853E863

#include <vector>
#include <algorithm>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_caret.hpp"
#include "mt_lib.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"

class Mt_textinput : public Mt_widget
{
protected:
	Mt_label *input = nullptr;
	Mt_caret *caret = nullptr;

	size_t caretPos_x = 0;
	size_t caretPos_y = 0;
	int text_x = 0;
	int text_y = 0;

	Mt_textinput(Mt_widget &widget);
	Mt_textinput(Mt_window &window, int x, int y, int w, int h);
	Mt_textinput(const Mt_textinput &) = delete;

	void init();

	virtual void updateCaretPosition();

	virtual void pointCursor();

	virtual void copy();
	virtual void paste();
	virtual void wheel();
	virtual void enter();
	virtual void backspace();
	virtual void del();
	virtual void up();
	virtual void left();
	virtual void right();
	virtual void down();
	virtual void end();
	virtual void home();
	virtual void tab();
	virtual void textInput();

public:
	bool editable = true;
	bool password = false;

	virtual ~Mt_textinput();

	virtual std::string str() const;
	virtual void str(const std::string &text);

	void handleMouse();

	void handleEvent() override;
	void update() override;
	void draw() override;
};

class Mt_textbox : public Mt_textinput
{
private:
	Mt_textbox(Mt_widget &widget);
	Mt_textbox(Mt_window &window, int x, int y, int w, int h);
	Mt_textbox(const Mt_textbox &) = delete;

public:
	static Mt_textbox &create(Mt_widget &widget);
	static Mt_textbox &create(Mt_window &window, int x, int y, int w, int h);

	~Mt_textbox();
};

class Mt_textarea : public Mt_textinput
{
private:
	std::vector<Mt_label *> lines;

	Mt_textarea(Mt_window &window, int x, int y, int w, int h);
	Mt_textarea(const Mt_textarea &) = delete;

	void pointCursor() override;

	void updateLines();

	void wheel() override;
	void enter() override;
	void backspace() override;
	void del() override;
	void up() override;
	void down() override;
	void tab() override;

public:
	static Mt_textarea &create(Mt_window &window, int x, int y, int w, int h);

	~Mt_textarea();

	std::string str() const override;
	void str(const std::string &str) override;

	Mt_label *newLine(const std::string &content = "");

	void update() override;
	void draw() override;
};

#endif /* C26CC945_6DCC_4AC9_AE1F_9F84A853E863 */
