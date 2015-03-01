/* ---------------------------------------------------------------- */
/* Copyright 2003 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#ifndef PAIR_IO_HH_
#define PAIR_IO_HH_
#include <iostream>
#include <utility>
#include <functional>

template<class A,class B>
std::ostream& operator<<(std::ostream& out,const std::pair<A,B>& x) {
  out<<x.first<<' '<<x.second; return out;}

template<class P>
struct pair_less_first : public std::binary_function<P,P,bool> {
  typedef P pairtype;
  typedef typename std::binary_function<pairtype,pairtype,bool>::second_argument_type second_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::first_argument_type first_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::result_type result_type; 

  pair_less_first() {}
  bool operator()(const pairtype& a,const pairtype& b) {
    return (a.first<b.first);}
};

template<class P>
struct pair_less_second : public std::binary_function<P,P,bool> {
  typedef P pairtype;
  typedef typename std::binary_function<pairtype,pairtype,bool>::second_argument_type second_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::first_argument_type first_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::result_type result_type; 

  pair_less_second() {}
  bool operator()(const pairtype& a,const pairtype& b) {
    return (a.second<b.second);}
};


template<class P>
struct pair_greater_first : public std::binary_function<P,P,bool> {
  typedef P pairtype;
  typedef typename std::binary_function<pairtype,pairtype,bool>::second_argument_type second_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::first_argument_type first_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::result_type result_type; 

  pair_greater_first() {}
  bool operator()(const pairtype& a,const pairtype& b) {
    return (a.first>b.first);}
};

template<class P>
struct pair_greater_second : public std::binary_function<P,P,bool> {
  typedef P pairtype;
  typedef typename std::binary_function<pairtype,pairtype,bool>::second_argument_type second_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::first_argument_type first_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::result_type result_type; 

  pair_greater_second() {}
  bool operator()(const pairtype& a,const pairtype& b) {
    return (a.second>b.second);}
};

template<class P>
struct pair_equal_first : public std::binary_function<P,P,bool> {
  typedef P pairtype;
  typedef typename std::binary_function<pairtype,pairtype,bool>::second_argument_type second_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::first_argument_type first_argument_type; 
  typedef typename std::binary_function<pairtype,pairtype,bool>::result_type result_type; 

  pair_equal_first() {}
  bool operator()(const pairtype& a,const pairtype& b) {
    return (a.first==b.first);}
};



// std::ostream& operator<<(std::ostream& out,const std::pair<A,const B>& x) {
//   out<<x.first<<' '<<x.second; return out;}
// template<class A,class B>
// std::ostream& operator<<(std::ostream& out,const std::pair<const A,B>& x) {
//   out<<x.first<<' '<<x.second; return out;}
// template<class A,class B>
// std::ostream& operator<<(std::ostream& out,const std::pair<const A,const B>& x) {
//   out<<x.first<<' '<<x.second; return out;}

template<class A,class B> 
std::istream& operator>>(std::istream& in,std::pair<A,B>& x) {
  in>>x.first>>x.second; return in;}

#endif
