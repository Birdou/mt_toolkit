#ifndef DAB8A644_4736_40F5_A341_651834B4D009
#define DAB8A644_4736_40F5_A341_651834B4D009

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_widget.hpp"
#include "mt_util.hpp"
#include "mt_point.hpp"
#include "mt_font.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Label : public Widget
	{
	private:
		WIDGET_CLASS("LABEL");

		SDL_Texture *iconTexture = nullptr;
		SDL_Texture *textTexture = nullptr;

		bool wrap = false;

		Label(Window &window, int x, int y, int w, int h);
		Label(Window &window, int x, int y);
		Label(Widget &widget);
		Label(const Label &) = delete;

		void init();

		Color renderedColor;
		std::string renderedText;
		TTF_Font *renderedFont = nullptr;

		void setColorMod();

	public:
		static Label &create(Window &window, int x, int y, int w, int h);
		static Label &create(Window &window, int x, int y);
		static Label &create(Widget &widget);

		~Label();

		std::string text;
		bool autoupdate = true;

		float scale = 1;

		void loadIcon(const std::string &path);

		void handleEvent() override;
		void update() override;
		void draw() override;
	};
}

#endif /* DAB8A644_4736_40F5_A341_651834B4D009 */
