#pragma once
#include "OptionSelect.hpp"
#include <array>
#include <cstdint>
#include <injector/assembly.hpp>
#include <injector/calling.hpp>
#include <injector/injector.hpp>

namespace {
auto getPlayerCWanted = injector::cstd<int(int)>::call<0x56E230>;
auto sub_5625A0 = injector::thiscall<int(int)>::call<0x5625A0>;
auto getPlayerVehicle = injector::cstd<uint32_t(int, char)>::call<0x56E0D0>;
auto CGarages__TriggerMessage = injector::cstd<int(
    const char *, int16_t, int16_t, int16_t)>::call<0x447B80>;
auto CAudioEngine__ReportFrontendAudioEvent =
    injector::thiscall<void(void *, int, float, float)>::call<0x506EA0>;
auto textLowPriority = injector::cstd<void(
    const char *text, unsigned time, bool flag1, bool flag2)>::call<0x00580750>;
auto textHighPriority = injector::cstd<void(
    const char *text, unsigned time, bool flag1, bool flag2)>::call<0x0069F0B0>;

constexpr unsigned short STAT_AUTO_REPAIR_AND_PAINTING_BUDGET = 0x10;

auto CStats__IncrementStat =
    injector::cstd<void(unsigned short, float)>::call<0x55C180>;
} // namespace

struct PayNSprayAction {
    std::array<uint8_t, 4> carColors{};
    std::array<uint8_t, 4> currentCarColors{};
    uint32_t playersVeh{};
    uint32_t garageId{};
    int selColorState = 0;
    int selectedColor{};

    enum SELECTION_STATE { NONE, EXIT, SELECTED, START };

    static void showSelectColorMsg(int selColor) {
        std::array msgs = {"Select 1st color / Selecione a 1a cor",
                           "Select 2nd color / Selecione a 2a cor",
                           "Select 3rd color / Selecione a 3a cor",
                           "Select 4th color / Selecione a 4a cor"};
        textHighPriority(msgs[selColor], 100, false, false);
    }

    auto runSelection() -> SELECTION_STATE {
        static OptionSelect::OPTIONS LastAction{OptionSelect::OPTIONS::NONE};

        if (playersVeh == 0) {
            selColorState = 0;
            return SELECTION_STATE::EXIT;
        }

        OptionSelect opts;
        const auto ACTION = opts();

        if (ACTION == LastAction) {
            return SELECTION_STATE::NONE;
        }

        LastAction = ACTION;

        if (ACTION == OptionSelect::CANCEL) {
            selColorState = 0;
            return SELECTION_STATE::EXIT;
        }

        const int MAX_CAR_COLORS = 4;

        if (selectedColor >= 0 && ACTION == OptionSelect::ACCEPT) {
            carColors[selColorState] = selectedColor;
            ++selColorState;

            if (selColorState != MAX_CAR_COLORS) {
                return SELECTION_STATE::SELECTED;
            }

            return SELECTION_STATE::START;
        }

        if (ACTION == OptionSelect::SKIP) {
            carColors[selColorState] =
                *(uint8_t *)(playersVeh + (0x434 + selColorState));
            ++selColorState;

            if (selColorState != MAX_CAR_COLORS) {
                return SELECTION_STATE::SELECTED;
            }

            return SELECTION_STATE::START;
        }

        return SELECTION_STATE::NONE;
    }

    static void openGarageDoor(uint32_t garageAddr, int delay = 2000) {
        const unsigned int &CURRENT_TIME = *(unsigned int *)0xB7CB84;
        *(uint8_t *)(garageAddr + 77) = 3;
        *(uint32_t *)(garageAddr + 60) = CURRENT_TIME + 2000;
    }

    void runPayNSprayLogic() {
        bool pol = *(DWORD *)(getPlayerCWanted(-1) + 44) != 0;

        if (pol) {
            sub_5625A0(getPlayerCWanted(-1));
        }

        *(float *)(playersVeh + 0x4C0) = 1000.0;

        // Primary color
        *(uint8_t *)(playersVeh + 0x434) = carColors[0];

        // Secondary color
        *(uint8_t *)(playersVeh + 0x435) = carColors[1];

        *(uint8_t *)(playersVeh + 0x436) = carColors[2];

        *(uint8_t *)(playersVeh + 0x437) = carColors[3];

        // Dirt level
        *(float *)(playersVeh + 0x4B0) = 0.0;

        // veh->vehicle.m_nFlags[6] &= 0x7F;
        // Clear bDisableParticles flag? (m_nFlags[6], offset 0x42E)
        // 00000428 m_nFlags        db 8
        *(uint8_t *)(playersVeh + 0x42E) &= 0x7F;

        (*(void(__thiscall **)(int))(*(DWORD *)playersVeh + 200))(playersVeh);
        *(DWORD *)(playersVeh + 1216) = 1148846080;

        // veh->vehicle.m_nFlags[7] |= 1;
        // Resprayed flag?
        // Set the "HasBeenResprayed" flag (see VehicleFlags::bHasBeenResprayed)
        *(uint8_t *)(playersVeh + 0x42F) |= 1;

        // Garage door
        openGarageDoor(garageId);

        int &playerMoney = *(int *)(0xB7CD98 + 0xB8);
        const bool &CGarages__RespraysAreFree = *(bool *)0x96C009;

        if (*(int8_t *)(garageId + 0x4E) > 0) {
            if (CGarages__RespraysAreFree) {
                CGarages__TriggerMessage((rand() & 1) != 0 ? "GA_15" : "GA_16",
                                         -1, 4000, -1);
            } else {
                CGarages__TriggerMessage(pol ? "GA_2" : "GA_XX", -1, 4000, -1);
                playerMoney -= 100;

                CStats__IncrementStat(STAT_AUTO_REPAIR_AND_PAINTING_BUDGET,
                                      100.0);
            }
        } else {
            CGarages__TriggerMessage("GA_22", -1, 4000, -1);
        }

        CAudioEngine__ReportFrontendAudioEvent((void *)0xB6BC90, 16, 0.0, 1.0);
        selColorState = 0;
    }
};
