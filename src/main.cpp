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
#include <cstdint>
// https://github.com/thelink2012/injector
#include <injector/assembly.hpp>
#include <injector/calling.hpp>
#include <injector/injector.hpp>

auto getPlayerCWanted = injector::cstd<int(int)>::call<0x56E230>;
auto sub_5625A0 = injector::thiscall<int(int)>::call<0x5625A0>;
auto getPlayerVehicle = injector::cstd<uint32_t(int, char)>::call<0x56E0D0>;
auto CGarages__TriggerMessage = injector::cstd<int(
    const char *, int16_t, int16_t, int16_t)>::call<0x447B80>;
auto CMenuSystem__GetCarColourFromGrid =
    injector::cstd<uint8_t(uint8_t, uint8_t)>::call<0x5822B0>;
auto CScriptThread__getPlayerKeyState =
    injector::thiscall<uint16_t(void *, uint16_t, uint16_t)>::call<0x485B10>;
auto CAudioEngine__ReportFrontendAudioEvent =
    injector::thiscall<void(void *, int, float, float)>::call<0x506EA0>;
static auto textLowPriority = injector::cstd<void(
    const char *text, unsigned time, bool flag1, bool flag2)>::call<0x00580750>;
static auto textHighPriority = injector::cstd<void(
    const char *text, unsigned time, bool flag1, bool flag2)>::call<0x0069F0B0>;

int &playerMoney = *(int *)(0xB7CD98 + 0xB8);
bool &CGarages__RespraysAreFree = *(bool *)0x96C009;

unsigned int &currentTime = *(unsigned int *)0xB7CB84;

const char *panelWat[] = {"DUMMY", "DUMMY", "DUMMY", "DUMMY"};

static ColorPanel myPanel;

static void openGarageDoor(uint32_t garageAddr, int delay = 2000) {
    *(uint8_t *)(garageAddr + 77) = 3;
    *(uint32_t *)(garageAddr + 60) = currentTime + 2000;
}

void inject() {
    injector::MakeJMP(0x44AEC0 + 5, 0x44B3FF);

    injector::MakeNOP(0x44AE39, 13);
    injector::MakeNOP(0x44AE4A, 5);

    injector::MakeInline<0x44AEC0>([](injector::reg_pack &regs) {
        static int selColor = 0;
        static const char *msgs[] = {"Select 1st color / Selecione a 1a cor",
                                     "Select 2nd color / Selecione a 2a cor"};
        static int primaryColor = 0;
        static int secondaryColor = 0;
        static bool selected = false;
        static bool jumped = false;
        bool startThings = false;

        /* Repair player's vehicle */
        uint32_t playersVeh = getPlayerVehicle(-1, 0);

        if (playersVeh == 0) {
            openGarageDoor(regs.esi);

            myPanel.remove();

            selColor = 0;
            selected = false;
            jumped = false;

            return;
        }

        textHighPriority(msgs[selColor], 100, false, false);

        if (!myPanel.exists()) {
            myPanel.create(1, panelWat, 29.0, 145.0, 40.0, 8, 1, 1, 1);
        }

        int selectedColor = myPanel.getActiveRow();

        if (!selected && selectedColor >= 0 &&
            CScriptThread__getPlayerKeyState(0, 0, 16)) {
            if (selColor == 0) {
                primaryColor = CMenuSystem__GetCarColourFromGrid(
                    myPanel.myPanel, selectedColor);
                ++selColor;
                selected = true;

                myPanel.remove();
                return;
            }

            secondaryColor = CMenuSystem__GetCarColourFromGrid(myPanel.myPanel,
                                                               selectedColor);

            startThings = true;
        } else if (CScriptThread__getPlayerKeyState(0, 0, 16) == 0) {
            selected = false;
        }

        if (!jumped && CScriptThread__getPlayerKeyState(0, 0, 4)) {
            if (selColor == 0) {
                primaryColor = *(uint8_t *)(playersVeh + 0x434);
                ++selColor;
                jumped = true;

                myPanel.remove();
                return;
            }

            secondaryColor = *(uint8_t *)(playersVeh + 0x435);

            startThings = true;
        } else if (CScriptThread__getPlayerKeyState(0, 0, 4) == 0) {
            jumped = false;
        }

        if (startThings) {
            bool pol = *(DWORD *)(getPlayerCWanted(-1) + 44) != 0;

            if (pol) {
                sub_5625A0(getPlayerCWanted(-1));
            }

            *(float *)(playersVeh + 0x4C0) = 1000.0;

            // Primary color
            *(uint8_t *)(playersVeh + 0x434) = primaryColor;

            // Secondary color
            *(uint8_t *)(playersVeh + 0x435) = secondaryColor;

            // Dirt level
            *(uint8_t *)(playersVeh + 0x4B0) = 0;

            (*(void(__thiscall **)(int))(*(DWORD *)playersVeh + 200))(
                playersVeh);
            *(DWORD *)(playersVeh + 1216) = 1148846080;

            // Garage door
            openGarageDoor(regs.esi);

            if (*(int8_t *)(regs.esi + 0x4E) > 0) {
                if (CGarages__RespraysAreFree) {
                    CGarages__TriggerMessage((rand() & 1) ? "GA_15" : "GA_16",
                                             -1, 4000, -1);
                } else {
                    CGarages__TriggerMessage(pol ? "GA_2" : "GA_XX", -1, 4000,
                                             -1);
                    playerMoney -= 100;
                }
            } else {
                CGarages__TriggerMessage("GA_22", -1, 4000, -1);
            }

            myPanel.remove();

            CAudioEngine__ReportFrontendAudioEvent((void *)0xB6BC90, 16, 0.0,
                                                   1.0);
            selected = false;
            jumped = false;
            selColor = 0;
        }

        if (CScriptThread__getPlayerKeyState(0, 0, 15)) {
            // Garage door
            openGarageDoor(regs.esi);

            myPanel.remove();
            selected = false;
            jumped = false;
            selColor = 0;
        }
    });
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason,
                    _In_ LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH)
        inject();

    return true;
}