// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2024 Karlfried S. and others
 */
#include "WebApi_ws_live.h"
#include "Configuration.h"
#include "MessageOutput.h"
#include "Utils.h"
#include "WebApi.h"
#include "defaults.h"
#include <AsyncJson.h>

u_int32_t _telegramCounter = 0;

WebApiWsLiveClass::WebApiWsLiveClass()
    : _ws("/livedata")
    , _wsCleanupTask(1 * TASK_SECOND, TASK_FOREVER, std::bind(&WebApiWsLiveClass::wsCleanupTaskCb, this))
    , _sendDataTask(1 * TASK_SECOND, TASK_FOREVER, std::bind(&WebApiWsLiveClass::sendDataTaskCb, this))
{
}

void WebApiWsLiveClass::init(AsyncWebServer& server, Scheduler& scheduler)
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;
    using std::placeholders::_5;
    using std::placeholders::_6;

    server.on("/api/livedata/status", HTTP_GET, std::bind(&WebApiWsLiveClass::onLivedataStatus, this, _1));

    server.addHandler(&_ws);
    _ws.onEvent(std::bind(&WebApiWsLiveClass::onWebsocketEvent, this, _1, _2, _3, _4, _5, _6));

    scheduler.addTask(_wsCleanupTask);
    _wsCleanupTask.enable();

    scheduler.addTask(_sendDataTask);
    _sendDataTask.enable();
}

void WebApiWsLiveClass::wsCleanupTaskCb()
{
    // see: https://github.com/me-no-dev/ESPAsyncWebServer#limiting-the-number-of-web-socket-clients
    _ws.cleanupClients();

    if (Configuration.get().Security.AllowReadonly) {
        _ws.setAuthentication("", "");
    } else {
        _ws.setAuthentication(AUTH_USERNAME, Configuration.get().Security.Password);
    }
}

void WebApiWsLiveClass::sendDataTaskCb()
{
    // do nothing if no WS client is connected
    if (_ws.count() == 0) {
        return;
    }

    // Update on every 10 seconds
    if (millis() - _lastWsPublish > (10 * 1000)) {
        try {
            std::lock_guard<std::mutex> lock(_mutex);
            JsonDocument root;
            JsonVariant var = root;
            if (Utils::checkJsonAlloc(root, __FUNCTION__, __LINE__)) {

                generateJsonResponse(var);
                String buffer;
                serializeJson(root, buffer);

                _ws.textAll(buffer);
            }

        } catch (const std::bad_alloc& bad_alloc) {
            MessageOutput.printf("Call to /api/livedata/status temporarely out of resources. Reason: \"%s\".\r\n", bad_alloc.what());
        } catch (const std::exception& exc) {
            MessageOutput.printf("Unknown exception in /api/livedata/status. Reason: \"%s\".\r\n", exc.what());
        }

        _lastWsPublish = millis();
    }
}

void WebApiWsLiveClass::generateJsonResponse(JsonVariant& root)
{
    /* #######################################
       # TODO: Anpassen an aktuelle Istwerte #
       #######################################
    */

    // The client site is definied in 'LiveDataStatus.ts'
    // and is used in HomeView.vue.
    // -------------------------------------------------

    _telegramCounter++;

    JsonArray commonDataObj = root.createNestedArray("commonData");

    // Loop: Do all arrays
    for (uint8_t i = 0; i < 2; i++) {

        JsonObject stateObject = commonDataObj.createNestedObject();

        stateObject["name"] = "Hallo -- TODO --" + i;
        stateObject["runtime_sec"] = millis() / 1000;
        stateObject["limit_absolute"] = -1;
        stateObject["telegramCounter"] = _telegramCounter;
        // TODO: u.s.w.
    }

    // Create 'hints' Object
    JsonObject hintObj = root.createNestedObject("hints");
    struct tm timeinfo;
    hintObj["time_sync"] = !getLocalTime(&timeinfo, 5);
    if (!strcmp(Configuration.get().Security.Password, ACCESS_POINT_PASSWORD)) {
        hintObj["default_password"] = true;
    } else {
        hintObj["default_password"] = false;
    }

    // Create 'system' Object
    JsonObject systemObj = root.createNestedObject("system");
    systemObj["lifeCounter"] = _telegramCounter;

}

void WebApiWsLiveClass::onWebsocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len)
{
    if (type == WS_EVT_CONNECT) {
        MessageOutput.printf("Websocket: [%s][%u] connect\r\n", server->url(), client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        MessageOutput.printf("Websocket: [%s][%u] disconnect\r\n", server->url(), client->id());
    }
}

void WebApiWsLiveClass::onLivedataStatus(AsyncWebServerRequest* request)
{
    if (WebApi.checkCredentialsReadonly(request) == false)
    {
        return;
    }

    try {
        std::lock_guard<std::mutex> lock(_mutex);
        AsyncJsonResponse* response = new AsyncJsonResponse();
        auto& root = response->getRoot();

        generateJsonResponse(root);

        WebApi.sendJsonResponse(request, response, __FUNCTION__, __LINE__);

    } catch (const std::bad_alloc& bad_alloc) {
        MessageOutput.printf("Call to /api/livedata/status temporarely out of resources. Reason: \"%s\".\r\n", bad_alloc.what());
        WebApi.sendTooManyRequests(request);
    } catch (const std::exception& exc) {
        MessageOutput.printf("Unknown exception in /api/livedata/status. Reason: \"%s\".\r\n", exc.what());
        WebApi.sendTooManyRequests(request);
    }
}
