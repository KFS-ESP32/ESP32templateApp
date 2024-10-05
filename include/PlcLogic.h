// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Edge index
#define edgeXY1p 0
#define edgeXY2n 1
#define edgeXY3p 2

// IEC TIMER index
#define IEC_TIMER_XY1p 0
#define IEC_TIMER_XY2n 1
#define IEC_TIMER_XY3p 2


class PlcLogicClass
{
public:
    PlcLogicClass();
	bool isRisingEdge(bool Signal, int FlagNo);
    bool isFallingEdge(bool Signal, int FlagNo);
    bool isIEC_TIMER_TON(int TimerNo, bool Signal, unsigned int DelayTime /*ms*/);

private:
    // Edge detection
    bool _edgeDetection[11] = {};

    // IEC_TIMER TON: Switch-on delay / Einschaltverzögerung
    typedef struct struct_IEC_TON
    {
        bool lastSignalState;
        unsigned long _startTime;
        bool Q;
    } IEC_TON;
    IEC_TON _IEC_TON[11] = {};
};

extern PlcLogicClass PlcLogic;
