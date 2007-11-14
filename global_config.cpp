#include "global_config.h"
#include <stdio.h>
#include <ctype.h>

Global_Config global_config;

bool Global_Config::get_tocken(FILE *f,tocken_t &T)
{
	int c;
	while((c=fgetc(f))!=EOF) {
		if(c=='.') {
			T.first='.';
			return true;
		}
		else if(c=='=') {
			T.first='=';
			return true;
		}
		else if(c=='\n') {
			line_counter++;
			continue;
		}
		else if(c==' ' || c=='\r' || c=='\t') {
			continue;
		}
		else if(isalpha(c)) {
			T.second="";
			T.second.reserve(32);
			T.second+=(char)c;
			while((c=fgetc(f))!=EOF && isalnum(c)) {
				T.second+=(char)c;
			}
			if(c!=EOF){
				ungetc(c,f);
			}
			T.first=WORD;
			return true;
		}
		else if(isdigit(c) || c=='-') {
			T.second="";
			T.second.reserve(32);
			T.second+=(char)c;
			T.first=INT;
			while((c=fgetc(f))!=EOF && isdigit(c)) {
				T.second+=(char)c;
			}
			if(c=='.') {
				T.second+='.';
				T.first=DOUBLE;
				while((c=fgetc(f))!=EOF && isdigit(c)) {
					T.second+=(char)c;
				}
			}
			if(T.second=="-" || T.second=="." || T.second=="-.") {
				throw HTTP_Error("Illegal charrecters");
			}
			if(c!=EOF) {
				ungetc(c,f);
			}
			return true;
		}
		else if(c=='\"') {
			T.first=STR;
			T.second="";
			T.second.reserve(128);
			for(;;){
				c=fgetc(f);
				if(c=='\\'){
					if((c=fgetc(f))=='\"' ) {
						T.second+='"';
						continue;
					}
					else {
						T.second+='\\';
					}
				}
				if(c==EOF){
					throw HTTP_Error("Unexpected EOF ");
				}
				if(c=='\n') line_counter++;
				if(c=='\"') {
					return true;
				}
				T.second+=(char)c;
			}
		}
		else if(c=='#' || c==';'){
			while((c=fgetc(f))!=EOF) {
				if(c=='\n'){
					line_counter++;
					break;
				}
			}
			if(c==EOF) {
				return false;
			}
				
		}
		else {
			throw HTTP_Error(string("Unexpected charrecter")+(char)c);
		}
	}
	return false;
}

void Global_Config::load(char const *fname)
{
	FILE *f=fopen(fname,"r");
	line_counter=1;
	if(!f) {
		throw HTTP_Error(string("Failed to open file:")+fname);
	}
	tocken_t T;
	string key;
	int state=0;
	try{
		while(get_tocken(f,T) && state != 5) {
			switch(state) {
			case 0: if(T.first != WORD) {
					state=5;
				}else{
					key=T.second;
					state=1;
				}
				break;
			case 1: if(T.first != '.')
					state=5;
				else 
					state=2;
				break;
			case 2: if(T.first!=WORD){
					state=5;
				}else{
					state=3;
					key+='.';
					key+=T.second;
				}
				break;
			case 3: if(T.first!= '=') 
					state=5;
				else
					state=4;
				break;
			case 4: if(T.first==INT) {
					long val=atol(T.second.c_str());
					long_map.insert(pair<string,long>(key,val));
				}
				else if(T.first==DOUBLE) {
					double val=atof(T.second.c_str());
					double_map.insert(pair<string,double>(key,val));
				}
				else if(T.first==STR){
					string_map.insert(pair<string,string>(key,T.second));
				}
				else {
					state=5;
					break;
				}
				state=0;
				break;
			}
		}
		if(state!=0) {
			throw HTTP_Error("Parsing error");
		}
	}
	catch (HTTP_Error &err){
		fclose(f);
		char stmp[32];
		snprintf(stmp,32," at line %d",line_counter);
		throw HTTP_Error(string(err.get())+stmp);
	}
}


void Global_Config::load(int argc,char *argv[],char const *def)
{
	char const *def_file=def;
	int i;
	for(i=1;i<argc;i++) {
		if(strncmp(argv[i],"--config=",9)==0) {
			def_file=argv[i]+9;
			break;
		}
		else if(strcmp(argv[i],"-c")==0 && i+1<argc) {
			def_file=argv[i+1];
			break;
		}
	}
	if(def_file==NULL) {
		throw HTTP_Error("Configuration file not defined");
	}
	load(def_file);
}