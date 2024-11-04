#include <stdio.h>

main(void)
{
  int i;
  double num;

  for(i=0; i<5; i++){
    num=i/(double)2 + i;
    printf("num is %f \n", num);
  }
}