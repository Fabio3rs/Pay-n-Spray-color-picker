#pragma once

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

    void create(unsigned char a, const char **b, float c, float d, float e,
                unsigned char f, char g, char h, char i) {
        remove();
        auto createPanel = injector::cstd<int(
            unsigned char, const char **, float, float, float, unsigned char,
            char, char, char)>::call<0x582300>;
        myPanel = createPanel(a, b, c, d, e, f, g, h, i);
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
