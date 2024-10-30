//
// Created by amir on 14.10.24.
//

#ifndef NMEA_CONVERTER_PARSER_H
#define NMEA_CONVERTER_PARSER_H


#include <cstring>
#include "uart_utils.h"
#include "indexes.h"
#include "maths.h"
#include "bit_mask.h"


// Идентификатор спутниковой системы gnssID
#define GNSS_ID_GPS                         0U  // GPS
#define GNSS_ID_SBAS                        1U  // SBAS
#define GNSS_ID_GALILEO                     2U  // Galileo
#define GNSS_ID_BEIDOU                      3U  // Beidou
#define GNSS_ID_IMES                        4U  // IMES (система навигации в помещениях)
#define GNSS_ID_QZSS                        5U  // QZSS (Квазизенитная спутниковая система, Япония)
#define GNSS_ID_GLONASS                     6U  // ГЛОНАСС

//---------------------------------------------------------------------------
// Команды выбора спутниковых систем
//---------------------------------------------------------------------------
//#define GNSS_SS                        0x2FU // Спутниковая система:
//#define GNSS_SS_GPS                    0x01U // GPS
//#define GNSS_SS_GLO                    0x02U // ГЛОНАСС
//#define GNSS_SS_GAL                    0x04U // Galileo
//#define GNSS_SS_SBAS                   0x08U // SBAS
//#define GNSS_SS_BEI                    0x10U // Beidou
//#define GNSS_SS_SBAS_DIF               0x20U // Диф. поправки SBAS

// Флаги достоверности valid
//#define UBX_VLD_DATE                     0x01U  // Достовреность даты UTC
//#define UBX_VLD_TIME                     0x02U  // Достовреность времени UTC
//#define UBX_VLD_RESOLVED                 0x04U  // Время полностью достоверно (нет секундных неразрешённостей шкал времени)
//#define UBX_VLD_MAG                      0x08U  // Достоверность магнитного склонения


// Флаги состояния решения flags
#define UBX_GNSS_FIX_OK                  0x01U  // Решение достоверно
#define UBX_DIF_SOL_USED                 0x02U  // Были использованы дифференциальные поправки
#define UBX_HEAD_VEH_VALID               0x20U  // Достоверность истинного курса (только если есть БЧЭ)
#define UBX_CARR_SOL_MASK                0xC0U  // Маска состояния решения решения с фазовыми измерениями
#define UBX_NO_CARR_SOL                  0x00U  // В решении нет фазовых измерений
#define UBX_CARR_SOL_AMB                 0x40U  // В решении есть фазовые измерения с плавающими неопределённостями
#define UBX_CARR_SOL_AMB_FIXED           0x80U  // В решении есть фазовые измерения с устранёнными неопределённостями


// Флаги состояния решения flags2
#define UBX_CONFIRMED_AVAI               0x20U  // information about UTC Date and Time of Day validity confirmation is available
#define UBX_CONFIRMED_DATE               0x40U  // UTC Date validity could be confirmed
#define UBX_CONFIRMED_TIME               0x80U  // UTC Time of Day could be confirmed

// Флаги состояния решения flags3
//#define UBX_INVALID_LLH                  0x01U  // Координаты (lon, lat, height and hMSL) не достоверны
//#define UBX_DIF_AGE_COR_MASK             0x1EU  // Возраст дифф. поправок
//#define UBX_DIF_AGE_NOT_AVAIL            0x00U  // Нет данных
//#define UBX_DIF_AGE_1S                   0x02U  // 1 с
//#define UBX_DIF_AGE_2S                   0x04U  // 2 с
//#define UBX_DIF_AGE_5S                   0x06U  // 5 с
//#define UBX_DIF_AGE_10S                  0x08U  // 10 с
//#define UBX_DIF_AGE_15S                  0x0AU  // 15 с
//#define UBX_DIF_AGE_20S                  0x0CU  // 20 с
//#define UBX_DIF_AGE_30S                  0x0EU  // 30 с
//#define UBX_DIF_AGE_45S                  0x10U  // 45 с
//#define UBX_DIF_AGE_60S                  0x12U  // 60 с
//#define UBX_DIF_AGE_90S                  0x14U  // 90 с
//#define UBX_DIF_AGE_120S                 0x16U  // 120 с
//#define UBX_DIF_AGE_G120S                0x18U  // >120 с

// Поле flags
#define UBX_SAT_QUALITY_IND_MASK   0x00000007U  // Маска индикатора качества сигнала
#define UBX_SAT_NO_SIG             0x00000000U  // Нет сигнала
#define UBX_SAT_SEARCHING          0x00000001U  // Поиск сигнала
#define UBX_SAT_ACQUIRED           0x00000002U  // Сигнал захвачен
#define UBX_SAT_SIG_UNUSABLE       0x00000003U  // Сигнал детектирован, но не используется
#define UBX_SAT_CODE_LOCKED        0x00000004U  // Код зафиксирован и время синхронизировано
#define UBX_SAT_CODE_CAR_LOCKED_1  0x00000005U  // Код и несущая зафиксированы, время синхронизировано
#define UBX_SAT_CODE_CAR_LOCKED_2  0x00000006U  // Код и несущая зафиксированы, время синхронизировано
#define UBX_SAT_CODE_CAR_LOCKED_3  0x00000007U  // Код и несущая зафиксированы, время синхронизировано
#define UBX_SAT_USED               0x00000008U  // Спутник используется для решения навигационной задачи
#define UBX_SAT_HEALTH_MASK        0x00000030U  // Маска здоровья спутника
#define UBX_SAT_HEALTH_UNK         0x00000000U  // Состояние спутника неизвестно
#define UBX_SAT_HEALTH_OK          0x00000010U  // Спутник исправен
#define UBX_SAT_HEALTH_FLT         0x00000020U  // Спутник неисправен
#define UBX_SAT_DIF_COR            0x00000040U  // Имеются дифф поправки для спутника
#define UBX_SAT_CAR_SMOOTH         0x00000080U  // Псевдодальность сглажена фазовыми измерениями
#define UBX_SAT_ORBIT_SRC_MASK     0x00000700U  // Маска источника данных об орбите движения спутника
#define UBX_SAT_ORBIT_NO_SRC       0x00000000U  // Нет источника данных об орбитальном движении
#define UBX_SAT_ORBIT_EPH_SRC      0x00000100U  // Использованы эфемериды
#define UBX_SAT_ORBIT_ALM_SRC      0x00000200U  // Использован альманах
#define UBX_SAT_ORBIT_ANO_SRC      0x00000300U  // Использован офлайн сервис AssistNow
#define UBX_SAT_ORBIT_AOP_SRC      0x00000400U  // Использован сервис AssistNow Autonomous
#define UBX_SAT_ORBIT_SRC_1        0x00000500U  // Иной источник 1
#define UBX_SAT_ORBIT_SRC_2        0x00000600U  // Иной источник 2
#define UBX_SAT_ORBIT_SRC_3        0x00000700U  // Иной источник 3
#define UBX_SAT_EPH_AVAIL          0x00000800U  // Для спутника доступны эфемериды
#define UBX_SAT_ALM_AVAIL          0x00001000U  // Для спутника доступен альманах
#define UBX_SAT_ANO_AVAIL          0x00002000U  // Для спутника доступны дынные AssistNow Offline
#define UBX_SAT_AOP_AVAIL          0x00004000U  // Для спутника доступны дынные AssistNow Autonomous
#define UBX_SAT_SBAS_CORR_USED     0x00010000U  // Использована коррекция SBAS
#define UBX_SAT_RTCM_CORR_USED     0x00020000U  // Использована коррекция RTCM
#define UBX_SAT_SLAS_CORR_USED     0x00040000U  // Использована коррекция QZSS SLAS
#define UBX_SAT_SPARTN_CORR_USED   0x00080000U  // Использована коррекция SPARTN
#define UBX_SAT_PR_CORR_USED       0x00100000U  // Использована коррекция псевдодальности
#define UBX_SAT_CR_CORR_USED       0x00200000U  // Использованы коррекция фазовых измерений
#define UBX_SAT_DO_CORR_USED       0x00400000U  // Использованы коррекция допплеровского смещения
#define UBX_SAT_CLAS_CORR_USED     0x00800000U  // Использованы коррекция CLAS

// Достоверность решения State в структуре GNSS_Sol
#define NS_CERT_SOL                   0x01U // Получено решение (2D или 3D)
#define NS_CERT_3D                    0x02U // Получено 3D решение
#define NS_CERT_DIFF_USED             0x04U // В решении использовались дифф. поправки
#define NS_CERT_RAIM_OK               0x08U // Решение подтверждено RAIM контролем
#define NS_CERT_DIFF_ON               0x10U // Режим работы с дифференциальными поправками (разрешено принимать или формировать?)
#define NS_MOSCOW_TIME                0x20U // 1-московское зимнее время, 0-UTC
#define NS_CERT_TLAG                  0x80U // Достовреность времени запаздывания решения СНС

// Навигационное решение приёмника СНС
struct GNSS_Sol
{
    double Tsys;                            // Системное время получения данных [с]
    double Tday;                            // Время UTC от начала суток [с]
    double Pg[3];                           // Долгота, широта [радиан], высота [м]
    float  Vg[3];                           // Проекции путевой скорости: E, N, Up [м/с]
    float  Hdg;                             // Путевой угол [радиан]
    float  FOM[2];                          // Оценка точности горизонтальных координат и вертикальных координат [м]

    int    Day;                             // Число
    int    Month;                           // Месяц
    int    Year;                            // Год (ГГГГ)

    float  HDOP;                            // Горизонтальный геометрический фактор ухудшения точности
    float  VDOP;                            // Вертикальный геометрический фактор ухудшения точности
    float  TDOP;                            // Снижение точности по времени
    float  PDOP;                            // Снижение точности по местоположению
    float  GDOP;                            // Суммарное геометрическое снижение точности по местоположению и времени

    uint8_t  NSat[5];                         // Количество спутников в решении (GPS, ГЛОНАСС, Galileo, SBAS, Beidou)

    bool   HaveNew;                         // Есть обновлённые данные
    bool   CertV;                           // Есть достоверные и проверенные на целостность измерения высоты

    uint8_t  State;                           // Достоверность решения

    uint16_t   ChkStatus;                       // Состояние контроля целостности решения

    uint16_t   Reserv;

    float  Tlag;                            // Запаздывание текущего решения
};


struct SatInfo                              // Канальная информация о видимых спутниках приёмника СНС
{
    double TimeSys;                         // Системное время обновления данных [с]
    int8_t   Visible;                         // 1 - Ведётся слежение за спутником в приёмных каналах

    int8_t   Status;                          // 0 - данные спутника в решении не используются
    // 1 - спутник используется в решении
    // 2 - спутник исключён из решения алгоритмом RAIM

    int8_t   Type;                            // Тип СНС: 0-GPS, 1-ГЛОНАСС, 2-SBAS, 3-Beidou, 4-Galileo
    int8_t   Litera;                          // -7..13 - номер литеры для ГЛОНАСС, 127 для остальных спутников
    int16_t  PRN;                             // Номер спутника
    int16_t  Reserv[3];
    float  SNR;                             // Отношение сигнал/шум
    float  Azm;                             // Азимут спутника 0..2Pi [радиан]
    float  El;                              // Возвышение спутника [радиан]
};


struct UBX_NAV_PVT              // Навигационное решение
{
    uint32_t      iTow;           // Время GPS от начала недели [мс]
    uint16_t      year;           // Год (UTC)
    uint8_t       month;          // Месяц   (UTC) 1..12
    uint8_t       day;            // День    (UTC) 1..31
    uint8_t       hour;           // Час     (UTC) 0..23
    uint8_t       min;            // Минуты  (UTC) 0..59
    uint8_t       sec;            // Секунды (UTC) 0..60
    uint8_t       valid;          // Флаги достоверности
    uint32_t      tAcc;           // Оценка точности времени [нс]
    int32_t       nano;           // Дробная часть секунды -1e9..1e9 [нс]
    uint8_t       fixType;        // Тип решения
    uint8_t       flags;          // Флаги состояния решения
    uint8_t       flags2;         // Дополнительные флаги состояния решения 2
    uint8_t       numSv;          // Количество спутников использованных в навигационном решении
    int32_t       lon;            // Долгота, ЦМР=1e-7 градус
    int32_t       lat;            // Широта,  ЦМР=1e-7 градус
    int32_t       height;         // Высота над эллипсоидом  [мм]
    int32_t       hMSL;           // Высота над уровнем моря [мм]
    uint32_t      hAcc;           // Оценка точности горизонтальных координат [мм]
    uint32_t      vAcc;           // Оценка точности высоты  [мм]
    int32_t       velN;           // Северная  составляющая скорости [мм/с]
    int32_t       velE;           // восточная составляющая скорости [мм/с]
    int32_t       velD;           // Вертикальная составляющая скорости, направлена вниз [мм/с]
    int32_t       gSpeed;         // Горизонтальная составляющая скорости [мм/с]
    int32_t       headMot;        // Путевой угол, ЦМР=1e-5 градус
    uint32_t      sAcc;           // Оценка точности скорости [мм/с]
    uint32_t      headAcc;        // Оценка точности курса [градус]
    uint16_t      pDOP;           // PDOP, ЦМР=0.01
    uint16_t      flags3;         // Дополнительные флаги состояния решения 3
    uint8_t       reserved1[4];   // Резерв
    int32_t       headVeh;        // Путевой угол (ист. курс, если есть БЧЭ), ЦМР=1e-5 градус
    int16_t       magDec;         // Магнитное склонение (не реализовано в приёмнике NEO-M8P), ЦМР=1e-2 градус
    uint16_t      magAcc;         // Точность магнитного склонения (не реализовано в приёмнике NEO-M8P), ЦМР=1e-2 градус
};

struct UBX_Sat_Info             // Информация о спутнике
{
    uint8_t       gnssId;         // Идентификатор спутниковой системы
    uint8_t       svId;           // Идентификатор спутника
    uint8_t       cno;            // Отношение сигнал/шум [дБ*Гц]
    int8_t        elev;           // Угол возвышения -90..90 [градус]
    int16_t       azim;           // Азимут 0-360 [градус]
    int16_t       prRes;          // Невязка псевдодальности ЦМР = 0.1 м
    uint32_t      flags;          // Флаги
};

struct UBX_NAV_SAT              // Информация о спутниках
{
    uint16_t      Len;            // = 8 + 12*numSVs
    uint32_t      iTow;           // Время GPS от начала недели [мс]
    uint8_t       version;        // Версия сообщения = 1
    uint8_t       numSvs;         // Количество спутников
    uint8_t       reserved1[2];   // Резерв
    UBX_Sat_Info Sat[72];         // Данные о спутнике
};

#define MAX_VIS_SATS                   32   // Максимальное число видимых спутников

class ubx_parser{
public:

    GNSS_Sol gnssSol;
    ubx_receiver receiver;
    SatInfo ChanInfo[MAX_VIS_SATS];      // Информация о видимых спутниках
    int  NumSats_ChI;                 // Количество записей в структуре ChanInfo

    bool parse_NAV_PVT(const message& msg)
    {
        bool status = true;
        if(msg.size == sizeof(UBX_NAV_PVT))
        {
            UBX_NAV_PVT Pack_NAV_PVT;         // Навигационное решение
            memcpy(&Pack_NAV_PVT, msg.data, msg.size);

//            gnssSol.Tsys = TimeSys;

            int WeekDay = static_cast<int>(static_cast<double>(Pack_NAV_PVT.iTow)/86400000.);
            gnssSol.Tday   = (static_cast<double>(Pack_NAV_PVT.iTow) - static_cast<double>(WeekDay)*86400000.)/1000.;
            gnssSol.Pg[B]  = d2r(static_cast<double>(Pack_NAV_PVT.lat)*1.e-7);
            gnssSol.Pg[L]  = d2r(static_cast<double>(Pack_NAV_PVT.lon)*1.e-7);
            gnssSol.Pg[H]  = static_cast<double>(Pack_NAV_PVT.height)/1000.;
            gnssSol.Vg[N]  = static_cast<float>(Pack_NAV_PVT.velN)/1000.F;
            gnssSol.Vg[E]  = static_cast<float>(Pack_NAV_PVT.velE)/1000.F;
            gnssSol.Vg[U]  = static_cast<float>(Pack_NAV_PVT.velD)/-1000.F;
            gnssSol.Hdg    = d2r(static_cast<float>(Pack_NAV_PVT.headMot)*1.e-5F);
            gnssSol.FOM[0] = static_cast<float>(Pack_NAV_PVT.hAcc)/1000.F;
            gnssSol.FOM[1] = static_cast<float>(Pack_NAV_PVT.vAcc)/1000.F;

            for (int i = 0; i < static_cast<int>(2); i++)
            {
                if(gnssSol.FOM[i] >= 255.F)
                    gnssSol.FOM[i] = 255.F;
            }

            gnssSol.Day    = static_cast<int>(Pack_NAV_PVT.day);
            gnssSol.Month  = static_cast<int>(Pack_NAV_PVT.month);
            gnssSol.Year   = static_cast<int>(Pack_NAV_PVT.year);
            gnssSol.HaveNew = true;

            // Наличие решения
            switch(Pack_NAV_PVT.fixType)
            {
                default:
                case 0U:                            // Нет решения
                    gnssSol.State = 0U;
                    break;
                case 2U:                            // Есть 2D решение (без высоты)
                    gnssSol.State = NS_CERT_SOL;
                    break;
                case 3U:                            // Есть полное 3D решение
                    gnssSol.State = NS_CERT_SOL|NS_CERT_3D;
                    break;
            }

            // Наличие диф. режима
            SETC(TSTM(Pack_NAV_PVT.flags, UBX_DIF_SOL_USED), gnssSol.State, NS_CERT_DIFF_USED);
/*
        // Получили первое решение после метки времени?
        if (SyncSolPPS == true)
        {
            SyncSolPPS = false;
            // Время, прошедшее с момента получения PPS должно быть в разумных пределах
            double dT_PPS = TimeSys - TimePPS;
            if ((dT_PPS > 0.) && (dT_PPS < 0.1))
            {
                // Текущее время с учётом запаздывания
                double Tday = gnssSol.Tday+dT_PPS;
                if (Tday >= 86400.)
                    Tday -= 86400.;

                // Выполняем коррекцию времени
                gnssSolTimePPS.setTime(Tday);
            }
        }

        // Вычисление времени запаздывания решения СНС
        if (((TimeSys - gnssSolTimePPS.TimeCor) <= 1.2) && EQUM(gnssSol.State, NS_CERT_SOL|NS_CERT_3D))
        {
            double Tlag = gnssSolTimePPS.Tday - gnssSol.Tday;
            if (Tlag < 0.)
                Tlag += 86400.;

            gnssSol.Tlag = static_cast<float>(Tlag);

            // Время запаздывания должно быть в разумных пределах
            if ((gnssSol.Tlag > 0.F) && ((gnssSol.Tlag < 0.1)))
                SETM(gnssSol.State, NS_CERT_TLAG);
        }
*/
        }
        else
            status = false;

        return status;
    }

    // Информация о спутниках
    bool parse_NAV_SAT(const message& msg)
    {

        bool status = true;
        if(msg.size <= sizeof(UBX_NAV_SAT) - 2U)
        {
            static UBX_NAV_SAT    Pack_NAV_SAT;            // Информация о спутниках

            Pack_NAV_SAT.Len = msg.size;
            memcpy(&Pack_NAV_SAT.iTow, msg.data, msg.size);

            for (int i = 0; i < 5; ++i) {
                gnssSol.NSat[i] = 0;
            }


            for (int i = 0; Pack_NAV_SAT.numSvs; i++)
            {
                if(TSTM(Pack_NAV_SAT.Sat[i].flags, UBX_SAT_USED))
                {
                    switch(Pack_NAV_SAT.Sat[i].gnssId)
                    {
                        case GNSS_ID_GPS:
                            gnssSol.NSat[0]++;
                            break;
                        case GNSS_ID_SBAS:
                            gnssSol.NSat[3]++;
                            break;
                        case GNSS_ID_GALILEO:
                            gnssSol.NSat[2]++;
                            break;
                        case GNSS_ID_BEIDOU:
                            gnssSol.NSat[4]++;
                            break;
                        case GNSS_ID_GLONASS:
                            gnssSol.NSat[1]++;
                            break;
                    }
                }
            }

            // Обнуляем базу данных спутников
            for (int i = 0; i < MAX_VIS_SATS; i++)
            {
                ChanInfo[i].Visible = ChanInfo[i].Status = ChanInfo[i].Type = ChanInfo[i].Litera = 0;
                ChanInfo[i].PRN = 0;
                ChanInfo[i].SNR = ChanInfo[i].Azm = ChanInfo[i].El = 0.F;
            }

            NumSats_ChI = Pack_NAV_SAT.numSvs;
            if (NumSats_ChI > MAX_VIS_SATS)
                NumSats_ChI = MAX_VIS_SATS;

            for (int i = 0; i < NumSats_ChI; i++)
            {
//                ChanInfo[i].TimeSys = TimeSys;

                // Слежение за спутником (есть приём)
                if (TSTM(Pack_NAV_SAT.Sat[i].flags, UBX_SAT_ACQUIRED|UBX_SAT_SIG_UNUSABLE))
                    ChanInfo[i].Visible = 1;
                else
                    ChanInfo[i].Visible = 0;

                // Использование в решении
                if (TSTM(Pack_NAV_SAT.Sat[i].flags, UBX_SAT_USED))
                    ChanInfo[i].Status = 1;
                else
                    ChanInfo[i].Status = 0;

                // Тип спутника
                switch(Pack_NAV_SAT.Sat[i].gnssId)
                {
                    case GNSS_ID_GPS:
                        ChanInfo[i].Type = 0;
                        break;
                    case GNSS_ID_SBAS:
                        ChanInfo[i].Type = 2;
                        break;
                    case GNSS_ID_GALILEO:
                        ChanInfo[i].Type = 4;
                        break;
                    case GNSS_ID_BEIDOU:
                        ChanInfo[i].Type = 3;
                        break;
                    case GNSS_ID_GLONASS:
                        ChanInfo[i].Type = 1;
                        break;
                }

                ChanInfo[i].PRN = Pack_NAV_SAT.Sat[i].svId;
                ChanInfo[i].SNR = static_cast<float>(Pack_NAV_SAT.Sat[i].cno);
                ChanInfo[i].Azm = d2r(static_cast<float>(Pack_NAV_SAT.Sat[i].azim));
                ChanInfo[i].El  = d2r(static_cast<float>(Pack_NAV_SAT.Sat[i].elev));
            }
        }
        else
            status = false;

        return status;
    }

};





#endif //NMEA_CONVERTER_PARSER_H
