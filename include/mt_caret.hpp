#ifndef AE945488_D359_47A3_81D7_5EC2E140A8A2
#define AE945488_D359_47A3_81D7_5EC2E140A8A2

#include "mt_application.hpp"
#include "mt_widget.hpp"

class Mt_caret : public Mt_widget
{
private:
	SDL_Texture *texture = nullptr;

	const unsigned short blinkInterval = 500;
	Uint32 lastBlink = 0;

	bool show = true;

	Mt_caret(Mt_widget &widget);
	Mt_caret(const Mt_caret &) = delete;

	TTF_Font *renderedFont = nullptr;
	Mt_color renderedColor;

	void init() override;

	void setColorMod();

public:
	static Mt_caret &create(Mt_widget &widget);

	~Mt_caret();

	void update() override;
	void draw() override;
};

#endif /* AE945488_D359_47A3_81D7_5EC2E140A8A2 */
