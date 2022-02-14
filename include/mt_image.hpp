#ifndef D825BBB4_EF46_4EB7_80F0_778EFCED5CD2
#define D825BBB4_EF46_4EB7_80F0_778EFCED5CD2

#include "mt_application.hpp"
#include "mt_widget.hpp"

class Mt_image : public Mt_widget
{
private:
	SDL_Texture *texture = nullptr;

public:
	Mt_image(Mt_window &window, int x, int y, const std::string &filename);

	void loadImage(const std::string &filename);
};

#endif /* D825BBB4_EF46_4EB7_80F0_778EFCED5CD2 */
