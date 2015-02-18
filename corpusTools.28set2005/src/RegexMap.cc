/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include "RegexMap.h"
//#include "ATutils.hh"
#include "IOfile.hh"
#undef over_arr
#define over_arr(a,i) for(unsigned int i=0;i<(a).size();i++)

#include <sys/stat.h>
inline bool existsFile(const char* filename) {
  struct stat mystat;
  return  (stat(filename,&mystat)==0);
}
inline bool existsFile(const std::string& filename) {
  return existsFile(filename.c_str());
}

RegexMap::RegexMap(const std::string& fnameNormal,const std::string& fnameInvers,int verb) {
  if(existsFile(fnameNormal)) {
    Ifile in(fnameNormal);
    readFromNormal(in,verb);
  }
  if(existsFile(fnameInvers)) {
    Ifile in(fnameInvers);
    readFromInvers(in,verb);
  }
}


void RegexMap::readFromStream(std::istream&,int) {
  std::cerr<<"ERROR: this function is not implemented\n"
    " for a RegexMap you have to call readFromNormal or readFromInvers!\n";

  // go on in non-debug mode, then this map won't change anything
#ifdef DEBUG
  abort();
#endif
}


void readFrom(std::istream&in,int,std::vector< std::pair<regex_t,std::string> >&tabelle)
{
  if( !in ) 
    {
      std::cerr << "WARNING: can not read PhraseMap.\n";
      return;
    }
  std::string s; 
  while( getline(in,s)&&s!="#END-OF-MAP#" )
    {
      if( s.length())
	{
	  char c=s[0];
	  unsigned int i;
	  for(i=1;i<s.size()&&s[i]!=c;i++)
	    ;
	  std::string first(s,1,i-1);
	  unsigned int j;
	  for(j=i+1;j<s.size()&&s[j]!=c;j++)
	    ;
	  std::string second(s,i+1,j-i-1);
	  regex_t t;
	  int ret;
	  //cerr << "FIRST: " << first << " SECOND: " << second << endl;
	  if( (ret=regcomp(&t,first.c_str(),0)) )
	    {
	      std::cerr << "ERROR:RegexMap::readFrom: regular expression compile error: " << ret << std::endl;
	      //regerror(ret);
	    }
	  else
	    tabelle.push_back( std::pair<regex_t,std::string>(t,second) );
	}
    }
}

void RegexMap::readFromNormal(std::istream&in,int verb) {
  readFrom(in,verb,tabelleNormal);
}
void RegexMap::readFromInvers(std::istream&in,int verb) {
  readFrom(in,verb,tabelleInvers);
}


void apply(const std::string&in,std::string&changed,const std::vector< std::pair<regex_t,std::string> >&tabelle) 
{
  changed=in;
  //cerr << "TABSIZE: " << tabelle.size() << endl;
  over_arr(tabelle,tabi)
    { 
      int position=0;
      std::vector<regmatch_t> match(tabelle[tabi].first.re_nsub+1);
      //cerr << "CALL: " << tabi << " " << changed.c_sntr()+position << " " << match.size() << endl;
      while( regexec(&(tabelle[tabi].first),changed.c_str()+position,match.size(),&(match[0]),0)==0)
	{
	  std::string repstring=tabelle[tabi].second;
	  int start=match[0].rm_so+position;
	  int n=match[0].rm_eo-match[0].rm_so;
	  if( match.size()>=1 )
	    {
	      //cerr << "REPSTRING VORHER: " << repstring << endl;
	      for(unsigned int i=1;i<match.size();i++)
		{
		  char symbol[3];
		  symbol[0]='$';
		  symbol[1]='0'+i;
		  symbol[2]=0;
		  unsigned int pos=repstring.find(symbol);
		  if( pos<repstring.size() )
		    repstring.replace(pos,2,changed.c_str()+position+match[i].rm_so,match[i].rm_eo-match[i].rm_so);
		}
	      //cerr << "REPSTRING NACHHER: " << repstring << endl;
	    }
	  //cerr << "CHANGED VORHER: " << changed << endl;
	  changed.replace(start,n,repstring.c_str(),repstring.length());
	  //cerr << "CHANGED NACHHER: " << changed << endl;
	  
	  position=start+repstring.length();
	}
    }
}

void RegexMap::applyNormal(const std::string&in,std::string&changed) const {
  apply(in,changed,tabelleNormal);
}
void RegexMap::applyInvers(const std::string&in,std::string&changed) const {
  apply(in,changed,tabelleInvers);
}

