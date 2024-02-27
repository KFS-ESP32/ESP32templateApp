/* #######################################
   # TODO: Anpassen an aktuelle Istwerte #
   #######################################
*/

// The Server site is definied in 'WebApi_ws_live.cpp'
// --------------------------------------------------

export interface System {
    lifeCounter: number;
}

export interface CommmData {
    name: string;
    runtime_sec: number;
    limit_absolute: number;
    telegramCounter: number;
}

export interface Hints {
    time_sync: boolean;
    default_password: boolean;
}
export interface LiveData {
    commonData: CommmData[];
    hints: Hints;
    system: System;
}
