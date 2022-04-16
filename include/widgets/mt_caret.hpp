#ifndef AE945488_D359_47A3_81D7_5EC2E140A8A2
#define AE945488_D359_47A3_81D7_5EC2E140A8A2

#include "mt_application.hpp"
#include "mt_widget.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Caret : public Widget
	{
	private:
		SDL_Texture *texture = nullptr;

		const unsigned short blinkInterval = 500;
		Uint32 lastBlink = 0;

		bool show = true;

		Caret(Widget &widget);
		Caret(const Caret &) = delete;

		TTF_Font *renderedFont = nullptr;
		Color renderedColor;

		void init() override;

		void setColorMod();

	public:
		static Caret &create(Widget &widget);

		~Caret();

		void update() override;
		void draw() override;
	};
}

#endif /* AE945488_D359_47A3_81D7_5EC2E140A8A2 */
