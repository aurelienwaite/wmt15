/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include "IOfile.hh"
#include <algorithm>
#include <cstdlib>
#include <cstring>

using namespace std;

const int numOfFiles=6;

int main(int argc,char**argv)
{
  string s=argv[argc-1];
  if( argc < 3 || s=="-h" )
    {
      cerr << "USAGE:\n"<<argv[0]<<" src.train tar.train src.dev tar.dev src.test tar.test\n";
      exit(1);
    }

  string fName[numOfFiles]={"/dev/null","/dev/null","/dev/null","/dev/null","/dev/null","/dev/null"};
  enum {SRC_TR,TAR_TR,SRC_DV,TAR_DV,SRC_TE,TAR_TE};

  for(int i=1;i<argc;i++) {
    static int fileIndex=0;
    s=argv[i];
    if( fName[fileIndex]=="/dev/null" )
      fName[fileIndex++]=s;
  }

  Ifile *f[numOfFiles];
  for(int i=0; i<numOfFiles; i++ ){
    f[i]=new Ifile(fName[i].c_str());
    assert(f[i]);
  }

  int numOfLines[numOfFiles];
  int numOfWords[numOfFiles];
  int numOfWordsNoPunc[numOfFiles];
  map<string,int> voc[numOfFiles];
  set<string> voc_set[numOfFiles];

  for(int i=0; i<numOfFiles; i++){
    string t,s;
    numOfLines[i]=0;
    numOfWords[i]=0;
    numOfWordsNoPunc[i]=0;
    while( getline(*f[i],t) ) {
      numOfLines[i]++;
      istringstream inLine(t.c_str());
      
      while ( inLine >> s) {
	//	voc[i].insert(s);
	voc[i][s]++;
	voc_set[i].insert(s);
	if (!iscntrl(s[0]))
	  numOfWords[i]++;
	else cout << s << endl;
	if (s.size()>1) numOfWordsNoPunc[i]++;
	else if (!ispunct(s[0])) numOfWordsNoPunc[i]++;
      }
    }
  }

  int single_s=0, single_t=0;
  for(map<string,int>::const_iterator i=voc[SRC_TR].begin();i!=voc[SRC_TR].end();++i)
    if( i->second ==1 ) single_s++;
  for(map<string,int>::const_iterator i=voc[TAR_TR].begin();i!=voc[TAR_TR].end();++i)
    if( i->second ==1 ) single_t++;

  set<string> result1,result2;
  int unknown_s_dev_voc=0, unknown_t_dev_voc=0; // number of unknown voc. words
  int unknown_s_tes_voc=0, unknown_t_tes_voc=0;
  int unknown_s_dev_run=0, unknown_t_dev_run=0; // number of unknown running words
  int unknown_s_tes_run=0, unknown_t_tes_run=0;
  insert_iterator<set<string> > res1(result1,result1.begin()), res2(result2,result2.begin());  
  set_difference(voc_set[SRC_DV].begin(), voc_set[SRC_DV].end(), voc_set[SRC_TR].begin(), voc_set[SRC_TR].end(), res1); 
  set_difference(voc_set[TAR_DV].begin(), voc_set[TAR_DV].end(), voc_set[TAR_TR].begin(), voc_set[TAR_TR].end(), res2); 
  unknown_s_dev_voc =  result1.size();
  unknown_t_dev_voc =  result2.size();
  for (set<string>::const_iterator itr=result1.begin(); itr!=result1.end(); itr++)
    unknown_s_dev_run += voc[SRC_DV][*itr];
  for (set<string>::const_iterator itr=result2.begin(); itr!=result2.end(); itr++)
    unknown_t_dev_run += voc[TAR_DV][*itr];

  result1.clear(); result2.clear();
  set_difference(voc_set[SRC_TE].begin(), voc_set[SRC_TE].end(), voc_set[SRC_TR].begin(), voc_set[SRC_TR].end(), res1);
  set_difference(voc_set[TAR_TE].begin(), voc_set[TAR_TE].end(), voc_set[TAR_TR].begin(), voc_set[TAR_TR].end(), res2);
  unknown_s_tes_voc =  result1.size();
  unknown_t_tes_voc =  result2.size();
  for (set<string>::const_iterator itr=result1.begin(); itr!=result1.end(); itr++)
    unknown_s_tes_run += voc[SRC_TE][*itr];
  for (set<string>::const_iterator itr=result2.begin(); itr!=result2.end(); itr++)
    unknown_t_tes_run += voc[TAR_TE][*itr];

  cout << "\\begin{table}[htbd]" << endl;
  cout << "\\begin{center}" << endl;
  cout << "\\begin{tabular}{|lr|c|c|} \\hline" << endl;
  cout << "       &              &  \\SRC    &   \\TGT      \\\\ \\hline \\hline" << endl;

  if (numOfLines[SRC_TR]==numOfLines[TAR_TR])
    cout << "Train: & Sentences    & \\multicolumn{2}{c|}{" << numOfLines[SRC_TR] << "}\\\\ \\hline" << endl;
  else cout << "Train: & Sentences    & " << numOfLines[SRC_TR] << " & " << numOfLines[TAR_TR] << "\\\\ \\hline" << endl;
  cout << "       & Running Words& " << numOfWords[SRC_TR] << " & " << numOfWords[TAR_TR] << "\\\\ \\hline" << endl;
  cout << "       & Running Words without Punct. Marks& " << numOfWordsNoPunc[SRC_TR] << " & " << numOfWordsNoPunc[TAR_TR] << "\\\\ \\hline" << endl;
  cout << "       & Vocabulary   & " << voc[SRC_TR].size() << " & " << voc[TAR_TR].size() << "\\\\ \\hline" << endl;
  cout << "       & Singletons   & " <<   single_s         << " & " <<      single_t      << "\\\\ \\hline" << endl;
  cout << " \\hline" << endl;
  if (numOfLines[SRC_DV]==numOfLines[TAR_DV])
    cout << "Dev: & Sentences    & \\multicolumn{2}{c|}{" << numOfLines[SRC_DV] << "}\\\\ \\hline" << endl;
  else cout << "Dev.:  & Sentences    & " << numOfLines[SRC_DV] << " & " << numOfLines[TAR_DV] << "\\\\ \\hline" << endl;
  cout << "       & Running Words& " << numOfWords[SRC_DV] << " & " << numOfWords[TAR_DV] << "\\\\ \\hline" << endl;
  cout << "       & Running Words without Punct. Marks& " << numOfWordsNoPunc[SRC_DV] << " & " << numOfWordsNoPunc[TAR_DV] << "\\\\ \\hline" << endl;
  cout << "       & Vocabulary   & " << voc[SRC_DV].size() << " & " << voc[TAR_DV].size() << "\\\\ \\hline" << endl;
  cout << "       & OOVs (running words) & " << unknown_s_dev_run      << " & " <<  unknown_t_dev_run     << "\\\\ \\hline" << endl;
  cout << "       & OOVs (in voc.) & " << unknown_s_dev_voc      << " & " <<  unknown_t_dev_voc     << "\\\\ \\hline" << endl;
  cout << " \\hline" << endl;
  if (numOfLines[SRC_TE]==numOfLines[TAR_TE])
    cout << "Test: & Sentences    & \\multicolumn{2}{c|}{" << numOfLines[SRC_TE] << "}\\\\ \\hline" << endl;
  else cout << "Test   & Sentences    & " << numOfLines[SRC_TE] << " & " << numOfLines[TAR_TE] << "\\\\ \\hline" << endl;
  cout << "       & Running Words& " << numOfWords[SRC_TE] << " & " << numOfWords[TAR_TE] << "\\\\ \\hline" << endl;
  cout << "       & Running Words without Punct. Marks& " << numOfWordsNoPunc[SRC_TE] << " & " << numOfWordsNoPunc[TAR_TE] << "\\\\ \\hline" << endl;
  cout << "       & Vocabulary   & " << voc[SRC_TE].size() << " & " << voc[TAR_TE].size() << "\\\\ \\hline" << endl;
  cout << "       & OOVs (running words) & " <<    unknown_s_tes_run   << " & " << unknown_t_tes_run      << "\\\\ \\hline" << endl;
  cout << "       & OOVs (in voc.) & " <<    unknown_s_tes_voc   << " & " << unknown_t_tes_voc      << "\\\\ \\hline" << endl;

  cout << "\\end{tabular}" << endl;
  cout << "\\caption{ Corpus Statistics }\\label{stats}" << endl;
  cout << "\\end{center}" << endl;
  cout << "\\end{table}" << endl;

}
