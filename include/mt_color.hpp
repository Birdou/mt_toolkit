#ifndef FA5B1E82_3D8A_473B_A317_D74C285D595B
#define FA5B1E82_3D8A_473B_A317_D74C285D595B

#include "mt_application.hpp"

class Mt_color
{
private:
	int current_frame = 0;
	int frames = 0;

	SDL_Color *target_color = nullptr;

	float cr, cg, cb, ca;
	float sr, sg, sb, sa;

public:
	SDL_Color color;
	Mt_color()
	{
	}
	~Mt_color()
	{
	}

	void fadeInto(SDL_Color *target_color, int frames = 8)
	{
		if (color.r == target_color->r &&
			color.g == target_color->g &&
			color.b == target_color->b &&
			color.a == target_color->a)
		{
			return;
		}

		this->frames = frames;
		this->target_color = target_color;

		cr = color.r;
		sr = (target_color->r - color.r) / (float)frames;
		cg = color.g;
		sg = (target_color->g - color.g) / (float)frames;
		cb = color.b;
		sb = (target_color->b - color.b) / (float)frames;
		ca = color.a;
		sa = (target_color->a - color.a) / (float)frames;
	}

	void update()
	{
		if (target_color == nullptr)
			return;

		cr += sr;
		color.r = static_cast<Uint8>(cr);
		cg += sg;
		color.g = static_cast<Uint8>(cg);
		cb += sb;
		color.b = static_cast<Uint8>(cb);
		ca += sa;
		color.a = static_cast<Uint8>(ca);

		current_frame++;
		if (current_frame > frames)
		{
			color.r = target_color->r;
			color.g = target_color->g;
			color.b = target_color->b;
			color.a = target_color->a;
			target_color = nullptr;
			current_frame = 0;
			return;
		}
	}
};

#endif /* FA5B1E82_3D8A_473B_A317_D74C285D595B */
