/* PckDrv.h
****************************************
*                                      *
*               Простейший             *
*            пакетный драйвер          *
*          v1.0   (c) ИТТ, 2019        *
*                                      *
****************************************
* created: 13.07.2019, суббота [Тенюшев Е.Н.]
* revised: 14.12.2021, вторник, [Тенюшев Е.Н.]
*/

#include <string.h>
#include "ComnDefs.h"
#include "PackDrv.h"


//----------------------------------------------------------------------------
PacketDriver::PacketDriver(void) :
    CalcPckBodyCRC(true),
    Step(0U),
    NumError(0U),
    LenRcvd(0U)
{Reserv[0]=Reserv[1]=Reserv[2]=0; reset();}


//----------------------------------------------------------------------------
// Начать разбор сначала
void PacketDriver::reset(void)
{
    Step = NumError = 0U;
    LenRcvd = 0U;
}


//----------------------------------------------------------------------------
// Побайтная обработка потока в линии для выделения пакетов
int  PacketDriver::parse(byte Data, SimplePacket *Pack)
{
    int Status = FAILURE;

    switch (Step)
    {
    case 0U:                                // Ищем начало - заголовок пакета
        if (Data == PACK_HDR)               // Поймали!
        {
            Step = 1;
            Pack->Crc = Data;
            Pack->Len = 0U;
            LenRcvd = 0U;
        }
        break;

    case 1U:                                // Принимаем идентификатор пакета
        Pack->Id = Data;
        Pack->Crc += Data;
        Step = 2U;
        break;

    case 2U:                                // Принимаем длину пакета
        Pack->Len = Data;
        Pack->Crc += Data;
        if (Pack->Len == 0U)
            Step = 4U;                      // Пакет без тела, следующий байт CRC
        else
            Step = 3U;                      // Разбираем тело пакета
        break;

    case 3U:                                // Разбор тела пакета
        Pack->Data[LenRcvd++] = Data;
        Pack->Crc += Data;
        if (Pack->Len == LenRcvd)           // Следующий байт будет КС
        {
            if (CalcPckBodyCRC)
                Step = 4U;                  // Проверка КС включена
            else
                Step = 5U;                  // Проверка КС выключена
        }
        break;

    case 4U:                                // Принимаем и проверяем контрольную сумму
        if (Pack->Crc == Data)
            Status = SUCCESS;               // Пакет успешно принят
        else                                // Не сошлась контрольная сумма
        {
            Status = ERRORS;                // Либо ошибка в линии при передаче данных, либо ошибка синхронизации (поиска начала пакета)
            NumError++;
        }
        Step = 0U;
        break;

    case 5U:                                // Контрольную сумму не проверяем
        Status = SUCCESS;                   // Пакет успешно принят
        Step = 0U;
        break;
    }

    return Status;
}


//----------------------------------------------------------------------------
// Сформировать пакет для отправки в линию
// (возвращает фактический размер данных, передаваемых в линию)
uint16  PacketDriver::form(byte *Buf, const SimplePacket *Pack)
{
    byte *pB = Buf;
    uint8 Crc = PACK_HDR;

    // Формирования заголовка пакета
    *pB = PACK_HDR;  pB++;
    *pB = Pack->Id;  Crc += *pB; pB++;
    *pB = Pack->Len; Crc += *pB; pB++;

    memcpy(pB, Pack->pData, Pack->Len);
    pB += Pack->Len;

    if(CalcPckBodyCRC)
    {
        // Расчёт контрольной суммы
        const byte *pD = static_cast<const byte *>(Pack->pData);
        for(register int i = 0; i < Pack->Len; i++)
        {
            Crc += *pD;
            pD++;
        }
    }

    // Формирование арифметической контрольной суммы пакета
    *pB = Crc;

    return static_cast<uint16>(Pack->Len + 4U);
}
