/*
 * Copyright (C) 22/02/2024 Karlfried S.
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
    if (LittleFS.begin(false) == false) // Do not format if mount failed
    {
        MessageOutput.print("failed... trying to format...");
        if (LittleFS.begin(true) == true)
        {
            MessageOutput.print("success");
        }
        else
        {
            MessageOutput.print("failed");
        }
    }
    else
    {
        MessageOutput.println("done");
    }

    // Read configuration values
    // =========================
    MessageOutput.print("Reading configuration... ");
    Configuration.setDefaultConfig();   // 1) First we set the default config values
    bool isRead = Configuration.read(); // 2) We try to read the config values from LittleFS

    // Did something go wrong?
    if (isRead == false || Configuration.get().Cfg.AppID != CONFIG_APP_ID || Configuration.get().Cfg.AppID == 0)
    {
        MessageOutput.print("initializing... ");
        Configuration.setDefaultConfig(); // clear all & set default values!
        if (Configuration.write() == true)
        {
            MessageOutput.print("written... ");
        }
        else
        {
            MessageOutput.print("failed... ");
        }
    }
    if (Configuration.get().Cfg.Version != CONFIG_VERSION)
    {
        MessageOutput.print("migrated... ");
        Configuration.migrate();
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

    // LED Blinker
    LedBlinker.init(scheduler);
}

void loop()
{
    scheduler.execute();

    // LifeCouter
    _lifeCounter++;
}
