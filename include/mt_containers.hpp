#ifndef E16839BE_0A2F_4866_9CD5_6C81E791C128
#define E16839BE_0A2F_4866_9CD5_6C81E791C128

#include "mt_core.hpp"

#include "mt_widget.hpp"
#include "mt_button.hpp"

#include <vector>
#include <algorithm>

class Mt_container : public Mt_widget, public Mt_parent
{
protected:
	void confineObject(Mt_widget* widget);
	bool anyhover;

	void init();
public:
	Mt_container(Mt_window& window, int x, int y, int w, int h);
	virtual ~Mt_container();

};

class Mt_flex : public Mt_container
{
private:
	class Mt_row : public Mt_widget
	{
	private:
		class Mt_cel : public Mt_widget
		{
		private:
			Mt_row& row;

			void init();

		public:
			Mt_widget* widget = nullptr;
			size_t colspan;
			center alignment = middle_left;
			int padding = 4;
			Mt_cel(Mt_row& row, Mt_widget* widget, size_t colspan);

			~Mt_cel();
			void handleEvent() override;
			void update() override;
			void draw() override;
		};

		Mt_flex& grid;
		void init();


		int lastMaxH = 0;
		int lastTotalSpan = 0;

		std::vector<Mt_cel*> cols;

		friend Mt_flex;
	public:
		Mt_row(Mt_flex& grid);
		~Mt_row();
		int padding = 0;

		void addWidget(Mt_widget& widget, size_t colspan = 1);
		Mt_cel& at(size_t id);
		Mt_cel& operator[](size_t id);

		void handleEvent() override;
		void update() override;
		void draw() override;
	};

	Mt_flex(Mt_window& window, int x, int y, int w, int h);
	Mt_flex(const Mt_flex&) = delete;

	std::vector<Mt_row*> rows;

	bool gupdate = false;
	int lastVirtualX = 0;
	int lastVirtualY = 0;

public:
	static Mt_flex& create(Mt_window& window, int x, int y, int w, int h);
	~Mt_flex();
	int padding = 2;

	Mt_row& createRow();

	void updateGrid();

	Mt_row& at(size_t index);
	Mt_row& operator[](size_t index);

	void handleEvent() override;
	void update() override;
	void draw() override;
};



class Mt_scrollarea;
class Mt_box : public Mt_container
{
private:
	//std::vector<Mt_widget*> widgets;

	Mt_box(Mt_window& window, int x, int y, int w, int h);
	Mt_box(const Mt_box&) = delete;

	friend Mt_scrollarea;
public:
	static Mt_box& create(Mt_window& window, int x, int y, int w, int h);
	~Mt_box();

	//void addWidget(Mt_widget& widget);

	void handleEvent() override;
	void update() override;
	void draw() override;
};

class Mt_scrollarea : public Mt_box
{
private:
	SDL_Rect scroll;
	float progress = 0.f;

	Mt_button* up = nullptr;
	Mt_button* down = nullptr;
	Mt_button* scrollbar = nullptr;

	static const short scrollButtonWidth = 15;
	static const short scrollButtonHeight = 17;
	static const short padding = 2;
	SDL_Rect rect;

	Mt_scrollarea(Mt_window& window, int x, int y, int w, int h, int scroll_w, int scroll_h);
	Mt_scrollarea(const Mt_scrollarea&) = delete;

public:
	static Mt_scrollarea& create(Mt_window& window, int x, int y, int w, int h, int scroll_w, int scroll_h);
	~Mt_scrollarea();

	void handleEvent() override;
	void update() override;
	void draw() override;
};

#endif /* E16839BE_0A2F_4866_9CD5_6C81E791C128 */
