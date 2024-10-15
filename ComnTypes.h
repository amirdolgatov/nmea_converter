// ComnTypes.h
/**********************************************
*                                             *
*          Модуль общих типов данных          *
*                                             *
*             v1.0 (с) ITT, 2016              *
*                                             *
***********************************************
* created: 01.01.1992 [Голиков В.П.]
* revised: 06.04.2016 [Тенюшев Е.Н., Евстратенкова А.И.]
*/

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
//          #error "Unknown GNU compiller"
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


//============================================================================
// Сокращенные названия типов данных в венгерской нотации
#define  TYPES_DEF(type) \
  typedef  const    type  c##type;  \
  typedef  volatile type  v##type;  \
  typedef           type *p##type;  \
  typedef  const    type *cp##type; \
  typedef  volatile type *vp##type;

TYPES_DEF(float)
TYPES_DEF(double)
TYPES_DEF(void)
TYPES_DEF(char)
TYPES_DEF(int)
TYPES_DEF(byte)
TYPES_DEF(word)
TYPES_DEF(dword)
TYPES_DEF(qword)
TYPES_DEF(uint8)
TYPES_DEF(uint16)
TYPES_DEF(uint32)
TYPES_DEF(uint64)
TYPES_DEF(int8)
TYPES_DEF(int16)
TYPES_DEF(int32)
TYPES_DEF(int64)
