#ifndef F568CC2C_1167_49BF_944E_CDCED331082E
#define F568CC2C_1167_49BF_944E_CDCED331082E

#include "mt_window.hpp"
#include "mt_widget.hpp"

namespace TOOLKIT_NAMESPACE
{
    class Container : public Widget, public Parent
    {
    protected:
        void confineObject(Widget *widget);
        bool anyhover;

        void init();

    public:
        Container(Window &window, int x, int y, int w, int h);
        virtual ~Container();
    };
}

#endif /* F568CC2C_1167_49BF_944E_CDCED331082E */
