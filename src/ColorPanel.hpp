#pragma once

#include <cassert>
#include <cstdint>
#include <injector/calling.hpp>

struct ColorPanel {
    int myPanel{-1};

    void remove() {
        if (!exists()) {
            return;
        }
        auto removePanel = injector::cstd<uint8_t(uint8_t)>::call<0x580750>;
        removePanel(myPanel);
        myPanel = -1;
    }

    [[nodiscard]] auto getActiveRow() const -> int {
        if (!exists()) {
            return -1;
        }

        auto getPanelActiveRow = injector::cstd<int(int)>::call<0x5807E0>;
        return getPanelActiveRow(myPanel);
    }

    auto create(unsigned char type, const char *title, float posX, float posY,
                float width, char columns, bool interactive, bool background,
                char alignment) -> int {
        if (strlen(title) > 9) {
            assert(false && "Title too long"); // NOLINT
            title = "TooLong";
        }
        remove();
        auto createPanel = injector::cstd<int(
            unsigned char, const char *, float, float, float, unsigned char,
            bool, bool, char)>::call<0x582300>;
        myPanel = createPanel(type, title, posX, posY, width, columns,
                              interactive, background, alignment);
        return myPanel;
    }

    [[nodiscard]] auto getCarColourFromGrid() const -> int {
        auto CMenuSystem__GetCarColourFromGrid =
            injector::cstd<uint8_t(uint8_t, uint8_t)>::call<0x5822B0>;

        const int SELECTED = getActiveRow();
        if (SELECTED < 0) {
            return -1;
        }

        return CMenuSystem__GetCarColourFromGrid(myPanel, SELECTED);
    }

    [[nodiscard]] auto exists() const -> bool { return myPanel != -1; }
};
