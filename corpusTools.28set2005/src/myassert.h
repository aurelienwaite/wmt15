/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef MY_ASSERT_DEFINED
#define MY_ASSERT_DEFINED
void myerror(int line,const char *file,const char *expression);
void imyerror(int line,const char *file,const char *expression);

#define iassert(expression) do {if (!(expression)) {imyerror(__LINE__,__FILE__,#expression);}} while (0)

#ifdef DEBUG
#undef massert
#define massert(expression)  do {if (!(expression)) {myerror(__LINE__,__FILE__,#expression);}} while (0)
#else
#
#define massert(expr) do {} while(0)
#endif

#ifdef VDEBUG
#define vassert(expression)  do {if (!(expression)) {myerror(__LINE__,__FILE__,#expression);}} while (0)
#else
#define vassert(expr) do {} while(0)
#endif

#include <assert.h>

#endif





