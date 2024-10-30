// BitMask.h
/**********************************************
*                                             *
*                   Модуль                    *
*          работы с битовыми масками          *
*                                             *
*             v1.0 (с) ITT, 2016              *
*                                             *
***********************************************
* created: 01.01.1992 [Голиков В.П.]
* revised: 06.04.2016 [Тенюшев Е.Н.]
*/

#pragma once

//----------------------------------------------------------------------------
// Установить 1 в битах по маске
template <typename var_t, typename mask_t>
inline void  SETM(var_t& Var, mask_t Mask)
{ Var |= Mask;}


//----------------------------------------------------------------------------
// Установить 0 в битах по маске
template <typename var_t, typename mask_t>
inline void  CLRM(var_t& Var, mask_t Mask)
{ Var &= ~Mask;}

//----------------------------------------------------------------------------
// Установить в 1 биты, отличающиеся от маски
template <typename var_t, typename mask_t>
inline void  XORM(var_t& Var, mask_t Mask)
{ Var ^= Mask;}

//----------------------------------------------------------------------------
// Проверить, установлен ли хотя бы один бит из маски
template <typename var_t, typename mask_t>
inline bool  TSTM(var_t Var, mask_t Mask)
{ return (Var & Mask) != 0;}

//----------------------------------------------------------------------------
// Проверить имеют ли значение 1 все биты, установленные в значение 1 в маске
template <typename var_t, typename mask_t>
inline bool  EQUM(var_t Var, mask_t Mask)
{ return static_cast<mask_t>(Var & Mask) == Mask;}


//----------------------------------------------------------------------------
// Установка/Стирание битовой маски по условию
template <typename cond_t, typename var_t, typename mask_t>
inline void  SETC(cond_t Cond, var_t& Var, mask_t Mask)
{ Cond? SETM(Var,Mask) : CLRM(Var,Mask);}


//----------------------------------------------------------------------------
// = CLRM(Var,Mask0), SETM(Var,Mask1)
template <typename var_t, typename mask0_t, typename mask1_t>
inline void  CHGM(var_t& Var, mask0_t Mask0, mask1_t Mask1)
{ Var = static_cast<var_t>((Var & ~Mask0) | Mask1);}


//============================================================================
// Работа с битовыми масками с "фильтром"

//--------------------------------------------------------------------------------------------------
// Устанавливать значение 1 в битах по заданной маске, не изменяя биты, установленные в 0 в фильтрующей маске
template <typename var_t, typename mask_t, typename filter_t>
inline void  SETMF(var_t& Var, mask_t Mask, filter_t Filter)
{ Var |= Mask & Filter;}


//----------------------------------------------------------------------------
// Устанавливать значение 0 в битах по заданной маске, не изменяя биты, установленные в 0 в фильтрующей маске
template <typename var_t, typename mask_t, typename filter_t>
inline void  CLRMF(var_t& Var, mask_t Mask, filter_t Filter)
{ Var &= ~(Mask & Filter);}
