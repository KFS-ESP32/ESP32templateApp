// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <cstdint>

#define CONFIG_FILENAME "/config.json"
#define CONFIG_VERSION 0x00010001 // 0.01.01 // make sure to clean all after change

#define WIFI_MAX_SSID_STRLEN 32
#define WIFI_MAX_PASSWORD_STRLEN 64
#define WIFI_MAX_HOSTNAME_STRLEN 31

#define NTP_MAX_SERVER_STRLEN 31
#define NTP_MAX_TIMEZONE_STRLEN 50
#define NTP_MAX_TIMEZONEDESCR_STRLEN 50

#define MQTT_MAX_HOSTNAME_STRLEN 128
#define MQTT_MAX_CLIENTID_STRLEN 64
#define MQTT_MAX_USERNAME_STRLEN 64
#define MQTT_MAX_PASSWORD_STRLEN 64
#define MQTT_MAX_TOPIC_STRLEN 32
#define MQTT_MAX_LWTVALUE_STRLEN 20
#define MQTT_MAX_CERT_STRLEN 2560

struct CONFIG_T {
    struct {
        uint32_t AppID; // Projekt-Erkennung --> Evtl. Config löschen!
        uint32_t Version;
        uint32_t SaveCount;
    } Cfg;

    struct {
        char Ssid[WIFI_MAX_SSID_STRLEN + 1];
        char Password[WIFI_MAX_PASSWORD_STRLEN + 1];
        uint8_t Ip[4];
        uint8_t Netmask[4];
        uint8_t Gateway[4];
        uint8_t Dns1[4];
        uint8_t Dns2[4];
        bool Dhcp;
        char Hostname[WIFI_MAX_HOSTNAME_STRLEN + 1];
        uint32_t ApTimeout;
    } WiFi;

    struct {
        bool Enabled;
    } Mdns;

    struct {
        char Server[NTP_MAX_SERVER_STRLEN + 1];
        char Timezone[NTP_MAX_TIMEZONE_STRLEN + 1];
        char TimezoneDescr[NTP_MAX_TIMEZONEDESCR_STRLEN + 1];
        double Longitude;
        double Latitude;
        uint8_t SunsetType;
    } Ntp;

    struct {
        bool Enabled;
        char Hostname[MQTT_MAX_HOSTNAME_STRLEN + 1];
        uint32_t Port;
		char ClientId[MQTT_MAX_CLIENTID_STRLEN + 1];
        char Username[MQTT_MAX_USERNAME_STRLEN + 1];
        char Password[MQTT_MAX_PASSWORD_STRLEN + 1];
        char Topic[MQTT_MAX_TOPIC_STRLEN + 1];
        bool Retain;
        uint32_t PublishInterval;
        bool CleanSession;

        struct {
            char Topic[MQTT_MAX_TOPIC_STRLEN + 1];
            char Value_Online[MQTT_MAX_LWTVALUE_STRLEN + 1];
            char Value_Offline[MQTT_MAX_LWTVALUE_STRLEN + 1];
            uint8_t Qos;
        } Lwt;

        struct {
            bool Enabled;
            bool Retain;
            char Topic[MQTT_MAX_TOPIC_STRLEN + 1];
			bool IndividualPanels;
            bool Expire;
        } Hass;

        struct {
            bool Enabled;
            char RootCaCert[MQTT_MAX_CERT_STRLEN + 1];
            bool CertLogin;
            char ClientCert[MQTT_MAX_CERT_STRLEN + 1];
            char ClientKey[MQTT_MAX_CERT_STRLEN + 1];
        } Tls;
    } Mqtt;

    struct {
        char Password[WIFI_MAX_PASSWORD_STRLEN + 1];
        bool AllowReadonly;
    } Security;

};

class ConfigurationClass {
public:
    void init();
    bool read();
    bool write();
    void migrate();
    CONFIG_T& get();
};

extern ConfigurationClass Configuration;
