// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

enum WebApiError {
    GenericBase = 1000,
    GenericSuccess,
    GenericNoValueFound,
    GenericDataTooLarge, // not used anymore
    GenericParseError,
    GenericValueMissing,
    GenericWriteFailed,
    GenericInternalServerError,

    ConfigBase = 3000,
    ConfigNotDeleted,
    ConfigSuccess,

    MaintenanceBase = 6000,
    MaintenanceRebootTriggered,
    MaintenanceRebootCancled,

    MqttBase = 7000,
    MqttHostnameLength,
    MqttUsernameLength,
    MqttPasswordLength,
    MqttTopicLength,
    MqttTopicCharacter,
    MqttTopicTrailingSlash,
    MqttPort,
    MqttCertificateLength,
    MqttLwtTopicLength,
    MqttLwtTopicCharacter,
    MqttLwtOnlineLength,
    MqttLwtOfflineLength,
    MqttPublishInterval,
    MqttHassTopicLength,
    MqttHassTopicCharacter,
    MqttLwtQos,
    MqttClientIdLength,

    NetworkBase = 8000,
    NetworkIpInvalid,
    NetworkNetmaskInvalid,
    NetworkGatewayInvalid,
    NetworkDns1Invalid,
    NetworkDns2Invalid,
    NetworkApTimeoutInvalid,

    NtpBase = 9000,
    NtpServerLength,
    NtpTimezoneLength,
    NtpTimezoneDescriptionLength,
    NtpYearInvalid,
    NtpMonthInvalid,
    NtpDayInvalid,
    NtpHourInvalid,
    NtpMinuteInvalid,
    NtpSecondInvalid,
    NtpTimeUpdated,

    SecurityBase = 10000,
    SecurityPasswordLength,
    SecurityAuthSuccess,
};
