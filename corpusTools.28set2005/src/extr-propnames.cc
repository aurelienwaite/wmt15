/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#define NO_LEDA
#include <set>
#include <ctype.h>
#include <fstream>
#include <assert.h>

using namespace std;
#include "Array.h"

int main(int argc,char**argv)
{
  assert(argc==3||argc==4);
  ifstream *f1=0,*f2=0;
  int printPropers=0;

  for(int i=1;i<argc;i++)
    {
      string s=argv[i];
      if( s=="-propers" )
	printPropers=1;
      else 
	{
	  if( f1==0 )
	    f1=new ifstream(s.c_str());
	  else
	    f2=new ifstream(s.c_str());
	}
    }
  if( !f1 || !f2 )
    {
      cerr << argv[0] << " wrong usage.\n";
    }
  assert(f1&&f2); 
  set<string> words;
  set<string> noproper;
  string s;
  Array<string> t1,t2;
  int l=0;
  while( getline(*f1,t1[t1.size()])&&getline(*f2,t2[t2.size()]) )
    {
      multiset<string> ss1,ss2;
      istringstream e1(t1.top().c_str()),e2(t2.top().c_str());
      while(e1>>s)
	{
	  words.insert(s);
	  ss1.insert(s);
	}
      while(e2>>s)
	{
	  words.insert(s);
	  ss2.insert(s);
	}
      l++;
      for(multiset<string>::iterator i=ss1.begin();i!=ss1.end();++i)
	if( ss1.count(*i)!=ss2.count(*i) )
	  noproper.insert(*i);
      for(multiset<string>::iterator i=ss2.begin();i!=ss2.end();++i)
	if(ss1.count(*i)!=ss2.count(*i) )
	  noproper.insert(*i);
    }
  cerr << "Lines: " << t1.size() << endl;
  cerr << "Words: " << words.size() << endl;
  set<string> proper;
  for(set<string>::iterator i=words.begin();i!=words.end();++i)
    if( noproper.count(*i)==0 )
      proper.insert(*i);
  cerr << "Propers: " << proper.size() << endl;

  if( printPropers==0 )
    {
      over_arr(t1,i)
	{
	  istringstream e1(t1[i].c_str());
	  while(e1>>s)
	    {
	      if( proper.count(s) )
		cout << "PROPERNAME";
	      else
		cout << s;
	      cout << " ";
	    }
	  cout << '\n';
	}
    }
  else
    for(set<string>::iterator i=proper.begin();i!=proper.end();++i)
      cout << *i << '\n';
 return 0;
}

