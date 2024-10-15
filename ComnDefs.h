/* СomnDefs.h
***********************************************
*                                             *
*          Модуль общих определений           *
*                                             *
*            v1.0  (c) ИТТ, 2016              *
*                                             *
***********************************************
* created: 01.01.1992 [Голиков В.П.]
* revised: 22.08.2016 [Тенюшев Е.Н., Евстратенкова А.И.]
*/

#pragma once

#include "ComnTypes.h"


//============================================================================
// Мини-автоматизация
#define  FOR( i,NUM_MAX)  for ( int i=0; i<static_cast<int>(NUM_MAX); i++)
#define  FORU(i,NUM_MAX)  for (uint i=0; i<static_cast<uint>(NUM_MAX); i++)


//============================================================================
// Тип возвращаемого из функций значения
enum TripletValue
{
    FAILURE = -1,     // Дополнительное значение при ошибке, в случае необходимости (если значения 1 не хватает)
    SUCCESS = 0,      // Успешное выполнение
    ERRORS = 1        // Ошибка (м.б. код ошибки - число > 0)
};


//---------------------------------------------------------------------------
// Размер переменной в разных величинах
#define isizeof(x)  ((sizeof(x)+sizeof(int) -1)/sizeof(int))
#define wsizeof(x)  ((sizeof(x)+sizeof(word)-1)/sizeof(word))
#define lsizeof(x)  ((sizeof(x)+sizeof(long)-1)/sizeof(long))

template <typename T, typename TX>
inline unsigned tsizeof(TX x) { return (sizeof(x) + sizeof(T) - 1) / sizeof(T); }


//---------------------------------------------------------------------------
// Размер массива (кол-во элементов в нём)
#define dim_of(a)   (sizeof(a)/sizeof(a[0]))


//---------------------------------------------------------------------------
#if defined(__GNUC__) || defined(_WIN32)
# define Far
# define Near
# define StdCall    __stdcall
#else
# define Far        far
# define Near       near
# define StdCall    pascal
#endif
