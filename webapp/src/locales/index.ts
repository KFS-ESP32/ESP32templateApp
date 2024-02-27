import type { I18nOptions } from "vue-i18n";

export enum Locales {
    EN = 'en',
    DE = 'de',
}

export const LOCALES = [
    { value: Locales.EN, caption: 'English' },
    { value: Locales.DE, caption: 'Deutsch' },
]

export const dateTimeFormats: I18nOptions["datetimeFormats"] = {
    [Locales.EN]: {
        'datetime': {
            hour: 'numeric',
            minute: 'numeric',
            second: 'numeric',
            year: 'numeric',
            month: 'numeric',
            day: 'numeric',
            hour12: false
        }
    },
    [Locales.DE]: {
        'datetime': {
            hour: 'numeric',
            minute: 'numeric',
            second: 'numeric',
            year: 'numeric',
            month: 'numeric',
            day: 'numeric',
            hour12: false
        }
    }
};

export const numberFormats: I18nOptions["numberFormats"] = {
    [Locales.EN]: {
        decimal: {
            style: 'decimal',
        },
        decimalNoDigits: {
            style: 'decimal', minimumFractionDigits: 0, maximumFractionDigits: 0
        },
        decimalTwoDigits: {
            style: 'decimal', minimumFractionDigits: 2, maximumFractionDigits: 2
        },
        percent: {
            style: 'percent',
        },
        kilobyte: {
            style: 'unit', unit: 'kilobyte',
        },
    },
    [Locales.DE]: {
        decimal: {
            style: 'decimal',
        },
        decimalNoDigits: {
            style: 'decimal', minimumFractionDigits: 0, maximumFractionDigits: 0
        },
        decimalTwoDigits: {
            style: 'decimal', minimumFractionDigits: 2, maximumFractionDigits: 2
        },
        percent: {
            style: 'percent',
        },
        kilobyte: {
            style: 'unit', unit: 'kilobyte',
        },
     }
};

export const defaultLocale = Locales.DE;