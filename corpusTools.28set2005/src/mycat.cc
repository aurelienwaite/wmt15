/* ---------------------------------------------------------------- */
/* Copyright 2003 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <vector>
#include <string>
#include "IOfile.hh"

using namespace std;

int main(int argc,char**argv) {
  vector<string> files;
  string ofile;
  bool zip=0;
  
  for(int i=1;i<argc;++i) {
    string s(argv[i]);
    if(s=="-zip") zip=1;
    else if(s=="-o") ofile=string(argv[++i]);
    else if(s=="-h") {
      cout<<argv[0]<<" file1 file2 ... \n"
	  <<" options:\n"
	  <<"    -zip     -- gzip output\n\n"
	  <<"    -o file  -- output file (default stdout)\n"
	  <<" concatenate files (input files may be gzipped or unzippped\n\n";
      return 1;}
    else files.push_back(s);
  }

  Ofile out(ofile,zip);
  for(size_t i=0;i<files.size();++i) {
    Ifile in(files[i]);
    string line;
    while(getline(in,line)) out<<line<<'\n';
  }
}
