/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include "Tokenizer.h"
#include <iostream>
#include <string>


using namespace std;
bool DebugPhraseMap;

int main(int argc,char **argv) {
  bool inv=0,zip=0;
  string ifile,ofile,mapfile;
  
  for(int i=1;i<argc;++i) {
    string s(argv[i]);
    if(s=="-h" || s=="--help") {
      cerr << argv[0] << " usage: \n";
      cerr << argv[0] << " [ corpus ] map [ -inv ] " << endl;
      return 1;
    }
    else if(s=="-i") ifile=string(argv[++i]);
    else if(s=="-o") ofile=string(argv[++i]);
    else if(s=="-zip") zip=1;
    else if(s=="-inv") inv=1;
    else if(s=="-dbg") DebugPhraseMap=1;
    else if(ifile=="") ifile=s;
    else if(mapfile=="") mapfile=s;
  }

  if(mapfile=="") {mapfile=ifile;ifile="";}

  if(mapfile=="") {
    cerr<<argv[0]<<" ERROR: no mapfile given!"<<endl;
    return 1;
  }

  
  Tokenizer phraseMap;
  phraseMap.readFromFile(mapfile,1);

  string s,outs;
  while(getline(cin,s)) {
    if( inv ) phraseMap.applyInvers(s,outs);
    else      phraseMap.applyNormal(s,outs);
    cout << outs << endl;
  }
}
