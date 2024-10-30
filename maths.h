// Maths.h
/**********************************************
*                                             *
*                   Модуль                    *
*      простейших математических функций      *
*                                             *
*             v1.0 (с) ITT, 2016              *
*                                             *
***********************************************
* created: 15.05.2001 [Голиков В.П.]
* revised: 02.12.2016 [Тенюшев Е.Н., Евстратенкова А.И.]
*/


#pragma once


#if defined (__GNUC__)
#include <math.h>
#else
#include <cmath>
#endif

const float NearlyZero = 1.e-30F;

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_F
#define M_PI_F     3.14159265358979323846F
#endif

#ifndef M_PI2
#define M_PI2      (M_PI/2.)
#endif

#ifndef M_PI4
#define M_PI4      (M_PI/4.)
#endif

#ifndef M_2PI
#define M_2PI      (2.*M_PI)
#endif

#ifndef M_SQRTPI
#define M_SQRTPI   1.77245385090551602792981
#endif

#ifndef M_E
#define M_E        2.7182818284590452354
#endif

#ifndef M_LOG2E
#define M_LOG2E    1.4426950408889634074
#endif

#ifndef M_LOG10E
#define M_LOG10E   0.43429448190325182765
#endif

#ifndef M_LN2
#define M_LN2      0.69314718055994530942
#endif

#ifndef M_LN10
#define M_LN10     2.30258509299404568402
#endif

#ifndef M_SQRT2
#define M_SQRT2    1.41421356237309504880
#endif

#ifndef M_SQRT3
#define M_SQRT3    1.73205080756887719000
#endif


//============================================================================
// Преобразование в РАДИАНЫ (double реализация)
//============================================================================
const double Degree_to_radian = M_PI/180.;

inline double d2r(double Degrees)                            // Градусы -> Радианы
{ return Degrees*Degree_to_radian; }

inline double m2r(double Minutes)                            // Минуты  -> Радианы
{ return d2r(Minutes/60.); }

inline double s2r(double Seconds)                            // Секунды -> Радианы
{ return d2r(Seconds/3600.); }

inline double dh2rs(double DegHour)                          // град/ч  -> рад/с
{ return d2r(DegHour/3600.); }

inline double dms2r(double Deg, double Min, double Sec)      // ГГ.ММ.СС-> Радианы
{ return d2r(Deg + Min/60.+ Sec/3600.); }


//============================================================================
// Преобразование в ГРАДУСЫ (double реализация)
//============================================================================
const double Radian_to_degree = 180./M_PI;

inline double r2d(double Radians)                            // Радианы -> Градусы
{ return Radians*Radian_to_degree; }

inline double r2m(double Radians)                            // Радианы -> Минуты
{ return r2d(Radians)*60.; }

inline double r2s(double Radians)                            // Радианы -> Секунды
{ return r2d(Radians)*3600.; }

inline double rs2dh(double RadSec)                           // Радианы -> Секунды
{ return r2d(RadSec)*3600.; }

inline double dms2d(double Deg, double Min, double Sec)      // ГГ.ММ.СС-> Градусы
{ return Deg + Min/60.+ Sec/3600.; }


//============================================================================
// Преобразование в РАДИАНЫ (float реализация)
//============================================================================
const float  Degree_to_radian_f = M_PI_F/180.F;

inline float d2r(float Degrees)                              // Градусы -> Радианы
{ return Degrees*Degree_to_radian_f; }

inline float m2r(float Minutes)                              // Минуты  -> Радианы
{ return d2r(Minutes/60.F); }

inline float s2r(float Seconds)                              // Секунды -> Радианы
{ return d2r(Seconds/3600.F); }

inline float dh2rs(float DegHour)                            // град/ч  -> рад/с
{ return d2r(DegHour/3600.F); }

inline float dms2r(float Deg, float Min, float Sec)          // ГГ.ММ.СС-> Радианы
{ return d2r(Deg + Min/60.F+ Sec/3600.F); }


//============================================================================
// Преобразование в ГРАДУСЫ (float реализация)
//============================================================================
const float Radian_to_degree_f = 180.F/M_PI_F;

inline float r2d(float Radians)                              // Радианы -> Градусы
{ return Radians*Radian_to_degree_f; }

inline float r2m(float Radians)                              // Радианы -> Минуты
{ return r2d(Radians)*60.F; }

inline float r2s(float Radians)                              // Радианы -> Секунды
{ return r2d(Radians)*3600.F; }

inline float rs2dh(float RadSec)                             // рад/с   -> град/ч
{ return r2d(RadSec)*3600.F; }

inline float dms2d(float Deg, float Min, float Sec)          // ГГ.ММ.СС-> Градусы
{ return Deg + Min/60.F + Sec/3600.F; }


//============================================================================
// Преобразование м/с^2 <-> мкg
//============================================================================
inline float ms2ug(float Val)                                // м/с^2 -> мкg
{
    return Val*1000000.F/9.81F;
}

inline double ms2ug(double Val)                              // м/с^2 -> мкg
{
    return Val*1000000./9.81;
}

inline float ug2ms2(float Val)                               // мкg -> м/с^2
{
    return Val/1000000.F*9.81F;
}

inline double ug2ms2(double Val)                             // мкg -> м/с^2
{
    return Val/1000000.*9.81;
}


//============================================================================
// Преобразование времени в секунды
//============================================================================
template <typename T>
inline T hms2s(T Hour, T Min, T Sec)
{ return Hour*3600 + Min*60 + Sec; }


//============================================================================
// Определение знака числа
//============================================================================

//----------------------------------------------------------------------------
//2 положения: "+" или "-", ноль считается положительным
template <typename T>
inline int sign2(T Value)
{
    return (Value >= 0)? 1 : -1;
}


//----------------------------------------------------------------------------
//3 положения: "+", "-" и "0"
template <typename T>
inline int sign3(T Value)
{
    int Res;

    if      (Value > 0) Res = 1;
    else if (Value < 0) Res =-1;
    else                Res = 0;

    return Res;
}


//============================================================================
// Приведение углов в диапазон (double реализация)
//============================================================================

//----------------------------------------------------------------------------
// Приведение углов в диапазон [0, 2*Pi)
inline double to2Pi(double Angle)
{
    int Jumps;

    if(Angle >= 0.)
        Jumps = static_cast<int>(Angle/M_2PI + 0.5);
    else
        Jumps = static_cast<int>(Angle/M_2PI - 0.5);

    Angle -= static_cast<double>(Jumps)*M_2PI;

    if (Angle < 0.)          Angle += M_2PI;
    else if (Angle >= M_2PI) Angle -= M_2PI;
    else {}                                                  // Здесь ничего делать не нужно

    return Angle;
}


//----------------------------------------------------------------------------
// Приведение углов в диапазон (-Pi, Pi]
inline double toPi(double Angle)
{
    int Jumps;

    if(Angle >= 0.)
        Jumps = static_cast<int>(Angle/M_2PI + 0.5);
    else
        Jumps = static_cast<int>(Angle/M_2PI - 0.5);

    Angle -= static_cast<double>(Jumps)*M_2PI;

    if (Angle > M_PI)        Angle -= M_2PI;
    else if (Angle <= -M_PI) Angle += M_2PI;
    else {}                                                  // Здесь ничего делать не нужно

    return Angle;
}


//============================================================================
// Приведение углов в диапазон (float реализация)
//============================================================================

//----------------------------------------------------------------------------
// Приведение углов в диапазон [0, 2*Pi)
inline float to2Pi(float Angle)
{
    int Jumps;
    float m_2pi = static_cast<float>M_2PI;

    if(Angle >= 0.F)
        Jumps = static_cast<int>(Angle/m_2pi + 0.5F);
    else
        Jumps = static_cast<int>(Angle/m_2pi - 0.5F);

    Angle -= static_cast<float>(Jumps)*m_2pi;

    if (Angle < 0.F)         Angle += m_2pi;
    else if (Angle >= m_2pi) Angle -= m_2pi;
    else {}                                                  // Здесь ничего делать не нужно

    return Angle;
}


//----------------------------------------------------------------------------
// Приведение углов в диапазон (-Pi, Pi]
inline float toPi(float Angle)
{
    int Jumps;
    float m_2pi = static_cast<float>M_2PI;

    if(Angle >= 0.F)
        Jumps = static_cast<int>(Angle/m_2pi + 0.5F);
    else
        Jumps = static_cast<int>(Angle/m_2pi - 0.5F);

    Angle -= static_cast<float>(Jumps)*m_2pi;

    if (Angle > m_2pi)        Angle -= m_2pi;
    else if (Angle <= -m_2pi) Angle += m_2pi;
    else {}                                                  // Здесь ничего делать не нужно

    return Angle;
}


//============================================================================
// Возведение числа в квадрат
template <typename T>
inline T sqr(T Value) { return Value*Value; }


//============================================================================
// Нахождение большего числа
template <typename T>
T max_t(T a, T b)
{
    return (a > b) ? a : b;
}


//----------------------------------------------------------------------------
// Нахождение меньшего числа
template <typename T>
T min_t(T a, T b)
{
    return (a < b) ? a : b;
}


//---------------------------------------------------------------------------
// Вычисление полинома произвольной степени:
// P(Arg) = P0 + P1*Arg + P2*Arg^2 + P3*Arg^3 + ...
// Polinome[Order+1] = {P0, P1,…, POrder+1}
template <typename T1, typename T2>
double polynome(T1 *Polynome, T2 Arg, int Order)
{
    T2 TArg = 1.F;
    double Res  = 0.;

    for (int i = 0; i<(Order + 1); i++)
    {
        Res  += Polynome[i]*TArg;
        TArg *= Arg;
    }

    return Res;
}


//---------------------------------------------------------------------------
// Ограничение величины Val нелинейностью вида "насыщение"
template <typename T1, typename T2, typename T3>
T1   saturation(T1 Val, T2 Min, T3 Max)
{
    if(Val < Min)
        Val =  static_cast<T1>(Min);
    else if (Val > Max)
        Val =  static_cast<T1>(Max);
    else {}                                                  // Здесь ничего делать не нужно

    return Val;
}


//---------------------------------------------------------------------------
// Оценка математического ожидания, дисперсии и СКО
template<typename T>
class EstStat
{
public:
    EstStat(void) : Mean(0.F), Dispers(0.F), Residual(0.F), Counts(0), Reserv(0) {}
    T      Mean;                        // Мат. ожидание
    T      Dispers;                     // Дисперсия
    T      Residual;                    // Среднеквадратическое отклонение

    void   put(T Reading);              // Добавить замер
    int    numOf(void)  {return Counts;}
    void   reset(void)  {Counts = 0;}   // Начать оценку заново

private:
    int    Counts;                      // Число обработанных измерений
    int    Reserv;
};


//----------------------------------------------------------------------------
template<typename T>
void EstStat<T>::put(T Reading)
{
    if(Counts == 0)
    {
        Mean = Reading;
        Dispers = Residual = 0.F;
    }
    Counts++;
    Mean     = (static_cast<T>(Counts)*Mean + Reading)/static_cast<T>(Counts + 1);
    Dispers  = (1.F - 1.F/static_cast<T>(Counts))*Dispers + sqr(Reading - Mean)/static_cast<T>(Counts+1);
    Residual = sqrtf(static_cast<float>(Dispers));    
}
