#ifndef _COMMON_H
#define _COMMON_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


string s1tolower(string s){
	stringstream ss;
	ss << (char) toupper(s[0]);
	for(int i=1; i<s.length(); i++)
		if(s[i-1]=='_')
			ss <<  (char) toupper(s[i]);
		else
			ss <<  (char) tolower(s[i]);
	return ss.str();
}

string stolower(string s){
	stringstream ss;
	for(int i=0; i<s.length(); i++)
		ss <<  (char) tolower(s[i]);
	return ss.str();
}
/*
void stoupper(std::string& s){
	std::string::iterator i = s.begin();
	std::string::iterator end = s.end();

	while (i != end) {
		*i = std::toupper((unsigned char)*i);
		++i;
	}
}*/
string stoupper(string s){
	stringstream ss;
	string s2;
	for(int i=0; i<s.length(); i++){
		ss << (char) toupper(s[i]);
	}

	return ss.str();
}

//_COMMON_H
#endif
