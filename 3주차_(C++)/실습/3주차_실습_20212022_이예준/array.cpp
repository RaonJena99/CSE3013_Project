#include "Array.h"
#include <cstdlib>
#include<iostream>
using namespace std;
Array::Array(int size)
{
	if(size<0) cout<<"Error: input negative\n";
	else{
		data = new int[size];
		len = size;
	}
}
 Array::~Array()
{
	delete[] data;
	len=0;
}
int Array::length() const
{
	return len;
}

// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 오버로딩이다
int& Array::operator[](int i) // 배열에 원소 삽입
{
	static int tmp;

	// 배열의 인덱스가 범위 내에 있으면 값 리턴, 그렇지 않으면 에러메세지 출력하고 tmp리턴
	if(i >= 0 && i < len){
		return data[i];
	}
	else{
		cout<<"Array bound error!\n";
		return tmp;
	}
}

int Array::operator[](int i) const // 배열의 원소값 반환
{
	//배열의 인덱스가 범위 내에 잇으면 값을 리턴, 그렇지 않으면 에러메세지 출력하고 0을 리턴
	if(i >= 0 && i<len){
		return data[i];
	}
	else{
		cout<<"Array bound error!\n";
		return 0;
	}
}
void Array::print() //배열의 모든 내용을 출력해주는 함수
{
	int i;
	cout<<"[";
	for(i=0;i<len;i++)
		cout<<" "<<data[i];
	cout<<"]";
	cout<<endl;
}
