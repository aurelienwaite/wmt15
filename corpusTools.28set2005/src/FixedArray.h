/* ---------------------------------------------------------------- */
/* Copyright 1998 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef FIXARRAY_H_DEFINED
#define FIXARRAY_H_DEFINED
#include <iostream>
#include <string>
#include <functional>
#include <vector>

// write object and one blank
template<class T> bool writeOb(std::ostream& out,const T& f) { 
  out<<f<<" "; return 1;}

// read object and one blank
template<class T> bool readOb(std::istream& in,T& f) { 
  in>>f; char c; in.get(c); massert(c==' '); return 1;}

template<class T>
bool writeOb(std::ostream& out,const std::string& s,const T& f) { 
  out<<s<<" "<<f<<" "; return 1;}

template<class T> bool readOb(std::istream& in,const std::string& s,T& f) { 
  std::string ss;
  in>>ss;
  if(s!=ss) {
    std::cerr<<"ERROR: readOb should be '"<<s<<"' and is '"<<ss<<"'"<<std::endl;
    return 0;
  }
  return readOb(in,f);
}

template<class T,class Y=int> class FixedArray {
  private:  
  void copy(T *aa,const T *bb,int n) {while(n--) *aa++=*bb++;}
  
  public:
  typedef T* iterator;
  typedef const T* const_iterator;

  typedef T& reference;
  typedef const T& const_reference;
  
  T *p;                       	
  Y realSize;

  FixedArray() : p(0),realSize(0){}
  FixedArray(const FixedArray<T,Y> &x)
  : p(new T[x.realSize]),realSize(x.realSize) {copy(p,x.p,realSize);}
  explicit FixedArray(int n)
  : p(new T[n]),realSize(n){}
  FixedArray(int n,const T&_init)
  : p(new T[n]),realSize(n){for(int z=0;z<n;z++)p[z]=_init;}
  FixedArray(const FixedArray&f,const T&t)
  : p(new T[f.size()+1]),realSize(f.size()+1) {
    for(int z=0;z<f.size();z++)p[z]=f[z];p[f.size()]=t;}

  FixedArray(const std::vector<T>& v) {
    realSize=v.size();
    p=new T[realSize];
    copy(p,&v[0],realSize);
  }

  ~FixedArray() { delete [] p;p=0;realSize=-1;}
  
  FixedArray<T,Y>& operator=(const FixedArray<T,Y>&x) {
    if( this!= &x ) {
      delete [] p;
      realSize = x.realSize;
      p = new T[x.realSize]; 
      copy(p,x.p,realSize);
    }
    return *this;
  }
  
  void resize(int n) {
    if(n<=realSize) shrink(n);
    else {
      T*np=new T[n];
      copy(np,p,realSize);
      delete []p;
      p=np;
      realSize=n;
    }
  }

  void resize(int n,const_reference t) {
    int oldsize=realSize;
    resize(n);
    for(int i=oldsize;i<realSize;++i) p[i]=t;}
    



  void shrink(int n) {
    assert(n<=realSize);
    realSize=n;
  }
  void init(int n,const_reference _init) {
    delete []p;
    p=new T[n];
    realSize=n;
    for(int l=0;l<n;++l) p[l]=_init;
  }

  inline const_reference top(int n=0) const {return (*this)[realSize-1-n];}
  inline int size() const {return realSize;}

  inline iterator begin(){ return p; }
  inline iterator end()  { return p+realSize; }

  inline const_iterator begin() const {return p; }
  inline const_iterator end()   const {return p+realSize;}

  inline int low() const  {return 0;}
  inline int high() const {return realSize-1;}
  const void errorAccess(int n) const;
  
  inline reference operator[](int n) { 
#ifdef DEBUG
    if( n<0 ||n>=realSize )
      errorAccess(n);
#endif
    return p[n];
  }

  inline const_reference operator[](int n) const { 
#ifdef DEBUG
    if(n<0 || n>=realSize )
    errorAccess(n);
#endif
    return p[n]; 
  }

  bool writeTo(std::ostream& out) const {
    out << "FixedArray ";
    out << size() << " ";
    for(int a=0;a<size();a++)
    {
      writeOb(out,(*this)[a]);
      out << " ";
    }
    out << std::endl;
    return 1;
  }

  bool readFrom(std::istream& in) {
    std::string s;
    if( !in )
    {
      std::cerr << "ERROR(FixedArray): file cannot be opened.\n";
      return 0;
    }
    in >> s;
    if( !(s=="FixedArray") )
    {
      std::cerr << "ERROR(FixedArray): FixedArray!='"<<s<<"'\n";
      return 0;
    }
    int biggest;
    in >> biggest;
    resize(biggest);
    for(int a=0;a<size();a++) readOb(in,(*this)[a]);
    return 1;
  }

  void sort(int until=-1)
    {
      if( until== -1 ) until=size();
      std::sort(p,p+until);
    }

  void invsort(int until=-1)
    {
      if( until== -1 ) until=size();
      std::sort(p,p+until,std::greater<T>());      
    }

  int binary_locate(const_reference t)
    {
      iterator ppos=std::lower_bound(p,p+size(),t);
      int pos=ppos-p;
      if( pos>=-1&&pos<size() )
	return pos;
      else
	return -1;
    }

  int binary_search(const_reference t)
    {
      iterator ppos=std::lower_bound(p,p+size(),t);
      int pos=ppos-p;
      if( pos>=0&&pos<size()&& *ppos==t )
	return pos;
      else
	return -1;
    }

};

template<class T,class Y> bool operator<(const FixedArray<T,Y> &x, const FixedArray<T,Y> &y)
{return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());}

template<class T,class Y>  bool operator==(const FixedArray<T,Y> &x, const FixedArray<T,Y> &y){
  if( &x == &y )return 1;
  const int s = x.size();
  if( s !=y.size() )return 0;
  const T*a=x.p,*b=y.p,*c=x.p+s;
  //  for(int iii=0;iii<s;iii++)
  //  if( !(x.p[iii]==y.p[iii]) )
  //    return 0;
  while(a!=c)
    if( !(*a++ == *b++) )
      return 0;
  return 1;
}

template<class T,class Y> int Hash(const FixedArray<T,Y>&a)
{
  int n=0;
  const int s=a.size();
  for(int iii=0;iii<s;iii++)
    n=13*n+Hash(a.p[iii]);
  return n;
}

template<class T,class Y> const void FixedArray<T,Y>:: errorAccess(int n) const
{
  massert(0);
  std::cerr 	<< "ERROR: Access to array element " << n 
	<< " (" << realSize << "," << (void*)p << ")\n";
}
	
template<class T,class Y> std::ostream& operator<<(std::ostream&o,const FixedArray<T,Y>&a)
{
  o << "FixedArray(" << a.size() << "){ ";
  for(int iii=0;iii<a.size();++iii)
    o << " " << iii<< ":" << a[iii]<<";";
  return o << "}\n";
}

template<class T,class Y> std::istream& operator>>(std::istream&in, FixedArray<T,Y>&)
{ return in;}

template<class T,class Y> FixedArray<T,Y> operator+(const FixedArray<T,Y>&a,const FixedArray<T,Y>&b)
{
  massert(a.size()==b.size());
  FixedArray<T,Y> x(a.size());
  for(int iii=0;iii<a.size();++iii)
    x[iii]=a[iii]+b[iii];
  return x;
}
template<class T,class Y> void writeOr(const FixedArray<T,Y>&aaa,const FixedArray<T,Y>&bbb,FixedArray<T,Y>&xxx)
{
  assert(aaa.size()==bbb.size());
  assert(xxx.size()==bbb.size());
  for(int iii=0;iii<aaa.size();++iii)
    xxx.p[iii]=(aaa.p[iii] || bbb.p[iii]);
  //return xxx;
}

#endif



