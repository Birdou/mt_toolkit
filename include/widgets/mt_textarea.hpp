#ifndef C26CC945_6DCC_4AC9_AE1F_9F84A853E863
#define C26CC945_6DCC_4AC9_AE1F_9F84A853E863

#include "mt_textbox.hpp"

#include <vector>

namespace TOOLKIT_NAMESPACE
{
	class Textarea : public Textbox
	{
	private:
		WIDGET_CLASS("TEXTAREA");

		std::vector<Label *> lines;

		Textarea(Window &window, int x, int y, int w, int h);
		Textarea(const Textarea &) = delete;

		void updateCaretPosition() override;

		void pointCursor() override;

		void updateLines();

	public:
		static Textarea &create(Window &window, int x, int y, int w, int h);

		~Textarea();

		std::string str() const override;
		void str(const std::string &str) override;
		void append(const std::string &str);

		Label *newLine(const std::string &content = "");

		void update() override;
		void draw() override;
	};
}

#endif /* C26CC945_6DCC_4AC9_AE1F_9F84A853E863 */
