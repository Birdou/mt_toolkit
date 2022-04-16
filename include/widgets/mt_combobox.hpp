#ifndef BB5DE797_DB7E_4395_ACCD_25EE508CE326
#define BB5DE797_DB7E_4395_ACCD_25EE508CE326

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_textbox.hpp"
#include "mt_button.hpp"

#include <map>

namespace TOOLKIT_NAMESPACE
{
	class ComboBox : public Widget
	{
	private:
		std::map<std::string, Button *> options;

		bool show = false;

		ComboBox(Window &window, int x, int y, int w, int h);
		ComboBox(const ComboBox &) = delete;

		void init() override;

		void turnVisible();

	public:
		static ComboBox &create(Window &window, int x, int y, int w, int h);

		~ComboBox();

		Textbox *textbox = nullptr;
		Button *button = nullptr;

		void addOption(const std::string &string);

		void updatePosition();
		void updateOptions();

		std::string option();

		void handleEvent() override;
		void update() override;
		void draw() override;
	};
}
#endif /* BB5DE797_DB7E_4395_ACCD_25EE508CE326 */
