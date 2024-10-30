//
// Created by amir on 13.10.24.
//

#ifndef NMEA_CONVERTER_PARSER_NMEA_H
#define NMEA_CONVERTER_PARSER_NMEA_H

#include <string>
#include <vector>
#include "parser.h"

//enum SOURCES{
//    GP, //информация получена от спутников GPS (США).
//    GL, //информация получена от спутников Глонасс (Россия).
//    GA, //информация получена от спутников Galileo (Европа).
//    BD, //информация получена от спутников Beidou (Китай).
//    GQ, //информация получена от спутников QZSS (Япония).
//    GN, //информация получена от спутников разных навигационных систем.
//    NN  //ошибка
//};
//
//enum MESSAGE_TYPE{
//    GGA, //Данные о последнем зафиксированном местоположении.
//    GLL, //Географические координаты.
//    GSA, //Информация об активных спутниках (участвующих в позиционировании).
//    GSV, //Информация об активных спутниках (участвующих в позиционировании).
//    ZDA, //Дата и время.
//    DHV, //Информация о скорости движения GNSS приемника.
//    GST, //Статистика ошибок позиционирования.
//    NNN  //ошибка
//};
//
//
//
//struct Packet_05{
//    uint16_t sw1_ppa;
//    uint16_t sw2_ppa;
//    int vn_s;
//    int ve_s;
//    int vu_s;
//    float lat_s;
//    float lon_s; // 24
//    int16_t alt_s;
//    int16_t t_upd;
//    uint16_t time;
//    uint16_t sec;
//    uint16_t date;
//    int16_t DOP;
//    uint16_t n_sat;
//    int16_t GDOP;
//    int16_t HDOP;
//    int16_t VDOP;
//    int16_t v_hdg;
//    int16_t VFOM;
//    int16_t HFOM;
//    uint16_t date_SW;
//    uint16_t ver_SW;
//    uint16_t sat_info;
//    uint16_t ch_info;
//    uint16_t az_el; // 18 * 2 + 24 = 60
//};
//
//
//class nmea_parser{
//
//    // $GNGGA,102030.000,5546.95900,N,03740.69200,E,1,08,2.7,142.0,M,0.0,M,,*FF/r/n
//
//    /*!
//     * split string by symbol
//     * @param str
//     * @param d divider
//     * @return
//     */
//    std::vector<std::string> split(const std::string &str, char d)
//    {
//        std::vector<std::string> r;
//
//        std::string::size_type start = 0;
//        std::string::size_type stop = str.find_first_of(d);
//        while(stop != std::string::npos)
//        {
//            r.push_back(str.substr(start, stop - start));
//
//            start = stop + 1;
//            stop = str.find_first_of(d, start);
//        }
//        r.push_back(str.substr(start));
//        return r;
//    }
//
//    void* get_packet(){
//        return &udp_packet;
//    }
//
//    bool get_line(std::string& line){
//
//        // remove '$'
//        line.erase(0, 1);
//        auto it = line.begin();
//
//        while( it != line.end() && *it != '*')
//            it++;
//
//        // error if fail to find '*'
//        if(it == line.end())
//            return false;
//
//        // remove all symbols from '*'
//        line.erase(it, line.end());
//
//        return true;
//    }
//
//    bool parse_msg(std::string input){
//        auto nmea_msg = split(input, '\r');
//
//        for(auto& line: nmea_msg){
//            if(!parse_line(line))
//                return false;
//        }
//    }
//
//
//    bool parse_line(std::string& line){
//        if(!get_line(line))
//            return false;
//
//        auto msg_objects = split(line, ',');
//
//        auto header = msg_objects[0];
//
//        auto type = get_source(header);
//
//        uint8_t data[100];
//
//        switch(type){
//
//            case MESSAGE_TYPE::GGA: {
//                this->source = get_source(header); // если это GGA, зафиксируем решение
//                auto gga_data = parse_GGA(msg_objects, data);
//                if (gga_data == nullptr)
//                    return false;
//                udp_packet.time = gga_data->time.tm_mday; // спросить про формат
//                udp_packet.lat_s = gga_data->latitude; // спросить про формат
//                udp_packet.lon_s = gga_data->longitude; // спросить про формат
//                udp_packet.n_sat = gga_data->number_of_SV; // штуки
//                udp_packet.alt_s = gga_data->height; // метры
//                return true;
//            }
//
//            case MESSAGE_TYPE::DHV: {
//                auto dhv_data = parse_DHV(msg_objects, data);
//                if (dhv_data == nullptr)
//                    return false;
//                udp_packet.vn_s = dhv_data->spdX;
//                udp_packet.ve_s = dhv_data->spdY;
//                udp_packet.vu_s = dhv_data->spdZ;
//                return true;
//            }
//
//            case MESSAGE_TYPE::GST: {
//                auto gst_data = parse_GST(msg_objects, data);
//                if (gst_data == nullptr)
//                    return false;
//                udp_packet.VFOM = gst_data->stdDevMaj;
//                udp_packet.HFOM = gst_data->stdDevMin;
//                return true;
//            }
//
//            case MESSAGE_TYPE::ZDA: {
//                auto zda_data = parse_ZDA(msg_objects, data);
//                if (zda_data == nullptr)
//                    return false;
//                udp_packet.date = zda_data->date.tm_year;  // как передается дата ?
//                return true;
//            }
//        }
//
//    }
//
//    MESSAGE_TYPE get_type(std::string& header){
//        auto type = header.substr(2, 3);
//        if(type == "GGA")
//            return MESSAGE_TYPE::GGA;
//        else if(type == "GLL")
//            return MESSAGE_TYPE::GLL;
//        else if(type == "DHV")
//            return MESSAGE_TYPE::DHV;
//        else if(type == "GSA")
//            return MESSAGE_TYPE::GSA;
//        else if(type == "GST")
//            return MESSAGE_TYPE::GST;
//        else if(type == "ZDA")
//            return MESSAGE_TYPE::ZDA;
//        else
//            return MESSAGE_TYPE::NNN;
//    }
//
//    SOURCES get_source(std::string& header){
//        auto src = header.substr(0, 2);
//        if(src == "GP")
//            return SOURCES::GP;
//        else if(src == "GL")
//            return SOURCES::GL;
//        else if(src == "GA")
//            return SOURCES::GA;
//        else if(src == "BD")
//            return SOURCES::BD;
//        else if(src == "GN")
//            return SOURCES::GN;
//        else if(src == "GQ")
//            return SOURCES::GQ;
//        else
//            return SOURCES::NN;
//    }
//
//
//    SOURCES source;
//    MESSAGE_TYPE msg_type;
//    uint8_t data[100];
//    Packet_05 udp_packet;
//};

#endif //NMEA_CONVERTER_PARSER_NMEA_H
