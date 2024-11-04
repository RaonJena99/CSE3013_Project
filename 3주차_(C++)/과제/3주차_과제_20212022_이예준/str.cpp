#include "Str.h"
#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;

// leng 은 string의 길이.
Str::Str(int leng)
{ 
  if(leng<0) cout<<"Error : input negative\n";
  else{
    len = leng;
    str = new char[len];
  }
}

// neyong은 초기화할 내용이 들어감.
Str::Str(char *neyong)
{
  len = strlen(neyong);
  str = new char[len];
  strcpy(str,neyong);
}

// 소멸자.
Str::~Str()
{
  delete[] str;
  len = 0;
} 

// string의 길이를 리턴하는 함수.
int Str::length(void)
{
  return len;
}

// string의 내용을 리턴하는 함수.
char *Str::contents(void)
{
  return str;
}

// a의 내용과 strcmp.
int Str::compare(class Str& a)
{
  if(strcmp(str, a.str)) return 1;
  else return 0;
}

// a의 내용과 strcmp.
int Str::compare(char *a)
{
  if(strcmp(str, a)) return 1;
  else return 0;
} 
    
// string의 값을 대입.
void Str::operator=(char *a)
{
  strcpy(str,a);
  len = strlen(a);
}

// Str의 내용을 대입.
void Str::operator=(class Str& a)
{
  strcpy(str,a.str);
  len = strlen(a.str);
} 