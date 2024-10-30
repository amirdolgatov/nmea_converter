//
// Created by amir on 27.10.24.
//

#ifndef NMEA_CONVERTER_GET_UBX_MSG_H
#define NMEA_CONVERTER_GET_UBX_MSG_H

#include "ubx.h"


struct ubx_msg_buffer{

    enum STATES{
        WAIT_NAV_PVT,
        WAIT_NAV_SAT
    };

    STATES state = WAIT_NAV_PVT;

    bool data_ready = false;

    bool get_data_ready(){
        return data_ready;
    }

    void add(message&& msg){
        switch (state) {
            case WAIT_NAV_PVT:
            {
                if(msg.msg_id == NAV_PVT){
                    nav_pvt = msg;
                    state = WAIT_NAV_SAT;
                }
                data_ready = false;
            }
            break;
            case WAIT_NAV_SAT:
            {
                if(msg.msg_id == NAV_SAT){
                    nav_sat = msg;
                    state = WAIT_NAV_PVT;
                    data_ready = true;
                }
            }
                break;
            default:
                data_ready = false;
        }
    }

    void data_used(){
        data_ready = false;
    }

    message& get_nav_pvt(){
        return nav_pvt;
    }

    message& get_nav_sat(){
        return nav_sat;
    }

    message nav_pvt;
    message nav_sat;
};




struct ubx_receiver{

    enum States{
        SEARCH_SYNC_1,
        SEARCH_SYNC_2,
        SEARCH_ID_1,
        SEARCH_ID_2,
        SEARCH_LENGTH_1,
        SEARCH_LENGTH_2,
        PAYLOAD,
        CHECK_SUMM_A,
        CHECK_SUMM_B
    };

    /*!
     * Функция позволяет получить тип сообщения
     * способом не зависящим от порядка байт (little endian, big endian)
     * @param type 2 байта
     * @return тип сообщения
     */
    MSG_ID get_msg_type(uint8_t type[2]){
        if(type[0] == 0x01 && type[1] == 0x07)
            return NAV_PVT;
        if(type[0] == 0x01 && type[1] == 0x35)
            return NAV_SAT;
        else
            return UNKNOWN;
    }

    /*!
     * Функция позволяет получить длину сообщения
     * способом не зависящим от порядка байт (little endian, big endian)
     * @param type 2 байта длины
     * @return длина сообщения
     */
    uint16_t get_msg_len(uint8_t type[2]){
        uint16_t ret = type[1];
        ret = ret << 8;
        ret = ret + type[0];
        return ret;
    }

    bool add_byte(uint8_t byte){

        switch(state){
            case SEARCH_SYNC_1:
                if(byte == 0xB5){
                    state = SEARCH_SYNC_2;
                    tmp.add_element(byte);
                }
                break;
            case SEARCH_SYNC_2:
                if(byte == 0x62){
                    state = SEARCH_ID_1;
                    tmp.add_element(byte);
                }
                else{
                    state = SEARCH_SYNC_1;
                    tmp.clear();
                }
                break;
            case SEARCH_ID_1:
            {
                state = SEARCH_ID_2;
                type[0] = byte;
                tmp.add_element(byte);
                CHK_A = CHK_A + byte;
                CHK_B = CHK_B + CHK_A;
            }
                break;
            case SEARCH_ID_2:
            {
                state = SEARCH_LENGTH_1;
                type[1] = byte;
                tmp.add_element(byte);
                tmp.msg_id = get_msg_type(type);  //  зафиксировали тип текущего сообщения
                CHK_A = CHK_A + byte;
                CHK_B = CHK_B + CHK_A;
            }
                break;
            case SEARCH_LENGTH_1:
            {
                state = SEARCH_LENGTH_2;
                length[0] = byte;
                tmp.add_element(byte);
                CHK_A = CHK_A + byte;
                CHK_B = CHK_B + CHK_A;
            }
                break;
            case SEARCH_LENGTH_2:
            {
                state = PAYLOAD;
                length[1] = byte;
                tmp.add_element(byte);
                _length = get_msg_len(length);
                CHK_A = CHK_A + byte;
                CHK_B = CHK_B + CHK_A;
            }
                break;
            case PAYLOAD:
            {
                payload_count++;
                tmp.add_element(byte);
                CHK_A = CHK_A + byte;
                CHK_B = CHK_B + CHK_A;
                if(payload_count == _length)
                    state = CHECK_SUMM_A;
            }
                break;
            case CHECK_SUMM_A:
            {
                if(byte == CHK_A){
                    state = CHECK_SUMM_B;
                    tmp.add_element(byte);
                }
                else{
                    state = SEARCH_SYNC_1;
                    tmp.clear();
                }

            }
                break;
            case CHECK_SUMM_B:
            {
                if(byte == CHK_B){
                    tmp.add_element(byte);
                    buffer.add(std::move(tmp));
                }
                state = SEARCH_SYNC_1;
                tmp.clear();
                payload_count = 0;
                CHK_A = 0;
                CHK_B = 0;
            }
                break;
        }
    }

    bool data_ready(){
        return buffer.get_data_ready();
    }

    ubx_msg_buffer& get_msg_buffer(){
        return buffer;
    }

    States state = SEARCH_SYNC_1;
    message tmp;
    uint8_t type[2];
    uint8_t length[2];
    MSG_ID _type;
    uint16_t _length = 0;
    uint16_t payload_count = 0;

    uint8_t CHK_A = 0;
    uint8_t CHK_B = 0;

    ubx_msg_buffer buffer;

};



void get_ubx_msg(uint8_t *buf, size_t size){

    for(int i  = 0; i < size; ++i)
        ;
}

#endif //NMEA_CONVERTER_GET_UBX_MSG_H
