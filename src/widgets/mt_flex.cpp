
#include "widgets/mt_flex.hpp"

#include "mt_point.hpp"

TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Flex::counter;
TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Flex::Row::counter;
TOOLKIT_NAMESPACE::Widget::widgetCounter TOOLKIT_NAMESPACE::Flex::Row::Cel::counter;

TOOLKIT_NAMESPACE::Flex::Flex(Window &window, int x, int y, int w, int h) : Container(window, getClassId(), x, y, w, h)
{
}

TOOLKIT_NAMESPACE::Flex &TOOLKIT_NAMESPACE::Flex::create(TOOLKIT_NAMESPACE::Window &window, int x, int y, int w, int h)
{
    Flex *flex = new Flex(window, x, y, w, h);
    // window.widgets.emplace_back(flex);
    window.add(*flex);
    return *flex;
}
TOOLKIT_NAMESPACE::Flex::~Flex()
{
    Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

    for (auto row : rows)
        delete row;
}
TOOLKIT_NAMESPACE::Flex::Row &TOOLKIT_NAMESPACE::Flex::createRow()
{
    Row *row = new Row(*this);
    // row->parent = this;
    rows.emplace_back(row);
    return *row;
}

void TOOLKIT_NAMESPACE::Flex::createRows(size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        Row *row = new Row(*this);
        rows.emplace_back(row);
    }
}
void TOOLKIT_NAMESPACE::Flex::grid(std::initializer_list<std::initializer_list<std::pair<Widget &, size_t>>> scheme)
{
    for (auto &_row : scheme)
    {
        Row &row = createRow();
        for (auto &_col : _row)
        {
            row.add(_col.first, _col.second);
        }
    }
}

TOOLKIT_NAMESPACE::Flex::Row &TOOLKIT_NAMESPACE::Flex::at(size_t index)
{
    if (index >= rows.size())
    {
        Error("Linha inexistente '" << index << "'");
        throw std::out_of_range("A classe não conseguiu encontrar a linha especificada.");
    }
    return *rows[index];
}
TOOLKIT_NAMESPACE::Flex::Row &TOOLKIT_NAMESPACE::Flex::operator[](size_t index)
{
    return at(index);
}
void TOOLKIT_NAMESPACE::Flex::handleEvent()
{
    HANDLE_WINDOW_EVENTS;

    return_if(!visible);

    for (auto &row : rows)
        row->handleEvent();
}

void TOOLKIT_NAMESPACE::Flex::updateGrid()
{
    const int griddifX = geometry->destR.x - geometry->srcR.x;
    const int griddifY = geometry->destR.y - geometry->srcR.y;
    const int gridTPadding = 2 * padding;

    int lastRowEdgePos = griddifY + padding;
    anyhover = false;
    for (auto &row : rows)
    {
        int maxH = 0;
        int maxColPadding = 0;
        int totalSpan = 0;
        for (auto col : row->cols)
        {
            maxH = std::max(maxH, col->widget->geometry->getH() + (2 * col->padding));
            maxColPadding = std::max(maxColPadding, col->padding);
            totalSpan += col->colspan;
        }

        const int rowTPadding = 2 * row->padding;

        row->geometry->destR.x = griddifX + padding;
        row->geometry->destR.y = lastRowEdgePos;
        row->geometry->setW(geometry->getW() - gridTPadding);
        row->geometry->setH(maxH + rowTPadding);

        lastRowEdgePos += row->geometry->getH();

        geometry->confineObject(row);
        if (!row->visible)
            continue;

        const int rowdifX = row->geometry->destR.x - row->geometry->srcR.x;
        const int rowdifY = row->geometry->destR.y - row->geometry->srcR.y;

        const double space = (1 / (double)totalSpan) * (row->geometry->getW() - rowTPadding);

        int currentSpan = 0;
        const int currentRowY = rowdifY + row->padding;
        for (auto &col : row->cols)
        {
            col->geometry->destR.x = rowdifX + (int)(currentSpan * space) + row->padding;
            col->geometry->destR.y = currentRowY;
            col->geometry->setW((int)(space * col->colspan));
            col->geometry->setH(maxH);

            row->geometry->confineObject(col);
            const int coldifX = col->geometry->destR.x - col->geometry->srcR.x;
            const int coldifY = col->geometry->destR.y - col->geometry->srcR.y;

            switch (col->alignment)
            {
            case centers::none:
                break;
            case centers::top_left:
                col->widget->geometry->destR.x = coldifX + col->padding;
                col->widget->geometry->destR.y = coldifY + col->padding;
                break;
            case centers::top_center:
                col->widget->geometry->destR.x = coldifX + ((col->geometry->destR.w / 2.) - (col->widget->geometry->getW() / 2.));
                col->widget->geometry->destR.y = coldifY + col->padding;
                break;
            case centers::top_right:
                col->widget->geometry->destR.x = coldifX + (col->geometry->destR.w - col->widget->geometry->destR.w) - col->padding;
                col->widget->geometry->destR.y = coldifY + col->padding;
                break;
            case centers::middle_left:
                col->widget->geometry->destR.x = coldifX + col->padding;
                col->widget->geometry->destR.y = coldifY + ((col->geometry->destR.h / 2.) - (col->widget->geometry->getH() / 2.));
                break;
            case centers::middle_center:
                col->widget->geometry->destR.x = coldifX + ((col->geometry->destR.w / 2.) - (col->widget->geometry->getW() / 2.));
                col->widget->geometry->destR.y = coldifY + ((col->geometry->destR.h / 2.) - (col->widget->geometry->getH() / 2.));
                break;
            case centers::middle_right:
                col->widget->geometry->destR.x = coldifX + (col->geometry->destR.w - col->widget->geometry->destR.w) - col->padding;
                col->widget->geometry->destR.y = coldifY + ((geometry->destR.h / 2.) - (col->widget->geometry->getH() / 2.));
                break;
            case centers::bottom_left:
                col->widget->geometry->destR.x = coldifX + col->padding;
                col->widget->geometry->destR.y = coldifY + (col->geometry->destR.h - col->widget->geometry->destR.h) - col->padding;
                break;
            case centers::bottom_center:
                col->widget->geometry->destR.x = coldifX + ((col->geometry->destR.w / 2.) - (col->widget->geometry->getW() / 2.));
                col->widget->geometry->destR.y = coldifY + (col->geometry->destR.h - col->widget->geometry->destR.h) - col->padding;
                break;
            case centers::bottom_right:
                col->widget->geometry->destR.x = coldifX + (col->geometry->destR.w - col->widget->geometry->destR.w) - col->padding;
                col->widget->geometry->destR.y = coldifY + (geometry->destR.h - col->widget->geometry->destR.h) - col->padding;
                break;
            }

            col->geometry->confineObject(col->widget);
            col->widget->update();
            anyhover += col->widget->isHoverScrollable();

            currentSpan += col->colspan;

            col->update();
        }
        row->update();
    }
    hoverScroll += anyhover;
    geometry->setH(lastRowEdgePos - griddifY + padding);

    lastVirtualX = griddifX;
    lastVirtualY = griddifY;
    gupdate = false;
}

void TOOLKIT_NAMESPACE::Flex::update()
{
    return_if(!visible);

    for (auto it = rows.begin(); it != rows.end();)
    {
        if (!(*it)->isActive())
        {
            delete (*it);
            it = rows.erase(it);
        }
        else
        {
            it++;
        }
    }

    updateGrid();
    // const int griddifX = geometry->destR.x - geometry->srcR.x;
    // const int griddifY = geometry->destR.y - geometry->srcR.y;

    // if (griddifX != lastVirtualX || griddifY != lastVirtualY || gupdate)
    // {
    // 	updateGrid();
    // }
    // else
    // {
    // 	for (auto& row : rows)
    // 	{
    // 		for (auto col : row->cols)
    // 		{
    // 			col->widget->update();
    // 		}
    // 	}
    // }

    if (Point::mousePos().intercept(geometry->destR))
    {
        fadeToHover();
    }
    else
    {
        fadeToNormal();
    }
}
void TOOLKIT_NAMESPACE::Flex::draw()
{
    return_if(!visible);

    window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
    for (auto &row : rows)
        row->draw();
    window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}
void TOOLKIT_NAMESPACE::Flex::Row::init()
{
    geometry->setAnchor(centers::none);
    currentBackgroundColor.hex(hexColors::White).a = 0;
    currentBorderColor.hex(hexColors::White).a = 0;
}
TOOLKIT_NAMESPACE::Flex::Row::Row(TOOLKIT_NAMESPACE::Flex &grid) : Widget(grid, getClassId()), grid(grid)
{
    init();
}
TOOLKIT_NAMESPACE::Flex::Row::~Row()
{
    Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");

    for (auto col : cols)
        delete col;
}
void TOOLKIT_NAMESPACE::Flex::Row::add(Widget &widget, size_t colspan)
{
    grid.add(widget);
    Cel *col = new Cel(*this, &widget, colspan);
    cols.emplace_back(col);
    widget.update();
    grid.gupdate = true;
}
TOOLKIT_NAMESPACE::Flex::Row::Cel &TOOLKIT_NAMESPACE::Flex::Row::at(size_t id)
{
    if (id >= cols.size())
    {
        Error("Coluna inexistente '" << id << "'");
        throw std::out_of_range("A classe não conseguiu encontrar a coluna especificada.");
    }
    return *cols[id];
}
TOOLKIT_NAMESPACE::Flex::Row::Cel &TOOLKIT_NAMESPACE::Flex::Row::operator[](size_t id)
{
    return at(id);
}

void TOOLKIT_NAMESPACE::Flex::Row::handleEvent()
{
    return_if(cols.empty() || !visible);

    for (auto col : cols)
        col->handleEvent();
}
void TOOLKIT_NAMESPACE::Flex::Row::update()
{
    return_if(!visible);
    if (Point::mousePos().intercept(geometry->destR))
    {
        fadeToHover();
    }
    else
    {
        fadeToNormal();
    }
}
void TOOLKIT_NAMESPACE::Flex::Row::draw()
{
    return_if(cols.empty() || !visible);

    window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
    for (auto col : cols)
        col->draw();
    window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}

// ANCHOR FLEX::ROW::CEL CLASS
void TOOLKIT_NAMESPACE::Flex::Row::Cel::init()
{
    // geometry->setAnchor(centers::none);
    currentBackgroundColor.hex(hexColors::White).a = 0;
    currentBorderColor.hex(hexColors::White).a = 0;
}
TOOLKIT_NAMESPACE::Flex::Row::Cel::Cel(Row &row, Widget *widget, size_t colspan) : Widget(row, getClassId()), row(row), widget(widget), colspan(colspan)
{
    init();
}
TOOLKIT_NAMESPACE::Flex::Row::Cel::~Cel()
{
    Debug("Destroying " << this->id << " (" << ++counter.destroyedWidgetCount << "/" << counter.widgetCount << ")");
}
void TOOLKIT_NAMESPACE::Flex::Row::Cel::handleEvent()
{
    return_if(!visible);
    widget->handleEvent();
}
void TOOLKIT_NAMESPACE::Flex::Row::Cel::update()
{
    return_if(!visible);
    if (Point::mousePos().intercept(geometry->destR))
    {
        fadeToHover();
    }
    else
    {
        fadeToNormal();
    }
}
void TOOLKIT_NAMESPACE::Flex::Row::Cel::draw()
{
    return_if(!visible);
    window.renderer->drawFillRectangle(geometry->destR, currentBackgroundColor);
    // if (widget != nullptr)
    widget->draw();
    window.renderer->drawRectangle(geometry->destR, currentBorderColor);
}
