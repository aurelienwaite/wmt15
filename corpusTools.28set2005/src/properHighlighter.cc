/* ---------------------------------------------------------------- */
/* Copyright 2002 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#define NO_LEDA
#include <set>
#include <ctype.h>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <iterator>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE
#endif 

using namespace std;

int main(int argc,char**argv)
{
  if( argc==1 )
    {
      cerr << "USAGE:\n"<<argv[0]<<" pNounList \n";
      exit(1);
    }

  assert(argc==2);
  ifstream *f1=0;
  
  for(int i=1;i<argc;i++) {
    string s=argv[i];
    if( f1==0 )
      f1=new ifstream(s.c_str()); 
  }   
  if( !f1 ) {
    cerr << argv[0] << " wrong usage.\n";
  }
  assert(f1); 
  
  string t1,t2;
  string s;
  int l=0;
  set<string> ss1;

  while( getline(*f1,t1) ){
    istringstream e1(t1.c_str());
    while(e1>>s)
      ss1.insert(s);
    l++;
    if( l%10000==0 )
      cerr << "Reading " << l << " proper nouns." << endl;
  }
  while( getline(cin,t2) ){
    istringstream e(t2.c_str());
    while( e>>s) {
      if( ss1.find(s) != ss1.end() )
	cout << "_PNOUN { " << s << " } ";
      else
	cout << s << " ";
    }
    cout << endl;   
  }
}
