/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef RegexMap_DEFINED
#define RegexMap_DEFINED
#include "StringMap.h"
#include <string>
#include <vector>
#include <iostream>

#include <sys/types.h>
#include <regex.h>


class RegexMap : public StringMap {
  std::vector< std::pair<regex_t,std::string> > tabelleNormal;
  std::vector< std::pair<regex_t,std::string> > tabelleInvers;

 public:
  RegexMap(){}; 
  RegexMap(const std::string& fnameNormal,const std::string& fnameInvers,int verb=0) ;

  void readFromNormal(std::istream&normal,int verb=0);
  void readFromInvers(std::istream&normal,int verb=0);

  void readFromStream(std::istream&,int);

  void applyNormal(const std::string&in,std::string&out) const;
  void applyInvers(const std::string&in,std::string&out) const;
};

#endif


