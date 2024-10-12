/*
 * Copyright (C) 08/10/2024 Karlfried S.
 */
#include "Configuration.h"
#include "MessageOutput.h"
#include "MqttSettings.h"
#include "NetworkSettings.h"
#include "NtpSettings.h"
#include "RestartHelper.h"
#include "Scheduler.h"
#include "SunPosition.h"
#include "Utils.h"
#include "WebApi.h"
#include "ModuleTemplate.h"
#include "LedBlinker.h"
#include "defaults.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <TaskScheduler.h>

// Life counter
uint _lifeCounter;

void setup()
{
    // Move all dynamic allocations >512byte to psram (if available)
    heap_caps_malloc_extmem_enable(512);

    // Initialize serial output
    Serial.begin(SERIAL_BAUDRATE);
#if ARDUINO_USB_CDC_ON_BOOT
    Serial.setTxTimeoutMs(0);
    delay(100);
#else
    while (!Serial)
        yield();
#endif

    // LED-Blinker
    LedBlinker.StartupBlinkIntervall();

	// Message-Output
    MessageOutput.init(scheduler);
    MessageOutput.println();
    MessageOutput.println("Starting ESPtemplateApp");

    // Initialize file system
    MessageOutput.print("Initialize FS... ");
    if (!LittleFS.begin(false)) { // Do not format if mount failed
        MessageOutput.print("failed... trying to format...");
        if (!LittleFS.begin(true)) {
            MessageOutput.print("success");
        } else {
            MessageOutput.print("failed");
        }
    } else {
        MessageOutput.println("done");
    }

    // Read configuration values
    MessageOutput.print("Reading configuration... ");
    if (!Configuration.read()) {
        MessageOutput.print("initializing... ");
        Configuration.init();
        if (Configuration.write()) {
            MessageOutput.print("written... ");
        } else {
            MessageOutput.print("failed... ");
        }
    }
    if (Configuration.get().Cfg.Version != CONFIG_VERSION) {
        MessageOutput.print("migrated... ");
        Configuration.migrate();
    }
    if (Configuration.get().Cfg.AppID != CONFIG_APP_ID || Configuration.get().Cfg.AppID == 0)
    {
        // set default values
        MessageOutput.print("set new App ID... ");
        Configuration.init();
        Configuration.get().Cfg.AppID = CONFIG_APP_ID;
        Configuration.write();
        Configuration.read(); // --> set default values!
    }
    MessageOutput.println("done");

    // Initialize WiFi
    MessageOutput.print("Initialize Network... ");
    NetworkSettings.init(scheduler);
    MessageOutput.println("done");
    NetworkSettings.applyConfig();

    // Initialize NTP
    MessageOutput.print("Initialize NTP... ");
    NtpSettings.init();
    MessageOutput.println("done");

    // Initialize SunPosition
    MessageOutput.print("Initialize SunPosition... ");
    SunPosition.init(scheduler);
    MessageOutput.println("done");

    // Initialize MqTT
    MessageOutput.print("Initialize MqTT... ");
    MqttSettings.init();
    MessageOutput.println("done");

    // Initialize WebApi
    MessageOutput.print("Initialize WebApi... ");
    WebApi.init(scheduler);
    MessageOutput.println("done");

    // Initialize Restart-Helper
    MessageOutput.print("Initialize Restart-Helper... ");
    RestartHelper.init(scheduler);
    MessageOutput.println("done");

    // Initialize ModuleTemplate--> Rename it to your new function!
    MessageOutput.print("Initialize your new function???????????????... ");
    ModuleTemplate.init(scheduler);
    MessageOutput.println("done");



    // LED Blinker
    LedBlinker.init(scheduler);
}

void loop()
{
    scheduler.execute();

    // LifeCouter
    _lifeCounter++;
}
