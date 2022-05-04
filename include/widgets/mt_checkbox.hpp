#ifndef B8EAFDEA_B267_4EC6_896F_9F8F363AFC52
#define B8EAFDEA_B267_4EC6_896F_9F8F363AFC52

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_util.hpp"
#include "mt_color.hpp"
#include "mt_label.hpp"
#include "mt_point.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Checkbox : public Widget
	{
	private:
		WIDGET_CLASS("CHECKBOX");

		bool isChecked = false;
		bool pressed = false;

		Label *check = nullptr;

		Checkbox(Window &window, int x, int y, int size);
		Checkbox(const Checkbox &) = delete;

		void init() override;

	public:
		static Checkbox &create(Window &window, int x, int y, int size);

		~Checkbox();

		float checked_size = .7f;

		bool checked();

		void handleEvent() override;
		void update() override;
		void draw() override;
	};
}
#endif /* B8EAFDEA_B267_4EC6_896F_9F8F363AFC52 */
