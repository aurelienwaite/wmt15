/* ---------------------------------------------------------------- */
/* Copyright 1998 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef HEADER_MYMATH_DEFINED
#define HEADER_MYMATH_DEFINED

#include <cmath>

extern "C"
{
/*   double log(double); */
/*   double exp(double); */
/*   double pow(double,double); */

  //double fabs(double);
};
inline double mfabs(double x){return (x<0)?(-x):x;}
//#include <cmath>
#endif
