#ifndef FB734E9A_4C58_40C9_B7B9_3D14FEEC12E2
#define FB734E9A_4C58_40C9_B7B9_3D14FEEC12E2

#include "mt_core.hpp"
#include "mt_color.hpp"

#include <functional>
#include <unordered_map>

namespace TOOLKIT_NAMESPACE
{
	using DrawFunction = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color)>;
	using DrawFunctionWrapped = std::function<SDL_Surface *(TTF_Font *, const char *, SDL_Color, Uint32)>;

	class Font;
	class Geometry;
	class RGBA;

	class Renderer
	{
	private:
		SDL_Renderer *renderer = nullptr;
		RGBA renderColor;

		std::unordered_map<std::string, SDL_Surface *> surfaces;

	public:
		Renderer(SDL_Window *window);
		~Renderer();

		void clear();
		void setDrawColor(const RGBA &color);
		void present();
		SDL_Texture *createTextureFromSurface(SDL_Surface *surface);

		int substrWidth(TTF_Font *font, const std::string &string, size_t from = 0, size_t length = std::string::npos);

		void drawTexture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
		void drawRectangle(const SDL_Rect &dest, const RGBA &color);
		void drawFillRectangle(const SDL_Rect &dest, const RGBA &color);

		SDL_Texture *renderText(const std::string &text, TTF_Font *font, Geometry *geometry, DrawFunction TTF_RenderFunction);
		SDL_Texture *renderWrapped(const std::string &text, TTF_Font *font, Geometry *geometry, Uint32 wrapLenght, DrawFunctionWrapped TTF_RenderFunction);
	};
}
#endif /* FB734E9A_4C58_40C9_B7B9_3D14FEEC12E2 */
