//
// Created by amir on 22.10.24.
//

#ifndef NMEA_CONVERTER_UBX_PARSER_H
#define NMEA_CONVERTER_UBX_PARSER_H

#include "Serial.h"
#include "ubx.h"
#include "udp_server.h"
#include "packet_05.h"
#include <unistd.h>
#include <fcntl.h>

class ubx_converter{
public:
    ubx_converter(int tty):server{{42000, "192.168.20.19"}}
    {}

    bool init(){

        if(uart.open(1, 115000, 0, 1, 0) < 0){
            std::cout << "Can't init uart server!" << std::endl;
            return false;
        }

        if(! server.init()){
            std::cout << "Can't init udp server!" << std::endl;
            return false;
        }

        return true;
    }


    void start(){
        message ubx_msg;
        int n = 0;

        while(1){
            if(n = uart.get(ubx_msg.get_raw_data(), 500); n > 0) {
                ubx_msg.set_size(n);
                std::cout << "Receive message from UART " << n << " bytes " << std::endl;
                std::cout << std::hex;
                for (int i = 0; i < n; ++i) {
                    std::cout << (int)ubx_msg.data[i] << " ";
                    if(i % 15 == 0)
                        std::cout << std::endl;
                }
                if(container.add(ubx_msg)){
                    int i = 0;
                    for ( ; i < container.msg_buf.size(); ++i) {
                        if(! parse_ubx(container.msg_buf[i], data))
                            break;
                    }
                    if (i == container.msg_buf.size()){
                        std::cout << "Get " << i << " packets" << std::endl;
                        std::cout << data.nav_pvt << std::endl;
                        std::cout << udp_packet.data << std::endl;
                        update_packet();
                        server.send_data(udp_packet.buffer, udp_packet.size);
                    }
                }
            }
        }
    }

    void start_connection(){
        std::thread thr{
            [this](){
                uint32_t msg = 0;
                while(1){
                    server.send_data((uint8_t*)&msg, sizeof(msg));
                    sleep(1);
                }
            }
        };
    }

    void update_packet(){
        udp_packet.data->vn_s = data.nav_pvt->velN;
        udp_packet.data->ve_s = data.nav_pvt->velE;
        udp_packet.data->vu_s = data.nav_pvt->velD;

        udp_packet.data->lat_s = data.nav_pvt->lat;
        udp_packet.data->lon_s = data.nav_pvt->lon;

        udp_packet.data->VFOM = data.nav_pvt->vAcc;
        udp_packet.data->HFOM = data.nav_pvt->Acc;

        udp_packet.data->n_sat = data.nav_pvt->numSV;

        udp_packet.data->alt_s = data.nav_pvt->height;

        udp_packet.data->time = data.nav_pvt->year;
        udp_packet.data->date = data.nav_pvt->month;
        udp_packet.data->sec = data.nav_pvt->sec;
    }

    msg_container container;
    received_data data;
    udp_server server;
    packet udp_packet;

    SerialPort uart;

    int tty;

};

#endif //NMEA_CONVERTER_UBX_PARSER_H
