/* ---------------------------------------------------------------- */
/* Copyright 1998 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef HEADER_my_DEFINED
#define HEADER_my_DEFINED
#include "my_new.h"

#define over_array(a,i) for(i=(a).low();i<=(a).high();i++)
#define backwards_array(a,i) for(i=(a).high();i>=(a).low();i--)
#define over_arr(a,i) for(int i=(a).low();i<=(a).high();i++)
#define over_arrMAX(a,i,max) for(int i=(a).low();i<=min((a).high(),max-1);i++)
#define backwards_arr(a,i) for(int i=(a).high();i>=(a).low();i--)

extern double n1mult,n2mult,n3mult;

inline double realProb(int n1,int n2)
{
  massert(n1<=n2);
  iassert(n1>=0&&n2>0);
  if(n2==0)n2=1;
  return ((double)n1)/(double)n2;
}

inline double verfProb(int n1,int n2)
{
  double prob = realProb(n1,n2);
  if( n1==1 )return prob*n1mult;
  else if( n1==2 )return prob*n2mult;
  else if( n1==3 )return prob*n3mult;
  else return prob;
}

#endif
