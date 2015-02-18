/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef xxx_Tokenizer_h_defined
#define xxx_Tokenizer_h_defined
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "StringMap.h"
#include "RegexMap.h"

class Tokenizer : public StringMap {
  std::vector<std::vector<std::string> > exceptions;
  std::vector<std::vector<int> > wordChars;
  std::vector<std::vector<int> > wordChars2;
  std::vector<std::vector<int> > wordChars3;
  std::vector<std::string> wordChars2x;
  std::vector<std::string> wordChars3x;
  RegexMap ThePostTokMap;
  RegexMap ThePreTokMap;
  std::string inputCode;
  std::string outputCode;
 public:
  Tokenizer();
  Tokenizer(const std::string& fname,int verb=0);
  
  void readFromStream(std::istream&normal,int verb=0);

  void applyNormal(const std::string&in,std::string&out) const;
};

#endif
