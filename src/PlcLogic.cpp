// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2024 Karlfried S. and others
 */
#include <Arduino.h>
#include "PlcLogic.h"

PlcLogicClass PlcLogic;

PlcLogicClass::PlcLogicClass()
{
}

// Edge detection
bool PlcLogicClass::isRisingEdge(bool Signal, int FlagNo)
{
    bool Edge = false;

    if (Signal == true && _edgeDetection[FlagNo] == false)
    {
        Edge = true;
    }

    _edgeDetection[FlagNo] = Signal;

    return Edge;
}

// Edge detection
bool PlcLogicClass::isFallingEdge(bool Signal, int FlagNo)
{
    bool Edge = false;

    if (Signal == false && _edgeDetection[FlagNo] == true)
    {
        Edge = true;
    }

    _edgeDetection[FlagNo] = Signal;

    return Edge;
}

// IEC_TIMER TON: Switch-on delay / Einschaltverzögerung
bool PlcLogicClass::isIEC_TIMER_TON(int TimerNo, bool Signal, unsigned int DelayTime /*ms*/)
{
    // Is timer already stopped
    if (Signal == false && _IEC_TON[TimerNo].lastSignalState == false)
    {
        /* nothing to do */
    }
    // Is time already expired
    else if (Signal == true && _IEC_TON[TimerNo].Q == true)
    {
        /* nothing to do */
    }
    // any edge of signal
    else if (Signal != _IEC_TON[TimerNo].lastSignalState)
    {
        if (Signal == false)
        {
            // Stop time with negative edge of signal
            _IEC_TON[TimerNo].lastSignalState = false;
            _IEC_TON[TimerNo].Q = false;
        }
        else
        {
            // Start time with positive edge of signal
            _IEC_TON[TimerNo].lastSignalState = true;
            _IEC_TON[TimerNo]._startTime = millis();
        }
    }
    // Time expired ?
    else if (millis() > _IEC_TON[TimerNo]._startTime + DelayTime)
    {
        _IEC_TON[TimerNo].Q = true;
    }

    return _IEC_TON[TimerNo].Q;
}


