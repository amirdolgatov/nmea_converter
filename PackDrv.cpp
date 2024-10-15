/* PckDrv.h
****************************************
*                                      *
*               ����������             *
*            �������� �������          *
*          v1.0   (c) ���, 2019        *
*                                      *
****************************************
* created: 13.07.2019, ������� [������� �.�.]
* revised: 14.12.2021, �������, [������� �.�.]
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
// ������ ������ �������
void PacketDriver::reset(void)
{
    Step = NumError = 0U;
    LenRcvd = 0U;
}


//----------------------------------------------------------------------------
// ��������� ��������� ������ � ����� ��� ��������� �������
int  PacketDriver::parse(byte Data, SimplePacket *Pack)
{
    int Status = FAILURE;

    switch (Step)
    {
    case 0U:                                // ���� ������ - ��������� ������
        if (Data == PACK_HDR)               // �������!
        {
            Step = 1;
            Pack->Crc = Data;
            Pack->Len = 0U;
            LenRcvd = 0U;
        }
        break;

    case 1U:                                // ��������� ������������� ������
        Pack->Id = Data;
        Pack->Crc += Data;
        Step = 2U;
        break;

    case 2U:                                // ��������� ����� ������
        Pack->Len = Data;
        Pack->Crc += Data;
        if (Pack->Len == 0U)
            Step = 4U;                      // ����� ��� ����, ��������� ���� CRC
        else
            Step = 3U;                      // ��������� ���� ������
        break;

    case 3U:                                // ������ ���� ������
        Pack->Data[LenRcvd++] = Data;
        Pack->Crc += Data;
        if (Pack->Len == LenRcvd)           // ��������� ���� ����� ��
        {
            if (CalcPckBodyCRC)
                Step = 4U;                  // �������� �� ��������
            else
                Step = 5U;                  // �������� �� ���������
        }
        break;

    case 4U:                                // ��������� � ��������� ����������� �����
        if (Pack->Crc == Data)
            Status = SUCCESS;               // ����� ������� ������
        else                                // �� ������� ����������� �����
        {
            Status = ERRORS;                // ���� ������ � ����� ��� �������� ������, ���� ������ ������������� (������ ������ ������)
            NumError++;
        }
        Step = 0U;
        break;

    case 5U:                                // ����������� ����� �� ���������
        Status = SUCCESS;                   // ����� ������� ������
        Step = 0U;
        break;
    }

    return Status;
}


//----------------------------------------------------------------------------
// ������������ ����� ��� �������� � �����
// (���������� ����������� ������ ������, ������������ � �����)
uint16  PacketDriver::form(byte *Buf, const SimplePacket *Pack)
{
    byte *pB = Buf;
    uint8 Crc = PACK_HDR;

    // ������������ ��������� ������
    *pB = PACK_HDR;  pB++;
    *pB = Pack->Id;  Crc += *pB; pB++;
    *pB = Pack->Len; Crc += *pB; pB++;

    memcpy(pB, Pack->pData, Pack->Len);
    pB += Pack->Len;

    if(CalcPckBodyCRC)
    {
        // ������ ����������� �����
        const byte *pD = static_cast<const byte *>(Pack->pData);
        for(register int i = 0; i < Pack->Len; i++)
        {
            Crc += *pD;
            pD++;
        }
    }

    // ������������ �������������� ����������� ����� ������
    *pB = Crc;

    return static_cast<uint16>(Pack->Len + 4U);
}
