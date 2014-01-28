#ifndef MYSTRING_H
#define MYSTRING_H

#include <string>

struct MyString {
	MyString(char* pString) {thisString = pString;}
	std::string thisString;
};

#endif
