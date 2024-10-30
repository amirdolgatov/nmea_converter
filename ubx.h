//
// Created by amir on 15.10.24.
//

#ifndef NMEA_CONVERTER_UBX_H
#define NMEA_CONVERTER_UBX_H

#include <stdint.h>
#include <vector>
#include <byteswap.h>

#define TIME_INTERVAL 0.2
#define  SYNC_CHAR_1 0xB5
#define  SYNC_CHAR_2 0x62



enum MSG_ID{
    NAV_PVT = 0x0107,
    NAV_SAT = 0x0135,
    NAV_SIG = 0x0143,
    UNKNOWN = 0x0000
};


//using time_val = std::chrono::time_point<std::chrono::steady_clock>;

struct ubx_header{
    uint8_t sync_1;
    uint8_t sync_2;
    uint16_t id;
    uint16_t length;
    uint8_t data;
};


struct nav_pvt_payload{
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
};

struct nav_sig_payload{
    ;
};

struct nav_sat_payload{
    ;
};

struct received_data{
    nav_pvt_payload* nav_pvt;
    nav_sig_payload* nav_sig;
    nav_sat_payload* nav_sat;
};


struct message{
    MSG_ID msg_id;
    uint8_t data[500];              // буфер с данными
    size_t size;                    // размер полученного сообщения

    uint8_t* get_raw_data(){
        return data;
    }

    size_t get_size(){
        return size;
    }

    void set_size(size_t size){
        this->size = size;
    }

    void add_element(uint8_t byte){
        data[size++] = byte;
    }

    void clear(){
        size = 0;
    }
};


bool check_summ(message& input_msg){

    ubx_header* header = (ubx_header*) input_msg.get_raw_data();
    uint8_t* msg = (uint8_t*)input_msg.get_raw_data();
    size_t msg_size = input_msg.get_size();

    if(header->sync_1 != SYNC_CHAR_1 || header->sync_2 != SYNC_CHAR_2)
        return false;

    if(header->length != msg_size - 8)
        return false;

    uint8_t* data = input_msg.get_raw_data();
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;
    for (int i = 2; i < header->length + 2; ++i) {
        CK_A = CK_A + data[i];
        CK_B = CK_B + CK_A;
    }

    if(msg[msg_size - 2] != CK_A /* || msg[msg_size - 1] != CK_B */ ){
        return false;
    }


    return true;
}

nav_pvt_payload* parse_nav_pvt(uint8_t* data){
    return (nav_pvt_payload*) data;
}

nav_sig_payload* parse_nav_sig(uint8_t* data){
    return (nav_sig_payload*) data;
}

nav_sat_payload* parse_nav_sat(uint8_t* data){
    return (nav_sat_payload*) data;
}

bool parse_ubx(message& input_msg, received_data& received_data){
// Чтобы наполнить пакет_05 нужно извлечь информацию из нескольких сообщений,
// Стоит ли отправлять информацию если обновлена только часть полей? Нет.
// Пакет готов, если все поля относятся к одному моменту времени
// Ждем получения ubx сообщений X, Y, Z, ... Затем начинаем парсинг.
//
    /// ?????

    if(! check_summ(input_msg))
        return false;



    ubx_header* header = (ubx_header*) input_msg.get_raw_data();

    switch(bswap_16(header->id)){
        case UBX_NAV_PVT:
            received_data.nav_pvt = parse_nav_pvt(&header->data);
            break;
        case UBX_NAV_SAT:
            ;
            break;
        case UBX_NAV_SIG:
            ;
            break;
        default:
            ;
    }
    return true;
}



class msg_container{
public:

    enum STATES{
        WAIT_NAV_PVT,
        WAIT_NAV_SAT
    };

    msg_container(size_t size = 3):size{size}
    {
        msg_buf.reserve(size);
        state = WAIT_NAV_PVT;
    }

    /*!
     * добавляем вновь полученное сообщение
     * если возвращается true - пакет с сообщениями готов для парсинга
     */

    bool add(message msg){

        switch(msg.msg_id){
            case MSG_ID::UBX_NAV_PVT:
                if(state == WAIT_NAV_PVT){
                    std::cout << "Receive UBX_NAV_PVT" << std::endl;
                    msg_ptr = 0;
                    msg_buf[msg_ptr++] = msg;
                    state = WAIT_NAV_SAT;
                    return false;
                }
                break;
            case MSG_ID::UBX_NAV_SAT:
                if(state == WAIT_NAV_SAT){
                    std::cout << "Receive UBX_NAV_SAT" << std::endl;
                    msg_buf[msg_ptr] = msg;
                    state = WAIT_NAV_PVT;
                    std::cout << "get 2 msg" << std::endl;
                    return true;
                }
                break;
            default:
                break;
        }



    }

    std::vector<message>& get_msg(){
        return msg_buf;
    }

    STATES state = WAIT_NAV_PVT;
    std::vector<message> msg_buf;
    size_t msg_ptr = 0;
    size_t size = 2;

};


#endif //NMEA_CONVERTER_UBX_H
