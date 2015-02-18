/* ---------------------------------------------------------------- */
/* Copyright 2004 (c) by RWTH Aachen - Lehrstuhl fuer Informatik VI */
/* ---------------------------------------------------------------- */
#include "Tokenizer.h"
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "iconv.h"

Tokenizer::Tokenizer() : StringMap() {};

Tokenizer::Tokenizer(const std::string& fname,int verb) : StringMap() {
  readFromFile(fname,verb);}

void read(std::istream&il,std::vector<int>&wordChars)
{
  int l=0;
  std::string s;
  int idx=0;
  while(il>>s)
    {
      idx++;
      //      cerr<<"read: idx: "<<idx<<"  "<<s<<"\n";
      if( s=="isalpha" )
	{
	  for(int i=0;i<256;++i)
	    if( isalpha(i) )
	      {
		wordChars[i]=(idx==1)?2:1;
		++l;
	      }
	}
      else if( s=="isdigit" )
	{
	  for(int i=0;i<256;++i)
	    if( isdigit(i) )
	      {
		wordChars[i]=(idx==1)?2:1;
		++l;
	      }
	}		    
      else if( s.size() && isdigit(s[0]) )
	{
	  size_t i=atoi(s.c_str());
	  if(i>=wordChars.size()) {
	    std::cerr<<"ERROR: wordChars too small: number: "<<i<<" size: "<<wordChars.size()<<"  -> ignore \n";
	    continue;
	  }
	  wordChars[i]=(idx==1)?2:1;
	  ++l;
	}
      else if( s.size()==1 )
	{
	  unsigned char c=s[0];
	  size_t i=static_cast<size_t>(c);
	  if(i>=wordChars.size()) {
	    std::cerr<<"ERROR: Tokenizer::read - wordChars too small: char: "<<i<<" size: "<<wordChars.size()<<"  -> ignore \n";
	    continue;
	  }
	  wordChars[i]=(idx==1)?2:1;
	  ++l;
	}
      else
	std::cerr << "ERROR in tokenizer: what is '" << s << "'.\n";
    }
  //  cerr << "INFO: " << l << " characters in tokenizer.\n";
}

void Tokenizer::readFromStream(std::istream&normal,int) {
  //  cerr<<"start Tokenizer::readFromStream\n";

  std::string l,s;
  //cerr << "HIER: " << l << endl;
  
  inputCode = "";
  outputCode = "";


  while( getline(normal,l) )
    {
      //      cerr<<"next line: "<<l<<"\n";      
      if( l[0]=='#' );
      else if( l=="NEW")
	{
	  //cerr << "NEW: " << endl;
	  exceptions.push_back(std::vector<std::string>());
	  wordChars.push_back(std::vector<int>(256,0));
	  wordChars2.push_back(std::vector<int>(256,0));
	  wordChars3.push_back(std::vector<int>(256,0));
	  wordChars2x.push_back("");
	  wordChars3x.push_back("");
	}
      else
	{
	  std::istringstream il(l);
	  il>>s;
	  if( s==std::string("EXCEPTION") )
	    {
	      while(il>>s)
		exceptions.back().push_back(s);
	      std::sort(exceptions.back().begin(),exceptions.back().end());
	      //cerr << "INFO: read " << exceptions.back().size() << " exceptions.\n";
	    }
	  else if(s==std::string("LETTERS") )
	    read(il,wordChars.back());
	  else if(s==std::string("LETTERS2"))
	    {
	      if(il>>s)
		{
		  wordChars2x.back()=s;
		  read(il,wordChars2.back());
		}
	    }
	  else if(s==std::string("LETTERS3"))
	    {
	      if( il>>s )
		{
		  wordChars3x.back()=s;
		  read(il,wordChars3.back());
		}
	    }
	  else if(s==std::string("PREREGEXMAP")) {
	    il>>s;
	    std::ifstream read_pretokmap(s.c_str());
	    ThePreTokMap.readFromNormal(read_pretokmap);
	  }
	  else if(s==std::string("POSTREGEXMAP")) {
	    il>>s;
	    std::ifstream read_posttokmap(s.c_str());
	    ThePostTokMap.readFromNormal(read_posttokmap);
	  }	
	  else if(s==std::string("ENCODING")) {
	    il>>s;
	    if(s==std::string("from")) {
	      il>>s;
	      inputCode = s;
	      outputCode = s;
	    }
	    else if(s==std::string("to")) {
	      il>>s;
	      outputCode = s;	      
	    }
	    else
	      std::cerr << "ERROR: reading tokenizer: ENCODING'" << s << "' '" << l << "'.\n";
	  }
	  else 
	    std::cerr << "ERROR: reading tokenizer '" << s << "' '" << l << "'.\n";
	}
    }
  //  std::cerr<<"finished Tokenizer::readFromStream\n";
}

void applyNormal_tok(const std::string&in,std::string&out,const std::vector<std::string>&exceptions,const std::vector<int>&ok,const std::vector<int>&ok2,const std::vector<int>&ok3,const std::string&s2,const std::string&s3)
{
  out="";
  std::istringstream il(in.c_str());
  std::string s;
  short readParen=0;
  while(il>>s)
    {
      if( s=="{"||readParen )
	{
	  if( s=="{" )
	    readParen++;
	  if( s=="}" )
	    readParen--;
	  out+=s+' ';
	  continue;
	}
      bool okFlag=ok[(unsigned char)(s[0])]; // ||(s[0]=='&'); -- wichtig bool
      bool ok2Flag=ok2[(unsigned char)(s[0])]; // wichtig bool      
      //      bool htmlFlag=0;
      short exceptionFound=0;

      //      if( exceptions.size() ) {
      //	exceptionFound=std::binary_search(exceptions.begin(),exceptions.end(),s);
      //
      //	if(!exceptionFound) {
      //	  // check for street names (German):
      //	  //  -> end with "str."
      //	  
      //	  std::string strName="    ";
      //	  for(unsigned int i=1; i<=4; i++)
      //	    strName[4-i] = s[s.length()-i];
      //	  if(strName=="str.") exceptionFound=1;	 
      //	}
      //      }

      if( exceptions.size() ) {
	std::string sCopy=s;
	bool abbFlag=1;
	for(unsigned int i=0; i<s.length() && abbFlag; i++)
	  abbFlag = abbFlag && (ok[(unsigned char)s[i]] || (s[i]=='.'));

	if(s[s.length()-1] == '.' ) {
	  s.resize(s.length()-1);
	  if(s.find('.') < s.length() && abbFlag)
	    exceptionFound=1;
	  else
	    exceptionFound=std::binary_search(exceptions.begin(),exceptions.end(),s);
	}
	else
	  exceptionFound=std::binary_search(exceptions.begin(),exceptions.end(),s);
	
	if(!exceptionFound) {
      	  // check for street names (German):
      	  //  -> end with "str."       	  
      	  std::string strName="    ";
      	  for(unsigned int i=1; i<=4; i++)
      	    strName[4-i] = s[s.length()-i];
      	  if(strName=="str.") exceptionFound=1;	 
	}
	s=sCopy;
      }


      if( exceptionFound==0 )
	{
	  bool notFound=0,foundNeeded=0;
	  for(unsigned int i=0;i<s.length();++i)
	    {
	      if( ok2[s[i]]==0 )
		notFound=1;
	      if( ok2[s[i]]==2 )
		foundNeeded=1;
	    }
	  if( foundNeeded==0 )
	    notFound=1;
	  if( notFound==0 )
	    exceptionFound=2;
	}
      if( exceptionFound==0 )
	{
	  bool notFound=0,foundNeeded=0;
	  for(unsigned int i=0;i<s.length();++i)
	    {
	      if( ok3[s[i]]==0 )
		notFound=1;
	      if( ok3[s[i]]==2 )
		foundNeeded=1;
	    }
	  if( foundNeeded==0 )
	    notFound=1;
	  if( notFound==0 )
	    exceptionFound=3;
	}
		
      if( exceptionFound==1||(exceptionFound==2&&s2=="NONE")||(exceptionFound==3&&s3=="NONE") )
	out+=s;
      else if( exceptionFound==2 ){
	//	int len = s.length()-1;
	//	char s_len = s[len];
	//	if( ( (s[len]=='.' && len==4 )||s[len]==','||s[len]==';'||s[len]==':'||s[len]=='?'||s[len]=='!') && 
	//	    ok2[((unsigned char)s[len-1])]==2)
	//	  {
	//	    s.resize(len);
	//	    out+=s2+" { "+ s +" } "+s_len;
	//	  }
	//	else if( s[0]==',' ){
	//	  s[0] = ' ';
	//	  out+= ", "+s2+" {"+s+" }";
	//	}
	//	else
	  out+=s2+" { "+s+" }";
      }
      else if( exceptionFound==3 )
	out+=s3+" { "+s+" }";
      else
	for(unsigned int i=0;i<s.length();++i)
	  {
	    unsigned char si=s[i];
	    //	    if( htmlFlag==1 )
	    //	      {
	    //		out+=si;
	    //		if( si==';' )
	    //		  htmlFlag=0;
	    //	      }
	    //	    else
	    //	      {
	    //		if( si=='&' )
	    //		  {
	    //		    out+=si;
	    //		    htmlFlag=1;
	    //		    okFlag =bool(ok[(unsigned char)('a')]);
	    //		    ok2Flag=bool(ok2[(unsigned char)('a')]);
	    //		  }
	    //		else
	    //		  {
	    if( bool(ok[(unsigned char)(si)])==okFlag ) {
	      out+=si;
	      //	      ok2Flag=bool(ok2[(unsigned char)(si)]);
	    }
	    else if(ok2Flag && bool(ok2[(unsigned char)(si)])==ok2Flag && ( si!='.' || i!=s.length()-1 ))
	      out+=si;
	    else
	      {
		out+=' ';
		out+=si;
		okFlag =bool(ok[(unsigned char)(si)]);
		ok2Flag=bool(ok2[(unsigned char)(si)]);
	      }
	  }
      //    }
      //}
      out+=' ';
    }
}

void Tokenizer::applyNormal(const std::string&in,std::string&out)const
{
  std::string str1="",str2="";

  if(inputCode == "" )
    ThePreTokMap.applyNormal(in,str1);
  else {
    iconv_t Uni2Latin = iconv_open ("LATIN1", inputCode.c_str());
    if( Uni2Latin == (iconv_t) -1 )
      std::cerr << " The conversion from " << inputCode <<		\
	" to LATIN1 is not supported by the implementation." << std::endl;
    std::istringstream inStr(in);
    std::string token;
    while(inStr >> token) {
      char target[2000];
      char *tar=target;      
      char source[2000];
      char *src=source;
      size_t sl,tl;
      strcpy(source,token.c_str());
      sl = tl = strlen(token.c_str())+1;
      if( iconv(Uni2Latin, &src, &sl, &tar, &tl) > 0 )
	str1 += " ORG_CODING { "+token+" }";
      else {
	std::string res(target);
	str1 += " "+res;
      }
    }
    iconv_close(Uni2Latin);
    ThePreTokMap.applyNormal(str1,str2);
    str1=str2;
  }

  for(unsigned int i=0;i<exceptions.size();++i) {
    str2="";
    applyNormal_tok(str1,str2,exceptions[i],wordChars[i],wordChars2[i],wordChars3[i],wordChars2x[i],wordChars3x[i]);
    str1=str2;
  }
  
  if(outputCode == "" || outputCode==std::string("LATIN1"))
    ThePostTokMap.applyNormal(str1,out);
  else {
    str1="";
    iconv_t Latin2Uni = iconv_open ( outputCode.c_str() , "LATIN1"); 
    if( Latin2Uni == (iconv_t) -1 )
      std::cerr << " The conversion from LATIN1 to " << outputCode <<	\
	" is not supported by the implementation." << std::endl;
    std::istringstream inStr(str2);
    std::string token;
    while(inStr >> token)
      if( token == "ORG_CODING" ) {
	inStr >> token;	
	if(token!="{") {
	  str1 += " ORG_CODING "+token;
	  std::cerr << "ERROR: inconsistency between tokenizer and regexmap files!\n";
	  break;
	}
	str1 += " ";
	while(inStr >> token) {
	  if(token=="}")
	    break;
	  else
	    str1 += token;
	}
      }
      else {
	char target[6000];
	char *tar=target;      
	char source[2000];
	char *src=source;
	size_t sl,tl;
	strcpy(source,token.c_str());
	
	sl = strlen(token.c_str())+1;
	tl = 3*sl;
	if( iconv(Latin2Uni, &src, &sl, &tar, &tl) > 0 )	  
	  str1 += " "+token;
	else {
	  std::string res(target);	
	  str1 += " "+res;
	}
      }
    iconv_close(Latin2Uni);
    ThePostTokMap.applyNormal(str1,out);
  }
}
