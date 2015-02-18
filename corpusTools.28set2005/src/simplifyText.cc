/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#include "IOfile.hh"

using namespace std;

int main(int argc,char** argv) {
  string ifile,ofile;
  bool zip=0;

  for(int i=1;i<argc;++i) {
    string s(argv[i]);
    if(s=="-zip") zip=1;
    else if(s=="-i") ifile=string(argv[++i]);
    else if(s=="-o") ofile=string(argv[++i]);
  }

  string line;
  Ifile in(ifile);
  Ofile out(ofile,zip);

  while( getline(in,line) ) {
    istringstream eingabe(line);
    string s;
    bool printed=0;
    out<<' ';
    while(eingabe>>s) {
      if( printed==1 ) out<<' '; else printed=1;
      out<<s;
    }
    out<<'\n';
  }
}
