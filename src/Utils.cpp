// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2024 Karlfried S. and others
 */
#include "Utils.h"
#include "MessageOutput.h"
#include <Esp.h>
#include <LittleFS.h>

uint32_t Utils::getChipId()
{
    uint32_t chipId = 0;
    for (uint8_t i = 0; i < 17; i += 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    return chipId;
}

int Utils::getTimezoneOffset()
{
    // see: https://stackoverflow.com/questions/13804095/get-the-time-zone-gmt-offset-in-c/44063597#44063597

    time_t gmt, rawtime = time(NULL);
    struct tm* ptm;

    struct tm gbuf;
    ptm = gmtime_r(&rawtime, &gbuf);

    // Request that mktime() looksup dst in timezone database
    ptm->tm_isdst = -1;
    gmt = mktime(ptm);

    return static_cast<int>(difftime(rawtime, gmt));
}

bool Utils::checkJsonAlloc(const JsonDocument& doc, const char* function, const uint16_t line)
{
    if (doc.overflowed()) {
        MessageOutput.printf("Alloc failed: %s, %d\r\n", function, line);
        return false;
    }

    return true;
}

/// @brief  Remove all files from filesystem on factory reset
///         This allows to create more user defined files and get them deleted on factory reset.
void Utils::removeAllFiles()
{
    auto root = LittleFS.open("/");
    auto file = root.getNextFileName();

    while (file != "") {
        LittleFS.remove(file);
        file = root.getNextFileName();
    }
}

