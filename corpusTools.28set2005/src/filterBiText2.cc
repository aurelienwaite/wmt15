/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef vector<string> Sent;

std::ostream& operator<<(std::ostream& out,const Sent& s) {
  if(s.size())
    copy(s.begin(),s.end(),ostream_iterator<string>(out," "));
  else out<<"EMPTY-SENT";
  return out;
}

void printPair(std::ostream& out,const Sent& s,const Sent& t,bool counts,double c) {
  if(counts) out<<c<<' ';
  out<<s<<" # ";
  if(counts) out<<c<<' ';
  out<<t<<'\n';
}

bool checkSentForRealWord(const Sent& s) {
  for(Sent::const_iterator i=s.begin();i!=s.end();++i) {
    bool real=1;
    for(std::string::const_iterator j=i->begin();j!=i->end();++j)
      if(!isalpha(*j)) {real=0;break;}
    if(real) return 1;
  }
  return 0;
}


bool checkForRealWord(const Sent& a,const Sent& b) {
  if(checkSentForRealWord(a) && checkSentForRealWord(b)) return 1;
  return 0;
}


int main(int argc,char **argv) {
  bool inv=0,noemp=0,counts=0,checkrealwords=0, soft=0;
  unsigned int maxlen=256;

  for(int i=1;i<argc;++i) {
    string s(argv[i]);
    if(s=="-inv") inv=1;
    else if(s=="-noemp")  noemp=1;
    else if(s=="-realwords") checkrealwords=1;
    else if(s=="-soft") soft=1;
    else if(s=="-c") counts=1;
    else if(s=="-h") {
      cerr<<"usage: "<<argv[0]<<"  [options] \n\n"
	"filter bilingual text (source and target separated with '#')\n"
	" check number of opening/closing parentheses, sentence lengths\n"
	"\noptions:\n"
	"\t-c      -- source and target sentence start with a sentence count\n"
	"\t-realwords -- keep only sentences that contain at least one word consisting of characters (a-z,A-Z) only\n"
	"\t-soft   -- simple filtering, just remove clear errors\n"
	"\t-inv    -- invert decision\n"
	"\t-noemp  -- print all input sentences (no filtering)\n"
	"\tint     -- maximum sentence length (default: 256)\n"
	"\n";
      return 1;
    }
    else if( isdigit(argv[i][0]) )
      maxlen=atoi(argv[i]);
  }


  cerr<<argv[0]<<": maximum sentence length: " << maxlen << endl;
  string line;
  int lineNumber=0,linePrinted=0,lineWrong=0;
  Sent t1,t2;
  double fcount=1.0,ecount=1.0;
  while( getline(cin,line) ) {
    t1.clear();t2.clear();
    if( (++lineNumber%100000)==0 )
      cerr<<argv[0]<<" processed " << lineNumber<< " lines.\n";
    istringstream eingabe(line); 
    string s;
    bool hashread=0;
    bool toomanyhashes=0,parenwrong=0;
    int nParen=0,t1sx=0,t2sx=0;

    if(counts) eingabe>>fcount;
    while(eingabe>>s) {
      if( s=="{" )
	nParen++;
      if( s=="#"||s=="###" ) {
	if (hashread==1) {
	  cerr<<argv[0]<<": too many hashes in line " << lineNumber << endl;
	  toomanyhashes=1;
	}
	hashread=1;
	if( nParen!=0 ) {
	  parenwrong=1;
	  cerr<<argv[0]<<": wrong number of parens on left side: "<<nParen
	      <<" in line "<<lineNumber<<endl;
	}
	if(counts) {
	  eingabe>>ecount;
	  if(fcount!=ecount)
	    cerr<<argv[0]<<":  counts mismatch: "<<fcount<<" - "<<ecount<<"\n";
	}
      }
      else
	if( hashread ) {
	  t2.push_back(s);
	  if( nParen==0 ) ++t2sx;
	}
	else {
	  t1.push_back(s);
	  if( nParen==0 )
	    ++t1sx;
	}
      if( s=="}" )
	--nParen;
    }
    //unsigned int t1s=t1.size(),t2s=t2.size();
    unsigned int t1s=t1sx,t2s=t2sx;
    if( nParen!=0 ) {
      parenwrong=1;
      cerr<<argv[0]<<": wrong number of parens on rightside side: "<<nParen
	  <<" in line "<<lineNumber<<'\n';
    }
    bool hasRealWord=(!checkrealwords || checkForRealWord(t1,t2));

#ifdef DEBUG
    if(!hasRealWord) {
      cerr<<"no real word: ";
      printPair(cerr,t1,t2,counts,fcount);
    }
#endif

    if( noemp ) printPair(cout,t1,t2,counts,fcount);
    else if( inv^(toomanyhashes==0 && 
		  parenwrong==0 &&
		  hasRealWord==1 && 
		  t1s &&
		  t2s &&
		  (!(6*t1s<t2s || t1s>6*t2s) || soft ) && 
		  (t1s<3||t2s<3||(t1s<2.2*t2s&&t2s<2.2*t1s) || soft ) &&
		  (t1s<10||t2s<10||(t1s<1.8*t2s&&t2s<1.8*t1s) || soft ) &&
		  t1s<maxlen &&
		  t2s<maxlen ))
      {
	printPair(cout,t1,t2,counts,fcount);
	++linePrinted;
      }
    if( toomanyhashes==1 || parenwrong==1 )
      ++lineWrong;
  }
  cerr<<argv[0]<<" processed "<<lineNumber<<" lines.\n";
  if( lineWrong )
    cerr<<argv[0]<<": "<<lineWrong<<" lines contain errors.\n";
  if( lineNumber )
    cerr<<argv[0]<<": printed " <<linePrinted<<" of "<<lineNumber<<" which is "
	<<linePrinted/(1.0*lineNumber)<<'\n';
}
