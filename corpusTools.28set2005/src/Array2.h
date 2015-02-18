/* ---------------------------------------------------------------- */
/* Copyright 1998 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef CLASS_Array2_DEFINED
#define CLASS_Array2_DEFINED
#include "FixedArray.h"

template<class T,class Y=FixedArray<T,short> > class Array2
{
  public:
  Y p;
  //  signed char h1,h2;
  short h1,h2;

  Array2(int _h1,int _h2) 
    : p(_h1*_h2),h1(_h1),h2(_h2) {}
  Array2(int _h1,int _h2,const T&_init) 
    : p(_h1*_h2,_init),h1(_h1),h2(_h2) {}
  Array2() 
    : h1(0),h2(0) {}
  inline T &operator()(int i,int j)
    { massert(i<h1);massert(j<h2);return p[i*h2+j]; }
  inline const T&operator()(int i,int j) const
    { massert(i<h1);massert(j<h2);return p[i*h2+j]; }
  inline T get(int i,int j)
    { massert(i<h1);massert(j<h2);return p[i*h2+j]; }
  inline void set(int i,int j,T x)
    { massert(i<h1);massert(j<h2);p[i*h2+j]=x; }
  inline const T get(int i,int j) const
    { massert(i<h1);massert(j<h2);return p[i*h2+j]; }
  inline int getLen1() const
    { return h1; }
  inline int getLen2() const
    { return h2; }
  inline void resize(int a,int b)
    {
      if( !(a==h1&&b==h2))
	{
	  h1=a;
	  h2=b;
	  p.resize(h1*h2);
	}
    }
  inline void resize(int a,int b,const T&t)
    {
      if( !(a==h1&&b==h2))
	{
	  h1=a;
	  h2=b;
	  p.resize(h1*h2,t);
	}
    }
  
inline typename Y::iterator begin(){ return p.begin(); }
inline typename Y::iterator end(){ return p.end(); }

inline typename Y::const_iterator begin()const{ return p.begin(); }
inline typename Y::const_iterator end()const{return p.end();}

  bool writeTo(ostream&out) const
    {
      out << "Array2 ";
      out << h1 << " " << h2 << " ";
      return p.writeTo(out);
    }
  bool readFrom(istream&in)
    {
      string str;
      in >> str;
      if( str!="Array2" )
	{
	  cerr << "ERROR:Array2:readFrom: Not read: Array2 " << str << endl;
	  return 0;
	}
      in >> h1 >> h2;
      return p.readFrom(in);
    }
};

template<class T> ostream&operator<<(ostream&out,const Array2<T,FixedArray<T> >&a)
{
  for(int i=0;i<a.getLen1();i++) {
    for(int j=0;j<a.getLen2();j++)
      out << a(i,j) << " ";
    out << endl;
  }
  return out;
}
template<class T> ostream&operator<<(ostream&out,const Array2<T,FixedArray<T,short> >&a)
{
  for(int i=0;i<a.getLen1();i++) {
    for(int j=0;j<a.getLen2();j++)
      out << a(i,j) << " ";
    out << endl;
  }
  return out;
}

template<class T,class Y > bool
operator==(const Array2<T,Y>&a,const Array2<T,Y>&b){
  return a.h1==b.h1 && a.h2==b.h2 && a.p==b.p;}

template<class T,class Y > bool
operator<(const Array2<T,Y>&a,const Array2<T,Y>&b){ 
  if(a.h1<b.h1)      return 1;
  else if(b.h1<a.h1) return 0;
  else if(a.h2<b.h2) return 1;
  else if(b.h2<a.h2) return 0;
  else return a.p==b.p;
}


#endif
