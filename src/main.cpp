/*
Pay 'n' Spray color picker for GTA San Andreas

MIT License

Copyright (c) 2016 Fabio3rs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "ColorPanel.hpp"
#include "PayNSprayAction.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <windows.h>

// https://github.com/thelink2012/injector

namespace {

PayNSprayAction act;
ColorPanel myPanel;

float posX = 29.0F;
float posY = 145.0F;
float width = 40.0F;
char columns = 8;

bool panelbackground = true;

void loadConfigFromIni() {
    // Get the path of the running executable
    char exePath[MAX_PATH] = {};
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);

    // Remove the executable name to get the directory
    char *lastSlash = std::strrchr(exePath, '\\');
    if (lastSlash) {
        *(lastSlash + 1) = '\0';
    }

    // Build the ini file path relative to the exe directory
    std::string iniFile = std::string(exePath) + "PayNSprayColorPicker.ini";

    // If the file doesn't exist, create it with default values
    if (GetFileAttributesA(iniFile.c_str()) == INVALID_FILE_ATTRIBUTES) {
        WritePrivateProfileStringA(
            "PayNSpray", "posX", "29.0", iniFile.c_str());
        WritePrivateProfileStringA(
            "PayNSpray", "posY", "145.0", iniFile.c_str());
        WritePrivateProfileStringA(
            "PayNSpray", "width", "40.0", iniFile.c_str());
        WritePrivateProfileStringA(
            "PayNSpray", "columns", "8", iniFile.c_str());
        WritePrivateProfileStringA(
            "PayNSpray", "panelbackground", "1", iniFile.c_str());
    }

    std::array<char, 64> buf{};

    // posX
    if (GetPrivateProfileStringA("PayNSpray", "posX", "", buf.data(),
                                 static_cast<DWORD>(buf.size()),
                                 iniFile.c_str()) > 0) {
        posX = std::strtof(buf.data(), nullptr);
        if (std::isnan(posX) || std::isinf(posX)) {
            posX = 29.0F;
        }
    }

    // posY
    if (GetPrivateProfileStringA("PayNSpray", "posY", "", buf.data(),
                                 static_cast<DWORD>(buf.size()),
                                 iniFile.c_str()) > 0) {
        posY = std::strtof(buf.data(), nullptr);
        if (std::isnan(posY) || std::isinf(posY)) {
            posY = 145.0F;
        }
    }

    // width
    if (GetPrivateProfileStringA("PayNSpray", "width", "", buf.data(),
                                 static_cast<DWORD>(buf.size()),
                                 iniFile.c_str()) > 0) {
        width = std::strtof(buf.data(), nullptr);
        if (std::isnan(width) || std::isinf(width)) {
            width = 40.0F;
        }
    }

    // columns (integer)
    {
        UINT columnsValue =
            GetPrivateProfileIntA("PayNSpray", "columns",
                                  static_cast<UINT>(columns), iniFile.c_str());
        columns = static_cast<char>(columnsValue);

        if (columns < 1) {
            columns = 1;
        } else if (columns > 16) {
            columns = 16;
        }
    }

    {
        UINT panelbackgroundValue = GetPrivateProfileIntA(
            "PayNSpray", "panelbackground", static_cast<UINT>(panelbackground),
            iniFile.c_str());
        panelbackground = (panelbackgroundValue != 0);
    }
}

void inject() {
    injector::MakeJMP(0x44AEC0 + 5, 0x44B3FF);

    injector::MakeNOP(0x44AE39, 13);
    injector::MakeNOP(0x44AE4A, 5);
    loadConfigFromIni();

    injector::MakeInline<0x44AEC0>([](injector::reg_pack &regs) {
        if (!myPanel.exists()) {
            /**
             * @note make sure the configuration is up to date before creating
             * the panel in case the user modified the ini file while playing
             */
            loadConfigFromIni();
            myPanel.create(1, "PayNSpray", posX, posY, width, columns, true,
                           panelbackground, 1);
        }

        act.playersVeh = getPlayerVehicle(-1, 0);
        act.garageId = regs.esi;
        act.selectedColor = myPanel.getCarColourFromGrid();

        PayNSprayAction::showSelectColorMsg(act.selColorState);

        using SELECTION_STATE = PayNSprayAction::SELECTION_STATE;

        switch (act.runSelection()) {
        case SELECTION_STATE::NONE:
            break;
        case SELECTION_STATE::START:
            act.runPayNSprayLogic();
            myPanel.remove();
            break;
        case SELECTION_STATE::SELECTED:
            myPanel.remove();
            break;
        case SELECTION_STATE::EXIT:
            PayNSprayAction::openGarageDoor(act.garageId);
            myPanel.remove();
            break;
        }
    });
}

} // namespace

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason,
                    _In_ LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        inject();
    }

    return true;
}
