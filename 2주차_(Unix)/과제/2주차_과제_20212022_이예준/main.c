#include "Header.h"

int main(){
  scanf("%d",&Testcase); // 테스트 케이스 개수 입력
  while(Testcase--){
    Init(); // 배열 초기화 함수 호출
    Count(); // 숫자 개수 계산 함수 호출
    Print(); // 결과값 출력 함수 호출
  }
  return 0;
}