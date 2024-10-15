//
// Created by amir on 15.10.24.
//

#ifndef NMEA_CONVERTER_UBX_H
#define NMEA_CONVERTER_UBX_H

#include <stdint.h>

struct ubx_frame{

    uint16_t header;
    uint8_t type;
    uint8_t id;
    uint16_t length;

    uint32_t iTOW;
    uint16_t year;      // Year (UTC)
    uint8_t month;      // Month, range 1..12 (UTC)
    uint8_t day;        // Day of month, range 1..31 (UTC)
    uint8_t hour;       // Hour of day, range 0..23 (UTC)
    uint8_t min;        // Minute of hour, range 0..59 (UTC)
    uint8_t sec;        // Seconds of minute, range 0..60 (UTC)
    uint8_t valid;
    uint32_t tAcc;      // Time accuracy estimate (UTC)
    int nano;           // Fraction of second, range -1e9 .. 1e9 (UTC)
    uint8_t fixType;    // GNSS fix Type: 0: no fix 1: dead reckoning only 2: 2D-fix 3: 3D-fix 4: GNSS + dead reckoning combined 5: time only fix
    uint8_t flags;      // Fix status flags
    uint8_t flags2;
    uint8_t numSV;      // Number of satellites used in Nav Solution
    int lon;            // Longitude
    int lat;            // Latitude
    int height;         // Height above ellipsoid
    int hMSL;           // Height above mean sea level
    uint32_t Acc;       // Horizontal accuracy estimate
    uint32_t vAcc;      // Vertical accuracy estimate
    int velN;           // NED north velocity
    int velE;           // NED east velocity
    int velD;           // NED down velocity
    int gSpeed;         //
    int headMot;
    uint32_t sAcc;
    uint32_t headAcc;
    uint16_t pDOP;
    uint8_t flags3;
    uint8_t reserved1;
    int headVeh;
    int16_t magDec;
    uint16_t magAcc;

    uint8_t CK_A;
    uint8_t CK_B;
};

#endif //NMEA_CONVERTER_UBX_H
