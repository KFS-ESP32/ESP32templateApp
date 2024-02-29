// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2024 Karlfried S. and others
 */
#include "WebApi_ws_console.h"
#include "Configuration.h"
#include "MessageOutput.h"
#include "WebApi.h"
#include "defaults.h"

WebApiWsConsoleClass::WebApiWsConsoleClass()
    : _ws("/console")
    , _wsCleanupTask(1 * TASK_SECOND, TASK_FOREVER, std::bind(&WebApiWsConsoleClass::wsCleanupTaskCb, this))
{
}

void WebApiWsConsoleClass::init(AsyncWebServer& server, Scheduler& scheduler)
{
    server.addHandler(&_ws);
    MessageOutput.register_ws_output(&_ws);

    scheduler.addTask(_wsCleanupTask);
    _wsCleanupTask.enable();
}

void WebApiWsConsoleClass::wsCleanupTaskCb()
{
    // see: https://github.com/me-no-dev/ESPAsyncWebServer#limiting-the-number-of-web-socket-clients
    _ws.cleanupClients();

    if (Configuration.get().Security.AllowReadonly == true)
    {
        _ws.setAuthentication("", "");
    } else
    {
        _ws.setAuthentication(AUTH_USERNAME, Configuration.get().Security.Password);
    }
}
