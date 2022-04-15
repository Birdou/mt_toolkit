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

template <typename T>
using Function = std::function<T>;

class Mt_textbox : public Mt_widget
{
protected:
	Mt_label* input = nullptr;
	Mt_caret* caret = nullptr;

	size_t caretPos_x = 0;
	size_t caretPos_y = 0;
	int text_x = 0;
	int text_y = 0;

	Mt_textbox(Mt_widget& widget);
	Mt_textbox(Mt_window& window, int x, int y, int w, int h);
	Mt_textbox(const Mt_textbox&) = delete;

	void init();

	virtual void updateCaretPosition();

	virtual void pointCursor();



public:
	static Mt_textbox& create(Mt_widget& widget);
	static Mt_textbox& create(Mt_window& window, int x, int y, int w, int h);

	bool editable = true;
	bool password = false;

	virtual ~Mt_textbox();

	virtual std::string str() const;
	virtual void str(const std::string& text);

	void handleMouse();

	Function<void()> copy = [this]()
	{

		if (SDL_GetModState() & KMOD_CTRL)
		{
			SDL_SetClipboardText(str().c_str());
		}
	};
	Function<void()> paste = [this]()
	{

		if (SDL_GetModState() & KMOD_CTRL)
		{
			const char* text = SDL_GetClipboardText();
			std::string ftext(text);
			Mt_lib::replaceAll(ftext, "\t", "    ");
			str(ftext);
			delete[] text;

			onTextModified();
			pointCursor();
		}
	};
	Function<void()> wheel = [this]()
	{};
	Function<void()> enter = [this]()
	{};
	Function<void()> backspace = [this]()
	{
		if (input->text.size() > 0 && caretPos_x > 0)
		{
			caretPos_x--;
			input->geometry->srcR.x -= window.renderer->substrWidth(font->getFont(), input->text, caretPos_x, 1);
			if (input->geometry->srcR.x < 0)
				input->geometry->srcR.x = 0;
			input->text.erase(input->text.begin() + caretPos_x);

			// Handling accents
			if (caretPos_x > 0)
				if (input->text[caretPos_x - 1] == -61)
				{
					caretPos_x--;
					input->text.erase(input->text.begin() + caretPos_x);
				}

			onTextModified();
			pointCursor();
		}
	};
	Function<void()> del = [this]()
	{
		if (input->text.size() > 0 && caretPos_x < input->text.length())
		{

			if (input->text[caretPos_x] == -61)
			{
				input->text.erase(input->text.begin() + caretPos_x);
				input->text.erase(input->text.begin() + caretPos_x);
			}
			else
			{
				input->text.erase(input->text.begin() + caretPos_x);
			}

			onTextModified();
			pointCursor();
		}
	};
	Function<void()> up = [this]()
	{};
	Function<void()> left = [this]()
	{
		if (caretPos_x > 0)
		{
			caretPos_x--;
		}
		pointCursor();
	};
	Function<void()> right = [this]()
	{
		if (caretPos_x + 1 <= input->text.length())
		{
			caretPos_x++;
		}
		pointCursor();
	};
	Function<void()> down = [this]()
	{};
	Function<void()> end = [this]()
	{
		caretPos_x = input->text.length();
		if (input->geometry->destR.w >= geometry->destR.w)
			text_x = input->geometry->destR.w - geometry->destR.w;
		pointCursor();
	};
	Function<void()> home = [this]()
	{
		caretPos_x = 0;
		text_x = 0;
		pointCursor();
	};
	Function<void()> tab = [this]()
	{};
	Function<void()> textInput = [this]()
	{
		std::string text(window.event.text.text);
		if (!(SDL_GetModState() & KMOD_CTRL && (text[0] == 'c' || text[0] == 'C' || text[0] == 'v' || text[0] == 'V')))
		{
			input->text.insert(caretPos_x, text);
			input->update();

			caretPos_x += text.length();

			onTextModified();
			pointCursor();
		}
	};

	void handleEvent() override;
	void update() override;
	void draw() override;
};

class Mt_textarea : public Mt_textbox
{
private:
	std::vector<Mt_label*> lines;

	Mt_textarea(Mt_window& window, int x, int y, int w, int h);
	Mt_textarea(const Mt_textarea&) = delete;

	void updateCaretPosition() override;

	void pointCursor() override;

	void updateLines();

public:
	static Mt_textarea& create(Mt_window& window, int x, int y, int w, int h);

	~Mt_textarea();

	std::string str() const override;
	void str(const std::string& str) override;

	Mt_label* newLine(const std::string& content = "");

	void update() override;
	void draw() override;
};

#endif /* C26CC945_6DCC_4AC9_AE1F_9F84A853E863 */
