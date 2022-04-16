#ifndef B72EFF8C_D70C_44BC_BB4A_88698734A49A
#define B72EFF8C_D70C_44BC_BB4A_88698734A49A

#include "mt_application.hpp"

#include "mt_color.hpp"

namespace TOOLKIT_NAMESPACE
{
	class Font
	{
	private:
		Application &application;

		TTF_Font *font = nullptr;
		unsigned short fontSize = 0;
		int maxHeight = 0;

	public:
		Color color;

		Font(Application &application);
		Font(Application &application, const std::string &path, unsigned short fontSize);
		~Font();

		void setFont(const std::string &path, unsigned short size);

		TTF_Font *getFont();

		unsigned short getSize() const;

		int getH() const;
	};
}
#endif /* B72EFF8C_D70C_44BC_BB4A_88698734A49A */
