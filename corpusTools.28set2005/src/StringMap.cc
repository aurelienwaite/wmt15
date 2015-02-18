/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include "StringMap.h"
//#include "ATutils.hh"
#include "IOfile.hh"

void StringMap::readFromFile(const std::string& fname, int verb) {
//  if(util::existsFile(fname)) {
    Ifile in(fname);
    readFromStream(in,verb);
//  }
}
