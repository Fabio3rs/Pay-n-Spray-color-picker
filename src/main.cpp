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
#include "PayNSprayAction.hpp"
#include <array>
#include <cstdint>
// https://github.com/thelink2012/injector

namespace {

PayNSprayAction act;

void inject() {
    injector::MakeJMP(0x44AEC0 + 5, 0x44B3FF);

    injector::MakeNOP(0x44AE39, 13);
    injector::MakeNOP(0x44AE4A, 5);

    injector::MakeInline<0x44AEC0>([](injector::reg_pack &regs) {
        act.playersVeh = getPlayerVehicle(-1, 0);
        act.garageId = regs.esi;
        if (act.runSelection() == PayNSprayAction::SELECTION_STATE::START) {
            act.runPayNSprayLogic();
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
