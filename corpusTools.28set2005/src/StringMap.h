/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef StringMap_DEFINED
#define StringMap_DEFINED
#include <string>

class StringMap {
 public:
  StringMap(){}
  virtual ~StringMap() {};

  virtual void applyNormal(const std::string& in,std::string& out) const {out=in;};
  virtual void applyInvers(const std::string& in,std::string& out) const {out=in;};

  virtual void readFromFile(const std::string&,int v);
  virtual void readFromStream(std::istream&,int) {};
};
#endif
