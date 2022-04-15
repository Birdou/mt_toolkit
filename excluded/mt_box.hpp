#ifndef C54EBFCA_B58A_434B_92AC_0265BF4F4144
#define C54EBFCA_B58A_434B_92AC_0265BF4F4144

#include <vector>

#include "mt_application.hpp"
#include "mt_window.hpp"

class Mt_box : public Mt_widget
{
private:

    //std::vector<Mt_widget*> widgets;
    Mt_box(Mt_window&, int x, int y, int w, int h);
    Mt_box(const Mt_box&) = delete;
public:
    static Mt_box& create(Mt_window&, int x, int y, int w, int h);

};

#endif /* C54EBFCA_B58A_434B_92AC_0265BF4F4144 */
