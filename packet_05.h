//
// Created by amir on 15.10.24.
//

#ifndef NMEA_CONVERTER_PACKET_05_H
#define NMEA_CONVERTER_PACKET_05_H

#include <stdint.h>

struct packet_00{               // Пакет для установления связи по сети Ethernet
    uint32_t TCmd = 0;          // Технологическая команда. Передавать 0
}; // 4 байта


#pragma pack(push,1)

struct packet_05{

    uint8_t Hdr = 0xA5;            // Синхробайт        = 0xA5
    uint8_t Id = 0x05;             // Идентификатор     = 0x05
    uint8_t Len = 0x3C;            // Длина тела пакета = 0x3C

    uint16_t sw1_ppa;
    uint16_t sw2_ppa;
    int vn_s;
    int ve_s;
    int vu_s;
    int lat_s;
    int lon_s; // 24
    int16_t alt_s; // Абсолютная барометрическая высота
    int16_t t_upd;
    uint16_t time; // Время (часы, минуты)
    uint16_t sec;  // Секунды
    uint16_t date; // Дата
    int16_t DOP;
    uint16_t n_sat; // Число спутников
    int16_t GDOP;
    int16_t HDOP;
    int16_t VDOP;
    int16_t v_hdg;
    int16_t VFOM; // СКО вертикальных координат
    int16_t HFOM; // СКО горизонтальных координат
    uint16_t date_SW;
    uint16_t ver_SW;
    uint16_t sat_info;
    uint16_t ch_info;
    uint16_t az_el; // 18 * 2 + 24 = 60

    uint8_t crc = 0;
};
#pragma pack(pop)


class packet{

public:

    void set_vn_s(){ }
    void set_ve_s(){ }
    void set_vu_s(){ }
    void set_lat_s(){ }
    void set_lon_s(){ }
    void set_alt_s(){ }
    void set_time(){ }
    void set_sec(){ }
    void set_date(){ }
    void set_n_sat(){ }
    void set_VFOM(){ }
    void set_HFOM(){ }

    uint8_t buffer[300];
    packet_05* data = (packet_05*)buffer;
    uint32_t size = sizeof(packet_05);

};

#endif //NMEA_CONVERTER_PACKET_05_H
