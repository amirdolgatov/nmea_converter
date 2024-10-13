//
// Created by amir on 14.10.24.
//

#ifndef NMEA_CONVERTER_PARSER_H
#define NMEA_CONVERTER_PARSER_H

#include <string>
#include <vector>

/*!
 * Time, position, and fix related data for the receiver
 *  Информация о фиксированном решении
 */
struct GGA{
    tm time;
    float latitude{0}; // широта
    float longitude{0}; // долгота
    int number_of_SV{0};
    int height{0};    // высота
};

/*!
 * GLL-Geographic Position-Latitude/Longitude
 */
struct GLL{
    tm time;
    int latitude{0}; // широта
    int longitude{0}; // долгота
    int height{0};    // высота
};

/*!
 * DHV-Design Hourly Volume
 */
struct DHV{
    int spdX{0};
    int spdY{0};
    int spdZ{0};
};

/*!
 * ZDA-Time & Date
 */
struct ZDA{
    tm date;
};

/*!
 * GST-GNSS Pseudorange Error Statistics
 */
struct GST{
    int stdDevMaj{0}; // отклонение по горизонтали
    int stdDevMin{0}; // отклонения по вертикали
};

struct GGA* parse_GGA(std::vector<std::string>& msg, uint8_t* buffer){

    if(msg.size() < 14)
        return nullptr;

    float grad;
    float min;

    struct GGA* data = (struct GGA*)buffer;

    std::string time_UTC = msg[0];

    data->time.tm_hour = atoi(time_UTC.substr(0, 2).c_str());
    data->time.tm_min = atoi(time_UTC.substr(2, 2).c_str());
    data->time.tm_sec = atoi(time_UTC.substr(4, 2).c_str());

    grad = atof(msg[1].substr(0, 2).c_str());
    min = atof(msg[1].substr(2, 8).c_str());
    data->latitude = grad + min / 60.0;

    grad = atof(msg[3].substr(0, 2).c_str());
    min = atof(msg[3].substr(2, 8).c_str());
    data->longitude = grad + min / 60.0;

    data->number_of_SV = atoi(msg[6].c_str());
    data->height = atof(msg[8].c_str());

    return data;
}


struct DHV* parse_DHV(std::vector<std::string>& msg, uint8_t* buffer){

    if(msg.size() < 6)
        return nullptr;

    struct DHV* data = (struct DHV*)buffer;

    data->spdX = atof(msg[2].c_str());
    data->spdY = atof(msg[3].c_str());
    data->spdZ = atof(msg[4].c_str());
    return data;
}

struct GST* parse_GST(std::vector<std::string>& msg, uint8_t* buffer){

    if(msg.size() < 8)
        return nullptr;

    struct GST* data = (struct GST*)buffer;

    data->stdDevMaj = atof(msg[2].c_str());
    data->stdDevMin = atof(msg[3].c_str());
    return data;
}

struct ZDA* parse_ZDA(std::vector<std::string>& msg, uint8_t* buffer){

    if(msg.size() < 6)
        return nullptr;

    struct ZDA* data = (struct ZDA*)buffer;

    data->date.tm_mday = atoi(msg[1].c_str());
    data->date.tm_mon = atoi(msg[2].c_str());
    data->date.tm_year = atoi(msg[3].c_str());

    return data;
}

#endif //NMEA_CONVERTER_PARSER_H
