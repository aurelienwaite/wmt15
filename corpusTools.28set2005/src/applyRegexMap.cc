/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include "RegexMap.h"
#include "SplitParagraph.hh"
#include "IOfile.hh"

using namespace std;


int main(int argc,char **argv) {
  bool inv=0,zip=0,split=0;
  string ifile,ofile,mapfile;

  for(int i=1;i<argc;++i) {
    string s(argv[i]);
    if(s=="-h" || s=="--help") {
      cerr << "Usage: \n";
      cerr << argv[0] << " [ corpus ] regexmap [ -inv ] " << endl;
      cerr << "\nApplies iteratively the transformation rules in the file <regexmap>\n";
      cerr << "to all the lines in the file <corpus>. If no corpus is given\n";
      cerr << "The format of a rule is:\n";
      cerr << " #from#to#\n";
      cerr << "-> it is mapped the regular expression <from> to <to>\n";
      cerr << "-> the symbol '#' can be chosen arbitrarily\n";
      cerr << "-> one rule has to be in one line of the .regexmap file\n";
      cerr << "Examples: \n"
	"#\\<Name ist \\([A-Z]\\)#Name ist PPROPER_$1#\n"
	"#\\<\\([0-9][0-9]*\\) \\([a-zA-Z_,]*\\) \\([0-9][0-9]*\\) Uhr#$1U $2 $3U Uhr#\n"
	"#\\<Hotel \\([A-Z][a-z]*\\)#Hotel PPROPER_$1#\n";
      return 1;
    }
    else if(s=="-i") ifile=string(argv[++i]);
    else if(s=="-o") ofile=string(argv[++i]);
    else if(s=="-zip") zip=1;
    else if(s=="-inv") {inv=1;std::cerr<<"WARNING: inv will be the identity mapping!\n";}
    //    else if(s=="-dbg") DebugPhraseMap=1;
    else if(s=="-split") split=1;
    else if(ifile=="") ifile=s;
    else if(mapfile=="") mapfile=s;
  }

  if(mapfile=="") {mapfile=ifile;ifile="";}

  if(mapfile=="") {
    cerr<<argv[0]<<" ERROR: no mapfile given!"<<endl;
    return 1;
  }

  StringMap *m=0;
  
  if(split) m=new ParagraphSplitter(mapfile,"",1,9);
  else m=new RegexMap(mapfile,"",1);

  Ifile in(ifile);
  Ofile out(ofile,zip);

  string s,sout;
  while(getline(in,s)){
    if(inv) m->applyInvers(s,sout);
    else m->applyNormal(s,sout);
    out << sout << '\n';
  }
}
