/* ---------------------------------------------------------------- */
/* Copyright 1998 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef MY_NEW_SADFSADF_DEFINED
#define MY_NEW_SADFSADF_DEFINED
#include <map>
#include "myassert.h"
#include <iostream>

//#ifdef DEBUG
//void xxmemo_new(void*a,int b,char*file);
//#define memo_new(a) xxmemo_new(a,__LINE__,__FILE__);
//void memo_del(const void *ptr,int n=0);
//void print_map(ostream &out,int n=0);
//#else
inline void memo_new(const void *ptr,int n=0){};
inline void memo_del(const void *ptr,int n=0){};
inline void print_map(ostream &out,int n=0){};
//#endif

#endif

