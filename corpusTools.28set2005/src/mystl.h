/* ---------------------------------------------------------------- */
/* Copyright 1998 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef MY_STL_H_DEFINED
#define MY_STL_H_DEFINED
#include <string>
#ifdef USE_STLPORT
#ifdef __STL_DEBUG
using namespace _STL;
#else
using namespace _STL;
#endif
#else
#if __GNUC__>=3
using namespace std;
#endif
#endif

#include "myassert.h"
#include <string>
#include <utility>

// RZ:  USE_MY_HASH_MAP may be only defined here
//#define USE_MY_HASH_MAP

#ifdef USE_MY_HASH_MAP
#include "my_hash_map.H"
#else
#include "hash_map"
#endif

#include <iostream>
#include "mymath.h"
#include "Array2.h"
#include <sstream>

#include "pairIO.hh"


#define over_string(a,i) for(unsigned int i=0;i<a.length();i++)
#define over_array(a,i) for(i=(a).low();i<=(a).high();i++)
#define backwards_array(a,i) for(i=(a).high();i>=(a).low();i--)
#define over_arr(a,i) for(int i=(a).low();i<=(a).high();i++)
#define over_arrMAX(a,i,max) for(int i=(a).low();i<=min((a).high(),max-1);i++)
#define backwards_arr(a,i) for(int i=(a).high();i>=(a).low();i--)

extern double n1mult,n2mult,n3mult;

template<class A,class B>
inline double realProb(A n1,B n2) {
  massert(n1<=n2);
  iassert(n1>=0&&n2>0);
  if(n2==0) n2=1;
  return (n1/(1.0*n2));
}

template<class A,class B>
inline double verfProb(A n1,B n2) {
  double prob = realProb(n1,n2);
  if( n1<=1 ) return prob*n1mult;
  else if( n1<=2 ) return prob*n2mult;
  else if( n1<=3 ) return prob*n3mult;
  else return prob;
}

inline bool prefix(const std::string&x,const std::string&y,bool makeLower) {
  if(y.size()>x.size() ) return 0;
  if( makeLower==0 ) {
    for(unsigned int i=0;i<y.size();++i) if( y[i]!=x[i] ) return 0;
  }
  else {
    for(unsigned int i=0;i<y.size();++i) if( tolower(y[i])!=tolower(x[i])) return 0;
  }
  return 1;
}

template<class T>
std::string string_cast(const T& x) {
  std::ostringstream os;os<<x;return os.str();}


template<class T>
int lev(const T&s1,const T&s2)
{
  Array2<int,std::vector<int> > a(s1.size()+1,s2.size()+1,1000);
  //  Array2<pair<int,int>,vector<pair<int,int> > > back(s1.size()+1,s2.size()+1,pair<int,int>(0,0));
  for(unsigned int i=0;i<=(unsigned int)(s1.size());i++)
    for(unsigned int j=0;j<=(unsigned int)(s2.size());j++)
      {
	if( i==0&&j==0 )
	  a(i,j)=0;
	else
	  {
	    int aDEL=100,aINS=100,aSUB=100;
	    if(i>0)
	      aDEL=a(i-1,j)+1;
	    if(j>0)
	      aINS=a(i,j-1)+1;
	    if(i>0&&j>0)
	      aSUB=a(i-1,j-1)+ !(s1[i-1]==s2[j-1]);
	    if( aSUB<=aDEL && aSUB<=aINS )
	      {
		a(i,j)=aSUB;
		//		back(i,j)=pair<int,int>(i-1,j-1);
	      }
	    else if( aDEL<=aSUB && aDEL<=aINS )
	      {
		a(i,j)=aDEL;
		//		back(i,j)=pair<int,int>(i-1,j);
	      }
	    else
	      {
		a(i,j)=aINS;
		//		back(i,j)=pair<int,int>(i,j-1);
	      }
	  }
      }
  return a(s1.size(),s2.size());
}

template<class T>
int lev_with_back(const T&s1,const T&s2,Array2<std::pair<int,int>,std::vector<std::pair<int,int> > >&back)
{
  Array2<int,std::vector<int> > a(s1.size()+1,s2.size()+1,1000);
  back =Array2<std::pair<int,int>,std::vector<std::pair<int,int> > >(s1.size()+1,s2.size()+1,std::pair<int,int>(0,0));
  for(unsigned int i=0;i<=(unsigned int)(s1.size());i++)
    for(unsigned int j=0;j<=(unsigned int)(s2.size());j++)
      {
	if( i==0&&j==0 )
	  a(i,j)=0;
	else
	  {
	    int aDEL=100,aINS=100,aSUB=100;
	    if(i>0)
	      aDEL=a(i-1,j)+1;
	    if(j>0)
	      aINS=a(i,j-1)+1;
	    if(i>0&&j>0)
	      aSUB=a(i-1,j-1)+ !(s1[i-1]==s2[j-1]);
	    if( aSUB<=aDEL && aSUB<=aINS )
	      {
		a(i,j)=aSUB;
		back(i,j)=pair<int,int>(i-1,j-1);
	      }
	    else if( aDEL<=aSUB && aDEL<=aINS )
	      {
		a(i,j)=aDEL;
		back(i,j)=pair<int,int>(i-1,j);
	      }
	    else
	      {
		a(i,j)=aINS;
		back(i,j)=pair<int,int>(i,j-1);
	      }
	  }
      }
  return a(s1.size(),s2.size());
}

template<class T>
float rel_lev(const T&s1,const T&s2)
{
  if( s1.size()==0 )
    return s2.size()==0;
  else
    return min(1.0,lev(s1,s2)/(double)s1.size());
}

template<class V> int Hash(const std::pair<V,V>&a) 
{ return Hash(a.first)+13001*Hash(a.second); }

/* template<class T1,class T2> */
/* std::ostream& operator<<(std::ostream &out,const std::pair<T1,T2> &ir) */
/* {  */
/*   out << "(" << ir.first << "," << ir.second << ")"; */
/*   return out; */
/* }  */

inline int Hash(const std::string& s)
{
  int sum=0;
  std::string::const_iterator i=s.begin(),end=s.end();
  for(;i!=end;i++)sum=5*sum+(*i);
  return sum;
}

template<class A,class B,class C,class D>
class quad
{
public:
  A a;
  B b;
  C c;
  D d;
  quad(){};
  quad(const A&_a,const B&_b,const C&_c,const D&_d)
    : a(_a),b(_b),c(_c),d(_d) {}
};
template<class A,class B,class C,class D>
bool operator==(const quad<A,B,C,D>&x,const quad<A,B,C,D>&y)
{ return x.a==y.a&&x.b==y.b&&x.c==y.c&&x.d==y.d;}

template<class A,class B,class C,class D>
bool operator<(const quad<A,B,C,D>&x,const quad<A,B,C,D>&y)
{
  if(x.a<y.a)return 1;
  if(y.a<x.a)return 0;
  if(x.b<y.b)return 1;
  if(y.b<x.b)return 0;
  if(x.c<y.c)return 1;
  if(y.c<x.c)return 0;
  if(x.d<y.d)return 1;
  if(y.d<x.d)return 0;
  return 0;
}


template<class A,class B,class C>
class tri
{
public:
  A a;
  B b;
  C c;
  tri(){};
  tri(const A&_a,const B&_b,const C&_c)
    : a(_a),b(_b),c(_c) {}
};
template<class A,class B,class C>
bool operator==(const tri<A,B,C>&x,const tri<A,B,C>&y)
{ return x.a==y.a&&x.b==y.b&&x.c==y.c;}

template<class A,class B,class C>
bool operator<(const tri<A,B,C>&x,const tri<A,B,C>&y)
{
  if(x.a<y.a)return 1;
  if(y.a<x.a)return 0;
  if(x.b<y.b)return 1;
  if(y.b<x.b)return 0;
  if(x.c<y.c)return 1;
  if(y.c<x.c)return 0;
  return 0;
}

double used_time();

template<class T>
class my_hash
{
public:
  int operator()(const T&t)const {return Hash(t);}
};

template<class T>
class my_eq
{
public:
  bool operator()(const T&t1,const T&t2)const {return t1==t2;}
};

inline int Hash(int value) { return value; }

using namespace std;

/* #ifdef USE_MY_HASH_MAP */
/* #define MY_HASH_BASE hash_map<A,B,my_hash<A>,my_eq<A> > */
/* #else */
/* #define MY_HASH_BASE hash_map<A,B,my_hash<A> > */
/* #endif */
// #include <hash_map>

template<class A,class B>
#if __GNUC__>=3
  class leda_h_array : public __gnu_cxx::hash_map<A,B,my_hash<A> > {
  typedef __gnu_cxx::hash_map<A,B,my_hash<A> > mybaseclass;
#else
  class leda_h_array : public hash_map<A,B,my_hash<A> > {
  typedef hash_map<A,B,my_hash<A> > mybaseclass;
#endif
  private:
  B init;
  public:

  leda_h_array() : mybaseclass() {}
  leda_h_array(const B&_init)
    : mybaseclass(),init(_init) {}
  bool defined(const A&a) const
    { return find(a)!=mybaseclass::end(); }
  const B& operator[](const A&a)const
    { 
      typename mybaseclass::const_iterator pos=find(a);
      if( pos==mybaseclass::end() )
	return init;
      else
	return pos->second;
    }
  B&operator[](const A&a)
    { 
      typename mybaseclass::iterator pos=find(a);
      if( pos==mybaseclass::end() )
	{
	  insert(typename mybaseclass::value_type(a,init));
	  pos=find(a);
	  iassert(pos!=mybaseclass::end());
	}
      return pos->second;
    }
  const B&initValue()const
    {return init;}
};

#ifdef USE_MY_HASH_MAP
#define forall_defined_h(a,b,c,d) for(typename leda_h_array<a,b,my_hash<a>,my_eq<a> >::const_iterator __jj__=(d).begin();__jj__!=(d).end()&&((c=__jj__->first),1); ++__jj__)
#else
#define forall_defined_h(a,b,c,d) for(leda_h_array<a,b>::const_iterator __jj__=(d).begin();__jj__!=(d).end()&&((c=__jj__->first),1); ++__jj__)
#define forall_defined_ht(a,b,c,d) for(typename leda_h_array<a,b>::const_iterator __jj__=(d).begin();__jj__!=(d).end()&&((c=__jj__->first),1); ++__jj__)
#endif

template<class T,class U>
std::ostream & operator<<(std::ostream&out,const leda_h_array<T,U>&w)
{
  T t;
  bool makeNl=0;
  out << "h_array{";
  forall_defined_ht(T,U,t,w)
    {
      if( makeNl )
	out << "\n       ";
      out << "EL:" << t << " INH:" << w[t] << ".";
      makeNl=1;
    }
  return out << "}\n";
}

template<class T,class U>
std::istream & operator>>(std::istream&in,leda_h_array<T,U>&)
{
  return in;
}

template<class A,class B>
bool operator==(const leda_h_array<A,B>&p1,const leda_h_array<A,B>&p2)
{
  A v;
  forall_defined_ht(A,B,v,p1)
    if( !( p1[v]==p2[v]) ) return 0;
  forall_defined_ht(A,B,v,p2)
    if( !( p1[v]==p2[v]) ) return 0;
  return 1; 
}

template<class T>
int count_elements(T a,T b)
{
  int c=0;
  while(a!=b)
    {
      a++;
      c++;
    }
  return c;
}

template<class T>
T normalize_if_possible_with_increment(T*a,T*b,int increment)
{
  T sum=0;
  for(T*i=a;i!=b;i+=increment)
    sum+=*i;
  if( sum )
    for(T*i=a;i!=b;i+=increment)
      *i/=sum;
  else
    {
      T factor=increment/(b-a);
      for(T*i=a;i!=b;i+=increment)
	*i=factor;
    }
  return sum;
}

template<class T>
inline int m_comp_3way(T a,T b,int n)
{
  int _n=0;
  while((_n++<n) && a && b)
    {
      const typename T::value_type &aa=*a;
      const typename T::value_type &bb=*b;
      if( aa<bb )return 1;
      if( bb<aa )return -1;
      ++a;
      ++b;
    }
  return 0;
}

template<class T>
void smooth_standard(T*a,T*b,double p)
{
  int n=b-a;
  if( n==0 ) 
    return;
  double pp=p/n;
  for(T*i=a;i!=b;++i)
    *i = (1.0-p)*(*i)+pp;
}

#endif
