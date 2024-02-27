// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <ArduinoJson.h>
#include <cstdint>

class Utils {
public:
    static uint32_t getChipId();
    static int getTimezoneOffset();
    static void restartAPP();
    static bool checkJsonAlloc(const DynamicJsonDocument& doc, const char* function, const uint16_t line);
};
