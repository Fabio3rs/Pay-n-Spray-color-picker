#pragma once

#include <injector/calling.hpp>

struct OptionSelect {
    enum OPTIONS { NONE, SKIP, ACCEPT, CANCEL };

    auto operator()() -> OPTIONS {
        auto CScriptThread__getPlayerKeyState = injector::thiscall<uint16_t(
            void *, uint16_t, uint16_t)>::call<0x485B10>;
        if (CScriptThread__getPlayerKeyState(nullptr, 0, 4) != 0U) {
            return SKIP;
        }
        if (CScriptThread__getPlayerKeyState(nullptr, 0, 16) != 0U) {
            return ACCEPT;
        }
        if (CScriptThread__getPlayerKeyState(nullptr, 0, 15) != 0U) {
            return CANCEL;
        }

        return NONE;
    }
};
