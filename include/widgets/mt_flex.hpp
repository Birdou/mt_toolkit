#ifndef A2E76C48_844C_4A33_A3DF_D8A72ED640EA
#define A2E76C48_844C_4A33_A3DF_D8A72ED640EA

#include "mt_container.hpp"

namespace TOOLKIT_NAMESPACE
{
    class Flex : public Container
    {
    private:
        WIDGET_CLASS("FLEX");

        class Row : public Widget
        {
        private:
            WIDGET_CLASS("ROW");

            class Cel : public Widget
            {
            private:
                WIDGET_CLASS("CEL");

                Row &row;

                void init();

            public:
                Widget *widget = nullptr;
                size_t colspan;
                centers::centers alignment = centers::middle_left;
                int padding = 4;
                Cel(Row &row, Widget *widget, size_t colspan);

                ~Cel();
                void handleEvent() override;
                void update() override;
                void draw() override;
            };

            Flex &grid;
            void init();

            int lastMaxH = 0;
            int lastTotalSpan = 0;

            std::vector<Cel *> cols;

            friend Flex;

        public:
            Row(Flex &grid);
            ~Row();
            int padding = 0;

            void add(Widget &widget, size_t colspan = 1);
            Cel &at(size_t id);
            Cel &operator[](size_t id);

            void handleEvent() override;
            void update() override;
            void draw() override;
        };

        Flex(Window &window, int x, int y, int w, int h);
        Flex(const Flex &) = delete;

        bool gupdate = false;
        int lastVirtualX = 0;
        int lastVirtualY = 0;

    public:
        static Flex &create(Window &window, int x, int y, int w, int h);
        ~Flex();

        std::vector<Row *> rows;

        int padding = 2;

        Row &createRow();
        void createRows(size_t count);

        void grid(std::initializer_list<std::initializer_list<std::pair<Widget &, size_t>>> scheme);

        void updateGrid();

        Row &at(size_t index);
        Row &operator[](size_t index);

        void handleEvent() override;
        void update() override;
        void draw() override;
    };
}

#endif /* A2E76C48_844C_4A33_A3DF_D8A72ED640EA */
