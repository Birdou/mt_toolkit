#ifndef A2E76C48_844C_4A33_A3DF_D8A72ED640EA
#define A2E76C48_844C_4A33_A3DF_D8A72ED640EA

#include "mt_container.hpp"

namespace TOOLKIT_NAMESPACE
{
    class Flex : public Container
    {
    private:
        class Row : public Widget
        {
        private:
            class Cel : public Widget
            {
            private:
                Row &row;

                void init();

            public:
                Widget *widget = nullptr;
                size_t colspan;
                centers::center alignment = centers::middle_left;
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

            void addWidget(Widget &widget, size_t colspan = 1);
            Cel &at(size_t id);
            Cel &operator[](size_t id);

            void handleEvent() override;
            void update() override;
            void draw() override;
        };

        Flex(Window &window, int x, int y, int w, int h);
        Flex(const Flex &) = delete;

        std::vector<Row *> rows;

        bool gupdate = false;
        int lastVirtualX = 0;
        int lastVirtualY = 0;

    public:
        static Flex &create(Window &window, int x, int y, int w, int h);
        ~Flex();
        int padding = 2;

        Row &createRow();

        void updateGrid();

        Row &at(size_t index);
        Row &operator[](size_t index);

        void handleEvent() override;
        void update() override;
        void draw() override;
    };
}

#endif /* A2E76C48_844C_4A33_A3DF_D8A72ED640EA */
