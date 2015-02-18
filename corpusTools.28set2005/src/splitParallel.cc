/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "IOfile.hh"

using namespace std;

int main(int argc,char **argv) {
  string sfile,tfile,ifile;bool zip=0;

  for(int i=1;i<argc;++i) {
    string s(argv[i]);
    if(s=="-s") sfile=string(argv[++i]);
    else if(s=="-t") tfile=string(argv[++i]);
    else if(s=="-i") ifile=string(argv[++i]);
    else if(s=="-zip") zip=1;
  }

  Ifile in(ifile);
  Ofile out1(sfile,zip),out2(tfile,zip);

  string line;
  while( getline(in,line)) {
    istringstream is(line);
    bool hash=0;string w;
    while(is>>w)
      if(w=="#") hash=1;
      else if(hash) out2<<w<<' '; else out1<<w<<' ';
    out1<<'\n';out2<<'\n';
  }
}
