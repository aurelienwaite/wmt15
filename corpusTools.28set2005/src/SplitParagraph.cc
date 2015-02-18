/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include "SplitParagraph.hh"
#include <vector>
#include <sstream>
#include <cmath>
#include <cassert>
#include "RegexMap.h"

std::string splitParagraph(const std::string&_s,const RegexMap&m,int aimAt);


ParagraphSplitter::ParagraphSplitter() : StringMap(),m(new RegexMap()),aimAt(25) {}
ParagraphSplitter::ParagraphSplitter(const std::string& fn,const std::string& fi,int verb,int aimAt_) : StringMap(),m(new RegexMap(fn,fi,verb)),aimAt(aimAt_) {}
														      
ParagraphSplitter::~ParagraphSplitter() {delete m;}

void ParagraphSplitter::applyNormal(const std::string& in,std::string& out) const {
  assert(m);
  out=splitParagraph(in,*m,aimAt);
}

double costsSymbol(const std::string&s)
{
  if(s=="."||s=="!"||s=="?"||s==":")
    return 0.0;
  else if(s==","||s==";")
    return 25.0;
  else if( s.length()==1&&ispunct(s[0]) )
    return 30.0;
  else
    return 40.0;
}

double costs(int d,int aimAt) {
  if( d<aimAt ) return 0.0;
  else return pow(static_cast<double>(d-aimAt),1.1);
}

std::string splitParagraph(const std::string&_s,const RegexMap&m,int aimAt)
{
  std::string s=_s+" ";
  std::string s2;
  m.applyNormal(s,s2);
  std::cout << "PREPSTRING: " << s2 << std::endl;
  std::string w;
  
  std::istringstream is2(s2);
  std::vector<std::string> vis2;
  while(is2>>w)
    vis2.push_back(w);

  if( vis2.size()==0 )
    return std::string();

  std::vector<double> score(vis2.size(),1e6);
  std::vector<int> back(vis2.size(),-1);
  for(int i=0;i<int(vis2.size());++i)
    {
      double cs=costsSymbol(vis2[i]);
      for(int j=-1;j<i;++j)
	{
	  double add=cs+costs(i-j,aimAt);
	  double newscore=((j==-1)?0.0:score[j])+add;
	  //cerr << "nsc: " << j << " " << score[i] << " add " << add << endl;
	  if( newscore<score[i] )
	    {
	      score[i]=newscore;
	      back[i]=j;
	    }
	}
    }
  for(unsigned int i=0;i<vis2.size();++i)
    std::cerr << i << ": " << score[i] << " " << back[i] << "; ";
  std::cerr << std::endl;
  int b=vis2.size()-1;
  int n=0;
  while(b!=-1)
    {
      b=back[b];
      n++;
    }
  if( n==0 )
    return 0;
  //cerr << "found: " << n << endl;
  std::vector<std::string> result(n);
  int b1=vis2.size()-1;
  int b0=back[b1];
  int pos=n-1;
  do
    {
      //cerr << "I make results["<<pos<<"]" << b0 << " " << b1 <<endl;
      for(int i=b0+1;i<=b1;++i)
	result[pos]+=vis2[i]+" ";
      pos--;
      b1=b0;
      if( b0!=-1 )
	b0=back[b0];
    } while(b1!=-1);
  std::string rresult;
  for(unsigned int i=0;i<result.size();++i)
    {
      //cout << "L:" << i << " " << result[i] << endl;
      rresult+=result[i];
      if( i<result.size()-1 )
	rresult+="\n";
    }
  std::string rrresult;
  m.applyInvers(rresult,rrresult);
  return rrresult;
}

