#ifndef FFA654A9_41EE_4A84_BF89_4157CBE82750
#define FFA654A9_41EE_4A84_BF89_4157CBE82750

#include <string>
#include <functional>

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_util.hpp"
#include "mt_point.hpp"
#include "mt_widget.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Button : public Widget
	{
	private:
		WIDGET_CLASS("BUTTON");

		bool pressed = false;
		int scale = 1;

		bool clicked = false;

		Uint32 lastPressed;

		Button(Widget &widget);
		Button(Window &window, int x, int y, int w, int h);
		Button(const Button &) = delete;

		void init();

	public:
		static Button &create(Widget &widget);
		static Button &create(Window &window, int x, int y, int w, int h);

		~Button();

		Label *label = nullptr;

		Event<void> function;

		SDL_Point clickOffset;
		unsigned repeatInterval = 300;

		bool actioned() const;

		void fitH(int padding = 4);

		void operator()();

		void handleEvent() override;
		void update() override;
		void draw() override;
	};
}
#endif /* FFA654A9_41EE_4A84_BF89_4157CBE82750 */
