//
// Created by amir on 14.10.24.
//

#ifndef NMEA_CONVERTER_UDP_SERVER_H
#define NMEA_CONVERTER_UDP_SERVER_H

// Server side implementation of UDP client-server model
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mutex>

#include "parser.h"

#define PORT 8080
#define MAXLINE 1024

#pragma once

//============================================================================
//Объявление типов данных, которые не зависят от компилятора
typedef  unsigned int   uint;
typedef  unsigned char  uchar;
typedef  unsigned short ushort;
typedef  unsigned long  ulong;


//============================================================================
// Объявления типов данных для различных компиляторов
#if defined(__GNUC__)                // Используется компилятор GNU C

#if defined(__QNX__)             // GNU C for QNX Neutrino 6.x
#include <sys/types.h>

        typedef  _int8          int8;
        typedef  _int16         int16;
        typedef  _int32         int32;
        typedef  _int64         int64;

        typedef  _uint8         uint8;
        typedef  _uint16        uint16;
        typedef  _uint32        uint32;
        typedef  _uint64        uint64;

        #define  SizeOfInt64    8    // Есть поддержка 64-битных целых

#elif defined(__MICROBLAZE__)    // GNU C for Xilinx MicroBlaze
#include <xbasic_types.h>

        typedef  Xint8          int8;
        typedef  Xint16         int16;
        typedef  Xint32         int32;
        typedef  long long      int64;

        typedef  Xuint8         uint8;
        typedef  Xuint16        uint16;
        typedef  Xuint32        uint32;
        typedef  Xuint64        uint64;

        #define  SizeOfInt64    8    // Есть поддержка 64-битных целых

#elif defined(__MIPSEL__)        // GNU C для MIPS процессоров фирмы Элвис (1892ВМ10Я)
typedef  signed   char       int8;
        typedef  short    int        int16;
        typedef  long     int        int32;
        typedef  long     long       int64;

        typedef  unsigned char       uint8;
        typedef  unsigned short      uint16;
        typedef  unsigned long       uint32;
        typedef  unsigned long long  uint64;

        #define  SizeOfInt64    8    // Есть поддержка 64-битных целых
#elif defined(__ARMCC_VERSION)   // Keil uVision
#include <stdint.h>
        typedef signed char        int8;
        typedef short              int16;
        typedef int                int32;
        typedef int64_t            int64;
        typedef unsigned char      uint8;
        typedef unsigned short     uint16;
        typedef unsigned int       uint32;
        typedef uint64_t           uint64;

        #define  SizeOfInt64    8    // Есть поддержка 64-битных целых
#else //GNU C for unknown OS
#include <stdint.h>
typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef int64_t            int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef uint64_t           uint64;
#define  SizeOfInt64    8    // Есть поддержка 64-битных целых
#endif

#elif defined(_MSC_VER)              // Используется компилятор Microsoft C/C++ (OS Windows)
#include <stddef.h>

    typedef  __int8             int8;
    typedef  __int16            int16;
    typedef  __int32            int32;
    typedef  __int64            int64;

    typedef  unsigned __int8    uint8;
    typedef  unsigned __int16   uint16;
    typedef  unsigned __int32   uint32;
    typedef  unsigned __int64   uint64;

    #define  SizeOfInt64    8        // есть поддержка 64-битных целых

#elif defined(__BORLANDC__)          // Используется компилятор Borland C++ (OS Windows, Dos)
    #include <stddef.h>

    typedef  signed char    int8;
    typedef  short int      int16;
    typedef  long int       int32;
    typedef  int32          int64;   // Заплатка (Borland C++ 5.x не поддерживает 64-разрядную арихметику)

    typedef  unsigned char  uint8;
    typedef  unsigned short uint16;
    typedef  unsigned long  uint32;
    typedef  uint32         uint64;  // Заплатка (Borland C++ 5.x не поддерживает 64-разрядную арихметику)

    #define  SizeOfInt64    4        // 64-битные целые не поддерживаются

#elif defined(__ARMCC_VERSION)       // Keil uVision
    #include <stdint.h>
    typedef signed char        int8;
    typedef short              int16;
    typedef int                int32;
    typedef int64_t          int64;
    typedef unsigned char      uint8;
    typedef unsigned short     uint16;
    typedef unsigned int       uint32;
    typedef uint64_t           uint64;

    #define  SizeOfInt64    8    // Есть поддержка 64-битных целых

#else
    #error "Unknown compiller"
#endif // compilers

//============================================================================
// Объявления типов данных, используемых в ПО, разрабатываемом в АО «ИТТ»

typedef  uint8        byte;      // Беззнаковые целые числа, 8 бит
typedef  uint16       word;      // Беззнаковые целые числа, 16 бит
typedef  uint32       dword;     // Беззнаковые целые числа, 32 бита
typedef  uint64       qword;     // Беззнаковые целые числа, 64 бита

typedef  int8         cfix;      // Представление чисел в формате с фиксированной точкой, 8 бит (Char)
typedef  int16        hfix;      // Представление чисел в формате с фиксированной точкой, 16 бит (sHort)
typedef  int32        lfix;      // Представление чисел в формате с фиксированной точкой, 32 бита (Long)
typedef  int64        Lfix;      // Представление чисел в формате с фиксированной точкой, 64 бита (LLong)

typedef  uint8        ucfix;     // Представление беззнаковых чисел в формате с фиксированной точкой, 8 бит
typedef  uint16       uhfix;     // Представление беззнаковых чисел в формате с фиксированной точкой, 16 бит
typedef  uint32       ulfix;     // Представление беззнаковых чисел в формате с фиксированной точкой, 32 бита
typedef  uint64       uLfix;     // Представление беззнаковых чисел в формате с фиксированной точкой, 64 бита

typedef  lfix         afix;      // Представление чисел в формате с фиксированной точкой (для использования в ARINC), 32 бита
typedef  lfix         rfix;      // Представление чисел в формате с фиксированной точкой (для использования в РТМ), 32 бита
// Типы данных afix и rfix по-разному обрабатываются только в программах проверок и калибровок ("Смотрелках")

typedef  long double  ldouble;   // Стандартный тип данных языка С++ (long double), 80 бит (intel)
// Внимание! Microsoft Visual Studio c 2005г такие числа не поддерживает, считая их 64-битными.


//=============================================================================
// Слово Состояния 1 ППА-С/В, SW1_PPA
//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//efine SWP1_                       0x0001U     // 19:
#define SWP1_BEIDOU                 0x0002U     // 18: Работа по Beidou
#define SWP1_GALILEO                0x0004U     // 17: Работа по Galileo
#define SWP1_DIF_REG_LDPS           0x0008U     // 16: Наличие дифрежима по ЛДПС
#define SWP1_UPD_DKI                0x0010U     // 15: Обновлённый массив ДКИ
#define SWP1_DIF_REG                0x0020U     // 14: Работа в диф. режиме
#define SWP1_ERORR_SUPPRESS         0x0040U     // 13: Подавление помех
#define SWP1_PPS                    0x0080U     // 12: Метка времени 1с
#define SWP1_SYNC                   0x0100U     // 11: 1 - синхронизация, 0 - навигация
#define SWP1_COORD_SYS              0x0600U     // 9, 10: Система координат
#define SWP1_SK42                   0x0000U     // 0,0 - СК-42
#define SWP1_WGS84                  0x0200U     // 0,1 - WGS-84
#define SWP1_PE90                   0x0400U     // 1,0 - ПЗ-90
#define SWP1_SK95                   0x0600U     // 1,1 - СК-95
#define SWP1_GLO_HP                 0x0800U     // 8:  Работа с ВТ кодом ГЛОНАСС
#define SWP1_MOSCOW_TIME            0x1000U     // 7: 1 - Московское зимнее время, 0 - UTC
#define SWP1_GPS                    0x2000U     // 6: Работа по GPS
#define SWP1_GLONASS                0x4000U     // 5: Работа по ГЛОНАСС
#define SWP1_TEST                   0x8000U     // 5: 1 - контроль, 0 - работа

//=============================================================================
// Слово Состояния 2 ППА-С/В, SW2_PPA
//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//efine SWP2_                       0x0001U     // 19:
#define SWP2_DIFS_ABS               0x0002U     // 18: Отсутствуют измерения дифпоправок
#define SWP2_FLT_GPS_GLO            0x0004U     // 17: отказ GPS/ГЛОНАСС
#define SWP2_FEW_SATS               0x0008U     // 16: Недостаточно видимых спутников
#define SWP2_GPS_ALM_ABS            0x0010U     // 15: Отсутствует альманах GPS
#define SWP2_GLO_ALM_ABS            0x0020U     // 14: Отсутствует альманах ГЛОНАСС
#define SWP2_INIT_DATA_ABS          0x0040U     // 13: Отсутствуют начальные данные от БЦВМ
#define SWP2_FAULT                  0x0080U     // 12: Отказ изделия в целом
#define SWP2_FLT_PV                 0x0100U     // 11: Отказ блока ПВ-С/В-01
#define SWP2_FLT_AAP                0x0200U     // 10: Отказ блока ААР-8Э-02
#define SWP2_DOP_NSV_VALID          0x0400U     // 9:  Достоверность геометрических факторов и числа спутников
#define SWP2_DATE_VALID             0x0800U     // 8:  Достоверность даты
#define SWP2_TIME_VALID             0x1000U     // 7:  Достоверность времени
#define SWP2_ALT_VALID              0x2000U     // 6:  Достоверность высоты
#define SWP2_POS_VALID              0x4000U     // 5:  Достоверность координат
#define SWP2_VEL_VALID              0x8000U     // 4:  Достоверность составляющих скорости

//---------------------------------------------------------------------------
#define SCALE_ANGLE        d2r(180.)            // Угловые параметры [радиан]
#define SCALE_ANGLE_F      d2r(180.F)           // Угловые параметры [радиан]
#define SCALE_LAT          d2r( 90.F)           // Широта [радиан]

#define SCALE_H             32768.F             // Высота [м]

#define SCALE_V             1685.73F            // Скорость [м/с]

#define SCALE_Ab             128.F              // Ускорение в связанных осях [м/с^2]
#define SCALE_Wb          d2r(512.F)            // Угловая скорость в связанных осях

#define SCALE_RX           102.4F               // Расстояния до антенны [м]

#define SCALE_T_SYS       5368709.12            // Время с LSB.lx = 0.0025c (системное)
#define SCALE_T_SYS_RTM     20971.52            // Время системное, предаваемое в РТМ-2 [с] (LSB = 0.01 с), беззнаковое
#define SCALE_T_SNS_RTM     209715.2            // время СНС, , предаваемое в РТМ-2 [с] (LSB = 0.1 с)  беззнаковое
#define SCALE_TF            4.194304            // Время формирования данных

#define SCALE_DOP           3276.8F             // Геометрические факторы (HDOP, VDOP)
#define SCALE_TSEC             1.F              // Дробная часть секунды

#define SCALE_T_UQ          2.048F              // Длительность Очереди-1 Гц   LSB.ucx =  8 мс
#define SCALE_T_SQ        SCALE_T_UQ            // Длительность Очереди-10 Гц
#define SCALE_T_FQ          0.0256F             // Длительность Очереди-100 Гц LSB.ucx =  0.1 мс
#define SCALE_T_RQ        SCALE_T_FQ            // Длительность F_CYCLE
#define SCALE_T_RTC       0.00512F              // Измерение длительности интервала между Циклами-400 LSB.ucx=0.02 мс

#define LSB_MKI_TIMER        0.1e-6             // LSB внутреннего таймера МКИ (F = 10MHz) [только для измер. длительности Цикла]

#define SCALE_dP            32768.F             // Оценка ошибки местоположения.
#define SCALE_dH            3276.8F             // Оценка ошибки высоты.

#define SCALE_SNR            256.F              // Отношение сигнал/шум [дБ*Гц]
#define SCALE_EL            d2r(90.F)           // Угол возвышения спутника [радиан]
#define SCALE_GNSS_AZM      d2r(360.F)          // Азимут спутника [радиан]

#define SCALE_GNSS_TLAG     1.024F              // Время запаздывания решения СНС [с]

// Константы, связывающие масштаб и цену младшего разряда
#define RANGE_CX      128.F                 //   2^7   cfix
#define RANGE_CU      256.F                 //   2^8  ucfix

#define RANGE_HX    32768.F                 //   2^15  hfix
#define RANGE_HU    65536.F                 //   2^16 uhfix

#define RANGE_LX    2147483648.             //   2^31  lfix
#define RANGE_LU    4294967296.             //   2^32 ulfix

//----------------------------------------------------------------------------
#define MIN_INT8    (-128.F)                // -2^7      = -128
#define MAX_INT8      127.F                 //  2^7-1    =  127

#define MIN_INT16   (-32768.F)              // -2^15     = -32768
#define MAX_INT16     32767.F               //  2^15 - 1 =  32767

#define MIN_INT32   (-2147483648.)          // -2^31     = -2147483648
#define MAX_INT32     2147483647.           //  2^31 - 1 =  2147483647

#define MAX_UINT8    255U                   //  2^8  - 1 =  255
#define MAX_UINT16   65535U                 //  2^16 - 1 =  65535
#define MAX_UINT32   4294967295U            //  2^32 - 1 =  4294967295


struct  Packet_05               // Данные, принимаемые от ППА-С/В05СВ [ПП=05]
{
    word        SW1_PPA;        // Слово состояния 1 ППА-С/В
    word        SW2_PPA;        // Слово состояния 2 ППА-С/В
    lfix_mpx    Vn_S;           // Северная составляющая скорости                                           SCALE_V
    lfix_mpx    Ve_S;           // Восточная составляющая скорости                                          SCALE_V
    lfix_mpx    Vu_S;           // Вертикальная составляющая скорости                                       SCALE_V
    lfix_mpx    Lat_S;          // Широта                                                                   SCALE_ANGLE
    lfix_mpx    Lon_S;          // Долгота                                                                  SCALE_ANGLE
    hfix        Alt_S;          // Абсолютная барометрическая высота                                        SCALE_H
    hfix        Tupd;           // Время обновления относительно метки времени 1с                           SCALE_T_UPD
    word        Time;           // Время (часы, минуты)
    uint16      Sec;            // Секунды
    word        Date;           // Дата
    hfix        DOP;            // Геометрический фактор                                                    SCALE_DOP
    word        NSat;           // Число спутников
    hfix        GDOP;           // Ошибка координат GDOP                                                    SCALE_GDOP
    hfix        HDOP;           // Горизонтальный геометрический фактор                                     SCALE_HDOP
    hfix        VDOP;           // Вертикальный  геометрический фактор                                      SCALE_HDOP
    hfix        WHdg;           // Путевой угол                                                             SCALE_ANGLE
    hfix        VFOM;           // СКО вертикальных координат                                               SCALE_FOM
    hfix        HFOM;           // СКО горизонтальных координат                                             SCALE_FOM
    word        DateSW;         // Дата изменения ПМО
    word        VerSW;          // Версия ПМО
    word        SatInfo;        // Информация о спутнике
    word        ChInfo;         // Информация о канале
    word        AzmEl;          // Азимут и возвышение спутника
}; // 60 байт

struct tcp_addr{
    size_t port{42000};
    std::string ip_address{"0.0.0.0"};
};

class udp_server{
public:

    udp_server(tcp_addr client):client{client}
    {}

    bool init() {
        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            return false;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        uint32_t ip;

        if(inet_pton(AF_INET, client.ip_address.c_str(), &ip) == 0) {
            return false;
        }

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(server.port);


        // Filling client information
        cliaddr.sin_family = AF_INET; // IPv4
        cliaddr.sin_addr.s_addr = ip;
        cliaddr.sin_port = htons(client.port);

        // Bind the socket with the server address
        if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
        {
            perror("bind failed");
            return false;
        }

        return true;
    }


    bool send_data(uint8_t* buffer, size_t size) {

        std::lock_guard<std::mutex> lock(mu);

        socklen_t len;
        len = sizeof(cliaddr);  //len is value/result
        if(sendto(sockfd, buffer, size, MSG_CONFIRM,
                  (const struct sockaddr *) &cliaddr,
                  len) < 0)
            return false;

    }


    tcp_addr server;
    tcp_addr client;

    std::mutex mu;

    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
};

#define  YEAR0  2000    // Нулевым годом будем считать 2000-й

class udp_packet{

    Packet_05 packet;

    //-----------------------------------------------------------------------------
    // Формирование слова "Time"
    word   packTime(int Hour, int Min, int Sec)
    {
        return  static_cast<word>((Hour << (6 + 5)) + (Min << 5) + Sec / 2);
    }

    // Формирование слова "Date"
    word   packDate(int Day, int Month, int Year)
    {
        Year -= YEAR0;
        return  static_cast<word>((Year << 9)  + (Month << 5) + Day);
    }


    //============================================================================
    // float <--> lfix
    //============================================================================
    lfix  f2lx(double Value, double Scale)
    {

        Value = Value*RANGE_LX/Scale;

        if (Value < 0.)
        {
            Value -= 0.5;
            if (Value <= MIN_INT32)  Value = MIN_INT32;
        }
        else
        {
            Value += 0.5;
            if (Value >= MAX_INT32)  Value = MAX_INT32;
        }

        return static_cast<lfix>(static_cast<lfix>(Value) & 0xFFFFFFFFU);
    }


    //============================================================================
    // float <--> hfix
    //============================================================================
    hfix  f2hx(float  Value, float  Scale, int Bits)
    {
        Value = Value*RANGE_HX/Scale;

        if (Value < 0.F)
        {
            Value -= HHalfBit[Bits];
            if (Value <= MIN_INT16)  Value = MIN_INT16;
        }
        else
        {
            Value += HHalfBit[Bits];
            if (Value >= MAX_INT16)  Value = MAX_INT16;
        }

        return static_cast<hfix>(static_cast<hfix>(Value) & HBitMask[Bits]);
    }



    void makePack05(GNSS_Sol& gnssSol)
    {
        packet.SW1_PPA = 0;
        packet.SW2_PPA = 0;
        if (gnssSol.NSat[0] != 0)
            SETM(packet.SW1_PPA, SWP1_GPS);
        if (gnssSol.NSat[1] != 0)
            SETM(packet.SW1_PPA, SWP1_GLONASS);
        if (gnssSol.NSat[2] != 0)
            SETM(packet.SW1_PPA, SWP1_GALILEO);
        if (gnssSol.NSat[4] != 0)
            SETM(packet.SW1_PPA, SWP1_BEIDOU);

        SETC(TSTM(gnssSol.State, NS_CERT_DIFF_USED), packet.SW1_PPA, SWP1_DIF_REG_LDPS | SWP1_DIF_REG);
        SETC(TSTM(gnssSol.State, NS_CERT_SOL) == 0, packet.SW1_PPA, SWP1_SYNC);

        if (TSTM(gnssSol.State, NS_CERT_SOL))
        {
            SETM(packet.SW2_PPA, SWP2_DOP_NSV_VALID);
            SETM(packet.SW2_PPA, SWP2_DATE_VALID);
            SETM(packet.SW2_PPA, SWP2_TIME_VALID);
            SETM(packet.SW2_PPA, SWP2_POS_VALID);
            SETM(packet.SW2_PPA, SWP2_VEL_VALID);
        }

        if (TSTM(gnssSol.State, NS_CERT_3D))
            SETM(packet.SW2_PPA, SWP2_ALT_VALID);

        packet.Vn_S = f2lx(gnssSol.Vg[N], SCALE_V);
        packet.Ve_S = f2lx(gnssSol.Vg[E], SCALE_V);
        packet.Vu_S = f2lx(gnssSol.Vg[U], SCALE_V);
        packet.Lat_S = f2lx(gnssSol.Pg[B], SCALE_ANGLE);
        packet.Lon_S = f2lx(gnssSol.Pg[L], SCALE_ANGLE);
        packet.Alt_S = f2hx(static_cast<float>(gnssSol.Pg[H]), SCALE_H);

        int Hour = static_cast<int>(gnssSol.Tday / 3600.);
        int Min = static_cast<int>((gnssSol.Tday - Hour * 3600.) / 60.);
        int Sec = static_cast<int>(gnssSol.Tday - Hour * 3600. - Min * 60.);

        packet.Sec = static_cast<uint16>((Sec & 63) << 10);
        packet.Time = packTime(Hour, Min, Sec);

        packet.Date = packDate(gnssSol.Day, gnssSol.Month, gnssSol.Year);

        packet.DOP = f2hx(gnssSol.GDOP, 3276.8F);
        packet.GDOP = f2hx(gnssSol.GDOP, 3276.8F);
        packet.HDOP = f2hx(gnssSol.HDOP, 3276.8F);
        packet.VDOP = f2hx(gnssSol.VDOP, 3276.8F);

        packet.HFOM = static_cast<uint16>(gnssSol.FOM[0]);
        packet.VFOM = static_cast<uint16>(gnssSol.FOM[1]);

        packet.NSat = (gnssSol.NSat[2] & 15) | ((gnssSol.NSat[4] & 15) << 4) | ((gnssSol.NSat[0] & 15) << 8) | ((gnssSol.NSat[1] & 15) << 12);

        packet.WHdg = f2hx(gnssSol.Hdg, SCALE_ANGLE_F);

    }

};


#endif //NMEA_CONVERTER_UDP_SERVER_H
