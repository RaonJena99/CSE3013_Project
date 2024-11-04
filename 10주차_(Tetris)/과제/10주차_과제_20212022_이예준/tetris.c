#include "tetris.h"

static struct sigaction act, oact;
int B,count;
time_t start, end, play_start, play_end;
double duration, Tree_time, Play_time;
long long Tree_space;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	rank_num=0;
	createRankList();

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	writeRankFile();
	head=NULL;
	free(recRoot);
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;

	recRoot = (RecNode*)malloc(sizeof(RecNode));
  recRoot->level = recRoot->accumulatedScore = 0;
  memcpy(recRoot->recfield, field, sizeof(field));
	recommend(recRoot);

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* next, next block을 보여주는 공간의 태두리를 그린다.*/
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j] == 1){
				if((i+blockY)<0||(i+blockY)>=HEIGHT) return 0; // Height check
				if((j+blockX)<0||(j+blockX)>=WIDTH) return 0; // Width check
				if(f[i+blockY][j+blockX]) return 0; // Block check
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
  // prev block info.
	int back_R=blockRotate, back_X=blockX, back_Y=blockY;
	switch(command){
	case KEY_UP:
			back_R=(back_R+3)%4;
		break;
	case KEY_DOWN:
			back_Y--;
		break;
	case KEY_RIGHT:
			back_X--;
		break;
	case KEY_LEFT:
			back_X++;
		break;
	default:
		break;
	}

	//delete prev shadow
	DrawField();

	//delete prev block
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++){
			if(block[currentBlock][back_R][i][j]==1 && i+back_Y>=0){
				move(i+back_Y+1,j+back_X+1);
				printw(".");
			}
		}

	//draw cur block & shadow & recommend
	DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
	move(HEIGHT,WIDTH+10);
}

void BlockDown(int sig){
	timed_out = 0;
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)){
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);	
	}
	else{
		if(blockY==-1) gameOver=1;
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=nextBlock[2];
		nextBlock[2]=rand()%7;

    recRoot->level = recRoot->accumulatedScore = 0;
    memcpy(recRoot->recfield, field, sizeof(field));
		recommend(recRoot);

		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH/2-2;
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
	}
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int touched=0;

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
                if(i+blockY+1 == HEIGHT || f[i+blockY+1][j+blockX]) touched++ ; 
				f[i+blockY][j+blockX] = 1;
			}
		}
	}

	return touched*10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	int flag,del_line=0;
	for(int i=0;i<HEIGHT;i++){
		flag=1;
		for(int j=0;j<WIDTH;j++)
			if(!f[i][j]) flag=0;
		if(flag){
			del_line++;
			for(int r=i;r>0;r--) for(int v=0;v<WIDTH;v++) f[r][v] = f[r-1][v];
		}
	}

	return (del_line*del_line)*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	while(CheckToMove(field, blockID, blockRotate, ++y, x)){};
	y--;
	DrawBlock(y,x,blockID,blockRotate,'/');
}

void DrawBlockWithFeatures(int y, int x, int blockID,int blockRotate){
	DrawBlock(y, x, blockID, blockRotate,' ');
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
	DrawShadow(y, x, blockID, blockRotate);
}

void createRankList(){
	head = NULL;
	int rank_score, i;
	char rank_name[NAMELEN+1];

	FILE *fp = fopen("rank.txt", "r");

	if(fp == NULL) return;
	
	if (fscanf(fp,"%d",&rank_num) != EOF) {
		for(i=0;i<rank_num;i++){

			fscanf(fp,"%s %d", rank_name, &rank_score);

			Node *newNode = (Node*)malloc(sizeof(Node));
			newNode->score = rank_score;
			strcpy(newNode->name,rank_name);
			newNode->next = NULL;

			if(head == NULL) head = newNode;
			else{
				Node *list = head;
				while(list->next != NULL)
					list = list->next;
				list->next = newNode;
			}
		}
	}
	else printw("Cannot read numbers from the file.\n");

	fclose(fp);
}

void rank(){
	int ch;
	Node *list;
	clear();

	printw("1. List ranks from X to Y\n");
	printw("2. List ranks by a specific name\n");
	printw("3. Delete a specific rank\n");

	ch = wgetch(stdscr);

	if (ch == '1') {
		int X = 1, Y = rank_num;
		list = head;

		echo();
		printw("X: ");
		scanw("%d", &X);
    printw("Y: ");
    scanw("%d", &Y);
		noecho();

		move(5,7);
		printw("name");
		move(5,18);
		printw("|   score\n------------------------------\n");

		if(X > Y) printw("\nsearch failure: no rank in the list\n");
		else{
			int cur=0;

			for(int count = 1; list != NULL; list = list->next, count++){
				if(X <= count && count <= Y){
					printw("%s",list->name);
					move(7+cur,18);
					printw("| %d\n",list->score);
					cur++;
				}
			}
		}
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;
		list = head;

		echo();
		printw("input the name: ");
		scanw("%s", str);
		noecho();

		move(5,7);
		printw("name");
		move(5,18);
		printw("|   score\n------------------------------\n");

		int cur=0;
		for(; list != NULL; list = list->next){
			if(!strcmp(list->name,str)){
				printw("%s",list->name);
				move(7+cur,18);
				printw("| %d\n",list->score);
				cur++;
				check=1;
			}
		}
		if(!check) printw("\nsearch failure: no name in the list\n");
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num=0;
		list = head;

		echo();
		printw("input the rank: ");
		scanw("%d", &num);
		noecho();

		if(num<=0 || num>rank_num) printw("\nsearch failure: the rank not in the list\n");
		else{
			Node *prev = NULL;
			if(num==1){
				prev=list;
				head=list->next;
			}
			else{
				for(int count=1; list != NULL; list = list->next,count++){
					if(count==num){
						prev->next=list->next;
						prev=list;
						break;
					}
					prev=list;
				}
			}
			free(prev);
			rank_num--;
			printw("\nresult: the rank deleted\n");
		}
	}
	getch();
}

void writeRankFile(){
	FILE *fp = fopen("rank.txt", "wt");

	fprintf(fp,"%d\n",rank_num);

	Node *list = head;
	for(; list != NULL; list = list->next){
		fprintf(fp,"%s %d\n",list->name, list->score);
	}

	fclose(fp);
}

void newRank(int score){
	char str[NAMELEN+1];
	int i, j;
	clear();

	printw("your name: ");
	echo();
	scanw("%s", str);
	noecho();

	Node *newNode=(Node*)malloc(sizeof(Node));
	newNode->score = score;
	strcpy(newNode->name,str);
	newNode->next = NULL;

	if(head == NULL) head = newNode;
	else{
		Node *cur = head;
		Node *prev = NULL;
		//첫번째 점수보다 높을 때
		if(cur->score <= newNode->score){
			newNode->next = cur;
			head = newNode;
		}
		else{
			while(cur != NULL && cur->score >= newNode->score){
				prev = cur;
				cur = cur->next;
			}
			//중간에 위치를 찾았을 때
			if(cur != NULL){
				newNode->next = cur;
				prev->next = newNode;
			}
			//끝까지 갔을 때
			else prev->next = newNode;
		}
	}
	rank_num++;
	
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode *root) {
  int max = 0;
	int child_num=0;

	root->c = (RecNode**)malloc(sizeof(RecNode*)*CHILDREN_MAX);

  for (int r = 0; r < NUM_OF_ROTATE; r++) {
    for (int x = -2; x < WIDTH; x++) {
      if (!CheckToMove(root->recfield, nextBlock[root->level], r, 0, x)) continue;
      RecNode *child = (RecNode*)malloc(sizeof(RecNode));
			root->c[child_num] = child;
      child->level = root->level + 1;
      child->recBlockY = 0;
			memcpy(child->recfield, root->recfield, sizeof(root->recfield));
              
      while (CheckToMove(child->recfield, nextBlock[root->level], r, child->recBlockY + 1, x))
        child->recBlockY++;
        
      child->accumulatedScore = root->accumulatedScore;
			child->accumulatedScore += AddBlockToField(child->recfield, nextBlock[root->level], r,child->recBlockY, x);
			child->accumulatedScore += DeleteLine(child->recfield);

			if (child->level < VISIBLE_BLOCKS) child->accumulatedScore += recommend(child);
      
			if (child->accumulatedScore >= max){
				if(child->level == 1){
          recommendR = r;
					recommendY = child->recBlockY;
					recommendX = x;
					max = child->accumulatedScore;
				}
				else max = child->accumulatedScore;
			}
			free(root->c[child_num]);
			child_num++;
    }
	}
  return max;
}
typedef struct {
    int score;
    RecNode *node;
} ScoredNode;

int compareNodes(const void *a, const void *b) {
    ScoredNode *nodeA = (ScoredNode *)a;
    ScoredNode *nodeB = (ScoredNode *)b;
    return nodeB->score - nodeA->score;
}

void modified_recommend(RecNode *root){
	Tree_space+=sizeof(*root);

	int max = 0;
  int child_num = 0;
	int queue_size = 0;
  RecNode *queue[BEAM_WIDTH];
  queue[queue_size++] = root;

	// 예측 block 수
  for (int level = 0; level < VISIBLE_BLOCKS; level++) {
    ScoredNode scored_nodes[BEAM_WIDTH * NUM_OF_ROTATE * WIDTH];
    int scored_node_count = 0;

    // 큐에 있는 모든 노드를 확장
    for (int i = 0; i < queue_size; i++) {
      RecNode *parent = queue[i];
    	for (int r = 0; r < NUM_OF_ROTATE; r++) {
      	for (int x = -2; x < WIDTH; x++) {
        	if (!CheckToMove(parent->recfield, nextBlock[level], r, 0, x)) continue;

          RecNode *child = (RecNode *)malloc(sizeof(RecNode));
          memcpy(child->recfield, parent->recfield, sizeof(parent->recfield));
          child->level = parent->level + 1;
					child->p = parent;
					child->recBlockX = x;
          child->recBlockY = 0;
					child->recBlockR = r;

          while (CheckToMove(child->recfield, nextBlock[level], r, child->recBlockY + 1, x)) {
            child->recBlockY++;
          }

          child->accumulatedScore = parent->accumulatedScore;
          child->accumulatedScore += AddBlockToField(child->recfield, nextBlock[level], r, child->recBlockY, x);
          child->accumulatedScore += DeleteLine(child->recfield);

          scored_nodes[scored_node_count].score = child->accumulatedScore;
          scored_nodes[scored_node_count++].node = child;

					Tree_space+=sizeof(*child);
        }
      }
  	}

    // sorting score
    qsort(scored_nodes, scored_node_count, sizeof(ScoredNode), compareNodes);

    //BEAM_WIDTH 개수만큼 남겨 두기
    queue_size = 0;
    for (int i = 0; i < scored_node_count; i++) {
      if(i < BEAM_WIDTH) queue[queue_size++] = scored_nodes[i].node;
			else free(scored_nodes[i].node);
    }
  }

	while(queue[0]->level > 1){
		queue[0]=queue[0]->p;
	}

	recommendR = queue[0]->recBlockR;
	recommendX = queue[0]->recBlockX;
	recommendY = queue[0]->recBlockY;

  // 메모리 해제
  for (int i = 0; i < queue_size; i++) {
    free(queue[i]);
  }
}

void Rec_BlockDown(int sig){
	timed_out = 0;
	if(!CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)){
		gameOver=1;
		return;
	}
	else{
		DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);

		score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX);
		score += DeleteLine(field);
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=nextBlock[2];
		nextBlock[2]=rand()%7;

		recRoot->level = recRoot->accumulatedScore = 0;
		memcpy(recRoot->recfield, field, sizeof(field));

		start = time(NULL);
		modified_recommend(recRoot);
		end = time(NULL);

		duration = (double)difftime(end, start);
		Tree_time += duration;

		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH/2-2;
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
		}
}

void recommendedPlay(){
	Tree_time=0, Play_time=0, Tree_space=0;

	play_start = time(NULL);
		int command;
		clear();
		act.sa_handler = Rec_BlockDown;
		sigaction(SIGALRM,&act,&oact);
		InitTetris();
		do{
			if(timed_out==0){
				alarm(1);
				timed_out=1;
			}

			command = wgetch(stdscr);
			switch(command){
			case 'q':
			case 'Q':
				command = QUIT;
				break;
			default:
				command = NOTHING;
				break;
			}
			
			if(ProcessCommand(command)==QUIT){
				alarm(0);
				DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
				move(HEIGHT/2,WIDTH/2-4);
				printw("Good-bye!!");
				refresh();
				getch();

				return;
			}

			move(25,0);
			printw("[ Total time for play        : %lf seconds ]\n", Play_time);
			printw("[ Total score                : %8d score ]\n", score);
			printw("[ Total time for recommend   : %lf seconds ]\n", Tree_time);
			printw("[ Total memory for recommend : %lld bytes ]\n", Tree_space);
			printw("[ Time efficiency            : %lf ]\n", (double)score/Tree_time);
			printw("[ Space efficiency           : %lf ]\n", (double)score/Tree_space);
		}while(!gameOver);
		play_end = time(NULL);
		Play_time = (double)difftime(play_end, play_start);

		move(25,0);
		printw("[ Total time for play        : %lf seconds ]\n", Play_time);
		printw("[ Total score                : %8d score ]\n", score);
		printw("[ Total time for recommend   : %lf seconds ]\n", Tree_time);
		printw("[ Total memory for recommend : %lld bytes ]\n", Tree_space);
		printw("[ Time efficiency            : %lf ]\n", (double)score/Tree_time);
		printw("[ Space efficiency           : %lf ]\n", (double)score/Tree_space);
		

		alarm(0);
		getch();
		DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
		move(HEIGHT/2,WIDTH/2-4);
		printw("GameOver!!");
		refresh();
		getch();
		newRank(score);
}
