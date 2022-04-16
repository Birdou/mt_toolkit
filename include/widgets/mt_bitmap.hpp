#ifndef F485EFCF_8079_4F33_91F7_A134EA3822B3
#define F485EFCF_8079_4F33_91F7_A134EA3822B3

#include "mt_widget.hpp"
#include "mt_pixelgrid.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Bitmap : public Widget, public Pixelgrid
	{
	private:
		SDL_Texture *tex = nullptr;

		bool render = false;
		bool pressed = false;

		Bitmap(Window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h);
		Bitmap(Window &window, int x, int y, size_t w, size_t h);
		Bitmap(const Bitmap &) = delete;

		void init() override;

	public:
		static Bitmap &create(Window &window, int x, int y, size_t w, size_t h, size_t map_w, size_t map_h);
		static Bitmap &create(Window &window, int x, int y, size_t w, size_t h);

		~Bitmap();

		RGBA &at(size_t x, size_t y) override;

		void alloc(size_t width, size_t height) override;

		void erase() override;

		void handleEvent() override;
		void update() override;
		void draw() override;
	};
}
#endif /* F485EFCF_8079_4F33_91F7_A134EA3822B3 */
