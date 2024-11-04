#include "20212022.h"

int main(){
  fp = fopen("20212022.maz", "wt");

  printf("Width: ");
  scanf("%d",&Width);
  printf("Height: ");
  scanf("%d",&Height);

  // 미로 및 집합 초기화
  Init_Maze();
  // 미로 생성
  Make_Maze();
  // 미로 저장
  Save_Maze();
  fclose(fp);
  // 미로 및 집합 동적 해제
  Free_Maze();
  return 0;
}

void Init_Maze(){
  int cnt=1;
  Maze = (char**)malloc(sizeof(char*) * (Height*2 + 1));
  Set = (int**)malloc(sizeof(int*) * Height);
  
  for(i=0;i<(Height*2 + 1);i++)
    Maze[i] = (char*)malloc(sizeof(char)*(Width*2 + 1));
  for(i=0;i<Height;i++)
    Set[i] = (int*)malloc(sizeof(int)*Width);
  

  for(i=0;i<Height;i++)
    for(j=0;j<Width;j++)
      Set[i][j] = cnt++;

  for(i=0;i<(Height*2 + 1);i++){
    for(j=0;j<(Width*2 + 1);j++){
      if(i%2){
        if(j%2) Maze[i][j] = ' ';
        else Maze[i][j] = '|';
      }
      else{
        if(j%2) Maze[i][j] = '-';
        else Maze[i][j] = '+';
      }
    }
  }
}

void Make_Maze(){
  int flag, chk, random, set, idx;
  srand(time(NULL));

  for(i=0;i<Height;i++){

    for(j=0;j<Width-1;j++){
      if(i == Height-1) flag=1;
      else flag = rand()%2;

      if(flag && Set[i][j] != Set[i][j+1]){
        tmp = Set[i][j+1];
        Maze[i*2+1][j*2+2] = ' ';

				for (int k = 0; k < Width; k++) 
					if(Set[i][k] == tmp) Set[i][k] = Set[i][j];
      }
    }

    if(i == Height-1) break; // 마지막 줄에서는 바닥 벽을 없애면 안됨

    random = idx = chk = 0; 
    set = Set[i][0];

    for(j=0;j<Width;j++){

      if(set != Set[i][j]){  //집합이 달라졌을 때
        if(!chk){ // 이전 집합에서 한 개의 방도 연결되지 않았을 때
          random = idx + rand() % (j - idx);
          Set[i+1][random] = Set[i][random];
          Maze[i*2+2][random*2+1] = ' ';
        }
        set = Set[i][j]; 
        chk=0; 
        idx = j;
      }
      
      flag = rand()%2;

      if(flag){
        Set[i+1][j] = Set[i][j];
        Maze[i*2+2][j*2+1] = ' ';
        chk=1;
      }
    }

    if(!chk){
      Set[i+1][Width-1] = Set[i][Width-1];
      Maze[i*2+2][(Width-1)*2+1] = ' ';
    }
  }
}

void Save_Maze(){
  for(i=0;i<(Height*2 + 1);i++)
    fprintf(fp,"%s\n",Maze[i]);
}

void Free_Maze(){
  for(i=0;i<(Height*2 + 1);i++)
    free(Maze[i]);
  for(i=0;i<Height;i++)
    free(Set[i]);

  free(Maze);
  free(Set);
}