#ifndef EEFEF793_D242_424D_B8C2_45615BB3A0B4
#define EEFEF793_D242_424D_B8C2_45615BB3A0B4

#include "mt_container.hpp"
namespace TOOLKIT_NAMESPACE
{
    class Box : public Container
    {
    private:
        WIDGET_CLASS("BOX");

    protected:
        Box(Window &window, int x, int y, int w, int h);
        Box(const Box &) = delete;

    public:
        static Box &create(Window &window, int x, int y, int w, int h);
        ~Box();

        void handleEvent() override;
        void update() override;
        void draw() override;
    };
}
#endif /* EEFEF793_D242_424D_B8C2_45615BB3A0B4 */
