// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2024 Karlfried S. and others
 */
#include "WebApi_sysstatus.h"
#include "Configuration.h"
#include "NetworkSettings.h"
#include "WebApi.h"
#include "__compiled_constants.h"
#include <AsyncJson.h>
#include <CpuTemperature.h>
#include <LittleFS.h>
#include <ResetReason.h>


#ifndef AUTO_GIT_HASH
#define AUTO_GIT_HASH ""
#endif

void WebApiSysstatusClass::init(AsyncWebServer& server, Scheduler& scheduler)
{
    using std::placeholders::_1;

    server.on("/api/system/status", HTTP_GET, std::bind(&WebApiSysstatusClass::onSystemStatus, this, _1));
}

void WebApiSysstatusClass::onSystemStatus(AsyncWebServerRequest* request)
{
    if (!WebApi.checkCredentialsReadonly(request)) {
        return;
    }

    AsyncJsonResponse* response = new AsyncJsonResponse();
    auto& root = response->getRoot();

    root["hostname"] = NetworkSettings.getHostname();

    root["sdkversion"] = ESP.getSdkVersion();
    root["cpufreq"] = ESP.getCpuFreqMHz();
    root["cputemp"] = CpuTemperature.read();

    root["heap_total"] = ESP.getHeapSize();
    root["heap_used"] = ESP.getHeapSize() - ESP.getFreeHeap();
    root["heap_max_block"] = ESP.getMaxAllocHeap();
    root["heap_min_free"] = ESP.getMinFreeHeap();
    root["psram_total"] = ESP.getPsramSize();
    root["psram_used"] = ESP.getPsramSize() - ESP.getFreePsram();
    root["sketch_total"] = ESP.getFreeSketchSpace();
    root["sketch_used"] = ESP.getSketchSize();
    root["littlefs_total"] = LittleFS.totalBytes();
    root["littlefs_used"] = LittleFS.usedBytes();

    root["chiprevision"] = ESP.getChipRevision();
    root["chipmodel"] = ESP.getChipModel();
    root["chipcores"] = ESP.getChipCores();
    root["flashsize"] = ESP.getFlashChipSize();

    String reason;
    reason = ResetReason::get_reset_reason_verbose(0);
    root["resetreason_0"] = reason;

    reason = ResetReason::get_reset_reason_verbose(1);
    root["resetreason_1"] = reason;

    root["cfgsavecount"] = Configuration.get().Cfg.SaveCount;

    char version[16];
    snprintf(version, sizeof(version), "%d.%d.%d", CONFIG_VERSION >> 24 & 0xff, CONFIG_VERSION >> 16 & 0xff, CONFIG_VERSION >> 8 & 0xff);
    root["config_version"] = version;
    root["git_hash"] = AUTO_GIT_HASH;
    root["pioenv"] = PIOENV;

    root["uptime"] = esp_timer_get_time() / 1000000;


    WebApi.sendJsonResponse(request, response, __FUNCTION__, __LINE__);
}
