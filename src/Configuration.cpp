// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2024 Karlfried S. and others
 */
#include "Configuration.h"
#include "MessageOutput.h"
#include "NetworkSettings.h"
#include "Utils.h"
#include "defaults.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <nvs_flash.h>

CONFIG_T config;

void ConfigurationClass::init()
{
    // löschen
    memset(&config, 0x0, sizeof(config));
}

bool ConfigurationClass::write()
{
    File f = LittleFS.open(CONFIG_FILENAME, "w", true); // create file!
    if (!f) {
        return false;
    }
    config.Cfg.SaveCount++;

    JsonDocument doc;

    JsonObject cfg = doc["cfg"].to<JsonObject>();
    cfg["app_id"] = config.Cfg.AppID;
    cfg["version"] = config.Cfg.Version;
    cfg["save_count"] = config.Cfg.SaveCount;

    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ssid"] = config.WiFi.Ssid;
    wifi["password"] = config.WiFi.Password;
    wifi["ip"] = IPAddress(config.WiFi.Ip).toString();
    wifi["netmask"] = IPAddress(config.WiFi.Netmask).toString();
    wifi["gateway"] = IPAddress(config.WiFi.Gateway).toString();
    wifi["dns1"] = IPAddress(config.WiFi.Dns1).toString();
    wifi["dns2"] = IPAddress(config.WiFi.Dns2).toString();
    wifi["dhcp"] = config.WiFi.Dhcp;
    wifi["hostname"] = config.WiFi.Hostname;
    wifi["aptimeout"] = config.WiFi.ApTimeout;

    JsonObject mdns = doc["mdns"].to<JsonObject>();
    mdns["enabled"] = config.Mdns.Enabled;

    JsonObject ntp = doc["ntp"].to<JsonObject>();
    ntp["server"] = config.Ntp.Server;
    ntp["timezone"] = config.Ntp.Timezone;
    ntp["timezone_descr"] = config.Ntp.TimezoneDescr;
    ntp["latitude"] = config.Ntp.Latitude;
    ntp["longitude"] = config.Ntp.Longitude;
    ntp["sunsettype"] = config.Ntp.SunsetType;

    JsonObject mqtt = doc["mqtt"].to<JsonObject>();
    mqtt["enabled"] = config.Mqtt.Enabled;
    mqtt["hostname"] = config.Mqtt.Hostname;
    mqtt["port"] = config.Mqtt.Port;
    mqtt["clientid"] = config.Mqtt.ClientId;
    mqtt["username"] = config.Mqtt.Username;
    mqtt["password"] = config.Mqtt.Password;
    mqtt["topic"] = config.Mqtt.Topic;
    mqtt["retain"] = config.Mqtt.Retain;
    mqtt["publish_interval"] = config.Mqtt.PublishInterval;
    mqtt["clean_session"] = config.Mqtt.CleanSession;

    JsonObject mqtt_lwt = mqtt["lwt"].to<JsonObject>();
    mqtt_lwt["topic"] = config.Mqtt.Lwt.Topic;
    mqtt_lwt["value_online"] = config.Mqtt.Lwt.Value_Online;
    mqtt_lwt["value_offline"] = config.Mqtt.Lwt.Value_Offline;
    mqtt_lwt["qos"] = config.Mqtt.Lwt.Qos;

    JsonObject mqtt_tls = mqtt["tls"].to<JsonObject>();
    mqtt_tls["enabled"] = config.Mqtt.Tls.Enabled;
    mqtt_tls["root_ca_cert"] = config.Mqtt.Tls.RootCaCert;
    mqtt_tls["certlogin"] = config.Mqtt.Tls.CertLogin;
    mqtt_tls["client_cert"] = config.Mqtt.Tls.ClientCert;
    mqtt_tls["client_key"] = config.Mqtt.Tls.ClientKey;

    JsonObject mqtt_hass = mqtt["hass"].to<JsonObject>();
    mqtt_hass["enabled"] = config.Mqtt.Hass.Enabled;
    mqtt_hass["retain"] = config.Mqtt.Hass.Retain;
    mqtt_hass["topic"] = config.Mqtt.Hass.Topic;
    mqtt_hass["expire"] = config.Mqtt.Hass.Expire;

    JsonObject security = doc["security"].to<JsonObject>();
    security["password"] = config.Security.Password;
    security["allow_readonly"] = config.Security.AllowReadonly;

    // Serialize JSON to file
    if (serializeJson(doc, f) == 0) {
        MessageOutput.println("Failed to write 'config' file");
        return false;
    }

    f.close();
    return true;
}

bool ConfigurationClass::read()
{
    File f = LittleFS.open(CONFIG_FILENAME, "r", false);

    JsonDocument doc;

    // Deserialize the JSON document
    const DeserializationError error = deserializeJson(doc, f);
    if (error)
    {
        MessageOutput.println("Failed to read file, using default configuration");
        return false; // --> set setDefaultConfig()
    }

   if (!Utils::checkJsonAlloc(doc, __FUNCTION__, __LINE__)) {
        return false;
    }

    JsonObject cfg = doc["cfg"];
    config.Cfg.AppID = cfg["app_id"];
    config.Cfg.Version = cfg["version"];
    config.Cfg.SaveCount = cfg["save_count"];

    JsonObject wifi = doc["wifi"];
    strlcpy(config.WiFi.Ssid, wifi["ssid"], sizeof(config.WiFi.Ssid));
    strlcpy(config.WiFi.Password, wifi["password"], sizeof(config.WiFi.Password));
    strlcpy(config.WiFi.Hostname, wifi["hostname"], sizeof(config.WiFi.Hostname));

    IPAddress wifi_ip;
    wifi_ip.fromString(wifi["ip"] | "");
    config.WiFi.Ip[0] = wifi_ip[0];
    config.WiFi.Ip[1] = wifi_ip[1];
    config.WiFi.Ip[2] = wifi_ip[2];
    config.WiFi.Ip[3] = wifi_ip[3];

    IPAddress wifi_netmask;
    wifi_netmask.fromString(wifi["netmask"] | "");
    config.WiFi.Netmask[0] = wifi_netmask[0];
    config.WiFi.Netmask[1] = wifi_netmask[1];
    config.WiFi.Netmask[2] = wifi_netmask[2];
    config.WiFi.Netmask[3] = wifi_netmask[3];

    IPAddress wifi_gateway;
    wifi_gateway.fromString(wifi["gateway"] | "");
    config.WiFi.Gateway[0] = wifi_gateway[0];
    config.WiFi.Gateway[1] = wifi_gateway[1];
    config.WiFi.Gateway[2] = wifi_gateway[2];
    config.WiFi.Gateway[3] = wifi_gateway[3];

    IPAddress wifi_dns1;
    wifi_dns1.fromString(wifi["dns1"] | "");
    config.WiFi.Dns1[0] = wifi_dns1[0];
    config.WiFi.Dns1[1] = wifi_dns1[1];
    config.WiFi.Dns1[2] = wifi_dns1[2];
    config.WiFi.Dns1[3] = wifi_dns1[3];

    IPAddress wifi_dns2;
    wifi_dns2.fromString(wifi["dns2"] | "");
    config.WiFi.Dns2[0] = wifi_dns2[0];
    config.WiFi.Dns2[1] = wifi_dns2[1];
    config.WiFi.Dns2[2] = wifi_dns2[2];
    config.WiFi.Dns2[3] = wifi_dns2[3];

    config.WiFi.Dhcp = wifi["dhcp"];
    config.WiFi.ApTimeout = wifi["aptimeout"];

    JsonObject mdns = doc["mdns"];
    config.Mdns.Enabled = mdns["enabled"];

    JsonObject ntp = doc["ntp"];
    strlcpy(config.Ntp.Server, ntp["server"], sizeof(config.Ntp.Server));
    strlcpy(config.Ntp.Timezone, ntp["timezone"], sizeof(config.Ntp.Timezone));
    strlcpy(config.Ntp.TimezoneDescr, ntp["timezone_descr"], sizeof(config.Ntp.TimezoneDescr));
    config.Ntp.Latitude = ntp["latitude"];
    config.Ntp.Longitude = ntp["longitude"];
    config.Ntp.SunsetType = ntp["sunsettype"];

    JsonObject mqtt = doc["mqtt"];
    config.Mqtt.Enabled = mqtt["enabled"];
    strlcpy(config.Mqtt.Hostname, mqtt["hostname"], sizeof(config.Mqtt.Hostname));
    config.Mqtt.Port = mqtt["port"] | MQTT_PORT;
    strlcpy(config.Mqtt.ClientId, mqtt["clientid"], sizeof(config.Mqtt.ClientId));
    strlcpy(config.Mqtt.Username, mqtt["username"], sizeof(config.Mqtt.Username));
    strlcpy(config.Mqtt.Password, mqtt["password"], sizeof(config.Mqtt.Password));
    strlcpy(config.Mqtt.Topic, mqtt["topic"], sizeof(config.Mqtt.Topic));
    config.Mqtt.Retain = mqtt["retain"];
    config.Mqtt.PublishInterval = mqtt["publish_interval"];
    config.Mqtt.CleanSession = mqtt["clean_session"];

    JsonObject mqtt_lwt = mqtt["lwt"];
    strlcpy(config.Mqtt.Lwt.Topic, mqtt_lwt["topic"], sizeof(config.Mqtt.Lwt.Topic));
    strlcpy(config.Mqtt.Lwt.Value_Online, mqtt_lwt["value_online"], sizeof(config.Mqtt.Lwt.Value_Online));
    strlcpy(config.Mqtt.Lwt.Value_Offline, mqtt_lwt["value_offline"], sizeof(config.Mqtt.Lwt.Value_Offline));
    config.Mqtt.Lwt.Qos = mqtt_lwt["qos"];

    JsonObject mqtt_tls = mqtt["tls"];
    config.Mqtt.Tls.Enabled = mqtt_tls["enabled"];
    strlcpy(config.Mqtt.Tls.RootCaCert, mqtt_tls["root_ca_cert"], sizeof(config.Mqtt.Tls.RootCaCert));
    config.Mqtt.Tls.CertLogin = mqtt_tls["certlogin"];
    strlcpy(config.Mqtt.Tls.ClientCert, mqtt_tls["client_cert"], sizeof(config.Mqtt.Tls.ClientCert));
    strlcpy(config.Mqtt.Tls.ClientKey, mqtt_tls["client_key"], sizeof(config.Mqtt.Tls.ClientKey));

    JsonObject mqtt_hass = mqtt["hass"];
    config.Mqtt.Hass.Enabled = mqtt_hass["enabled"];
    config.Mqtt.Hass.Retain = mqtt_hass["retain"];
    config.Mqtt.Hass.Expire = mqtt_hass["expire"];
    strlcpy(config.Mqtt.Hass.Topic, mqtt_hass["topic"], sizeof(config.Mqtt.Hass.Topic));

    JsonObject security = doc["security"];
    strlcpy(config.Security.Password, security["password"], sizeof(config.Security.Password));
    config.Security.AllowReadonly = security["allow_readonly"];

    f.close();
    return true;
}

bool ConfigurationClass::setDefaultConfig()
{
    init(); // Clear all

    config.Cfg.AppID = CONFIG_APP_ID;
    config.Cfg.Version = CONFIG_VERSION;
    config.Cfg.SaveCount = 0;

    strlcpy(config.WiFi.Ssid, WIFI_SSID, sizeof(config.WiFi.Ssid));
    strlcpy(config.WiFi.Password, WIFI_PASSWORD, sizeof(config.WiFi.Password));
    strlcpy(config.WiFi.Hostname, APP_HOSTNAME, sizeof(config.WiFi.Hostname));

    // config.WiFi.Ip --> no init needed, all values = 0;

    // config.WiFi.Netmask --> no init needed, all values = 0;

    // config.WiFi.Gateway --> no init needed, all values = 0;

    // config.WiFi.Dns1 --> no init needed, all values = 0;

    // config.WiFi.Dns2 --> no init needed, all values = 0;

    config.WiFi.Dhcp = WIFI_DHCP;
    config.WiFi.ApTimeout = ACCESS_POINT_TIMEOUT;

    config.Mdns.Enabled = MDNS_ENABLED;

    strlcpy(config.Ntp.Server, NTP_SERVER, sizeof(config.Ntp.Server));
    strlcpy(config.Ntp.Timezone, NTP_TIMEZONE, sizeof(config.Ntp.Timezone));
    strlcpy(config.Ntp.TimezoneDescr, NTP_TIMEZONEDESCR, sizeof(config.Ntp.TimezoneDescr));
    config.Ntp.Latitude = NTP_LATITUDE;
    config.Ntp.Longitude = NTP_LONGITUDE;
    config.Ntp.SunsetType = NTP_SUNSETTYPE;

    config.Mqtt.Enabled = MQTT_ENABLED;
    strlcpy(config.Mqtt.Hostname, MQTT_HOST, sizeof(config.Mqtt.Hostname));
    config.Mqtt.Port = MQTT_PORT;
    strlcpy(config.Mqtt.ClientId, NetworkSettings.getApName().c_str(), sizeof(config.Mqtt.ClientId));
    strlcpy(config.Mqtt.Username, MQTT_USER, sizeof(config.Mqtt.Username));
    strlcpy(config.Mqtt.Password, MQTT_PASSWORD, sizeof(config.Ntp.Server));
    strlcpy(config.Mqtt.Topic, MQTT_TOPIC, sizeof(config.Mqtt.Topic));
    config.Mqtt.Retain = MQTT_RETAIN;
    config.Mqtt.PublishInterval = MQTT_PUBLISH_INTERVAL;
    config.Mqtt.CleanSession = MQTT_CLEAN_SESSION;

    strlcpy(config.Mqtt.Lwt.Topic, MQTT_LWT_TOPIC, sizeof(config.Mqtt.Topic));
    strlcpy(config.Mqtt.Lwt.Value_Online, MQTT_LWT_ONLINE, sizeof(config.Mqtt.Lwt.Value_Online));
    strlcpy(config.Mqtt.Lwt.Value_Offline, MQTT_LWT_OFFLINE, sizeof(config.Mqtt.Lwt.Value_Offline));
    config.Mqtt.Lwt.Qos = MQTT_LWT_QOS;

    config.Mqtt.Tls.Enabled = MQTT_TLS;
    strlcpy(config.Mqtt.Tls.RootCaCert, MQTT_ROOT_CA_CERT, sizeof(config.Mqtt.Tls.RootCaCert));
    config.Mqtt.Tls.CertLogin = MQTT_TLSCERTLOGIN;
    strlcpy(config.Mqtt.Tls.ClientCert, MQTT_TLSCLIENTCERT, sizeof(config.Mqtt.Tls.ClientCert));
    strlcpy(config.Mqtt.Tls.ClientKey, MQTT_TLSCLIENTKEY, sizeof(config.Mqtt.Tls.ClientKey));

    config.Mqtt.Hass.Enabled = MQTT_HASS_ENABLED;
    config.Mqtt.Hass.Retain = MQTT_HASS_RETAIN;
    config.Mqtt.Hass.Expire = MQTT_HASS_EXPIRE;
    strlcpy(config.Mqtt.Hass.Topic, MQTT_HASS_TOPIC, sizeof(config.Mqtt.Hass.Topic));

    strlcpy(config.Security.Password, ACCESS_POINT_PASSWORD, sizeof(config.Security.Password));
    config.Security.AllowReadonly = SECURITY_ALLOW_READONLY;

    return true;
}

void ConfigurationClass::migrate()
{
    File f = LittleFS.open(CONFIG_FILENAME, "r", false);
    if (!f) {
        MessageOutput.println("Failed to open file, cancel migration");
        return;
    }

    JsonDocument doc;

    // Deserialize the JSON document
    const DeserializationError error = deserializeJson(doc, f);
    if (error) {
        MessageOutput.printf("Failed to read file, cancel migration: %s\r\n", error.c_str());
        return;
    }

    if (Utils::checkJsonAlloc(doc, __FUNCTION__, __LINE__) == false)
    {
        return;
    }

    // TODO: Version abändern!
    if (config.Cfg.Version < 0x00000001) {
        // This migration fixes this issue: https://github.com/espressif/arduino-esp32/issues/8828
        // It occours when migrating from Core 2.0.9 to 2.0.14
        // which was done by updating ESP32 PlatformIO from 6.3.2 to 6.5.0
        nvs_flash_erase();
        nvs_flash_init();
    }

    f.close();

    config.Cfg.Version = CONFIG_VERSION;
    write();
    read();
}

CONFIG_T& ConfigurationClass::get()
{
    return config;
}


ConfigurationClass Configuration;
