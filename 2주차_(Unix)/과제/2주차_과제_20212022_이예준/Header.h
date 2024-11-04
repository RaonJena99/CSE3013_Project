#ifndef __HEADER_H__ //중복 정의를 방지하기 위한 매크로
#define __HEADER_H__

#include <stdio.h> //표준 입출력 라이브러리

int arr[10]; //페이지를 구성하는 각 숫자의 개수를 저장하는 배열
int Testcase, Page, tmp; // 1.테스트 케이스 // 2. 입력받은 페이지 // 3. 임시 저장 값
int digit, ten, quot, remind;  // 4. 입력받은 수의 자릿수 // 5. 10의 거듭제곱을 저장 // 6. 몫 저장 // 7. 나머지 저장

void Init(); //arr 배열 초기화
void Count(); // 1 ~ Page 까지 숫자 개수 계산
void Print(); // 각 숫자의 개수 출력

#endif
