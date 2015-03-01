/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef SPLITPARAGRAPH_HH_
#define SPLITPARAGRAPH_HH_
#include <string>
#include "StringMap.h"
class RegexMap;

class ParagraphSplitter : public StringMap {
  RegexMap* m;
  int aimAt;
public:
  ParagraphSplitter();
  ParagraphSplitter(const std::string& fileNameStd,
		    const std::string& fileNameInv,
		    int verb,int aimAt_);

  ~ParagraphSplitter();

  void applyNormal(const std::string& in,std::string& out) const;
};
#endif
