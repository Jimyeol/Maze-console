#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

#define WALL 1
#define PASS 0
#define PLAYER1 2
#define HAVE 1
#define NOTHAVE 0
#define UDLR 4
#define MAX 1458
#define ROW 2
#define COL 8

enum
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT,
};

typedef struct PLAYER
{
	int StartX;
	int StartY;
	int EndX;
	int EndY;
	int code;			//개개인의 주어진 말번호
	int X;				//현재 X	(ROW)
	int Y;				//현재 Y	(COL)
}PLAYER;

typedef PLAYER element;

typedef struct StackNode {
	element item;
	struct StackNode *link;
} StackNode;
typedef struct {
	StackNode *top;
} LinkedStackType;

typedef struct mStackNode {			// 미로맵 저장을 위한 스택
   int** data;
   int size;
   struct mStackNode *link;
} mStackNode;

typedef struct {
   mStackNode *top;
} mStackType;

int mis_empty(mStackType *s)
{
   return (s->top == NULL);
}

void mpush(mStackType *s, int** item, int size)					
{
   mStackNode *temp = (mStackNode *)malloc(sizeof(mStackNode));
   if (temp == NULL){
      fprintf(stderr, "메모리 할당에러\n");
      return;
   }
   else{
      temp->data = item;
      temp->size = size;
      temp->link = s->top;
      s->top = temp;
   }
}

int** mpopmap(mStackType *s)
{
   if (mis_empty(s)) {
      fprintf(stderr, "스택이 비어있음\n");
      exit(1);
   }
   else{
      mStackNode *temp = s->top;
      int **item = temp->data;
      return item;
   }
}
int mpopsize(mStackType *s)
{
   if (mis_empty(s)) {
      fprintf(stderr, "스택이 비어있음\n");
      exit(1);
   }
   else{
      mStackNode *temp = s->top;
      int item = temp->size;
      s->top = s->top->link;
      free(temp);
      return item;
   }
}

// 초기화 함수
void init(LinkedStackType *s)
{
	s->top = NULL;
}
// 삽입 함수
void push(LinkedStackType *s, element item)
{
	StackNode *temp=(StackNode *)malloc(sizeof(StackNode));
	if( temp == NULL ){
		fprintf(stderr, "메모리 할당에러\n");
		return;
	}
	else{
		temp->item = item;
		temp->link = s->top;
		s->top = temp;
	}
}
// 공백 상태 검출 함수
int is_empty(LinkedStackType *s)
{
	return (s->top == NULL);
}

element pop(LinkedStackType *s)
{
	if( is_empty(s) ) {
		printf("스택이 비었다.");
		//exit(1);
	}
	else{
		StackNode *temp=s->top;
		element item = temp->item;
		s->top = s->top->link;
		free(temp);
		return item;
	}
}



int** initMazeSetting(int **nMap, int nSize);		//미로 초기화 함수
void printMaze(int **nMap, int nSize,  PLAYER* player);				//미로 출력
int** InfinityMazeCreate(int **nMap, int nSize);	//미로 생성 함수
PLAYER* SetPlayer(int **nMap, int nSize );		//플레이어 출발점 세팅
void push_loc(int **nMap, LinkedStackType* s, int r, int c, PLAYER* player);
void FindEndLine(int **nMap, int nSize,  LinkedStackType* s, PLAYER* player);


int main ()
{
	int						i = 0;
	int						m_nSize = 0;
	int						**m_nMap = NULL;
	int **temp = NULL;
    int size = 0;
	LinkedStackType			m_lStack_Player;
	PLAYER					*Player;
	mStackType q;
	q.top = NULL;
	init(&m_lStack_Player);

	for(;;)
	{
		printf("   ┌\n");
		printf("      랜덤 미로 찾기\n");
		printf("                     ┘\n\n");
		printf("원하시는 맵의 크기를 입력 하시오(size:2~9)\n사용하였던 미로를 보시려면 (-1)를 입력하시오\n : ");
		scanf("%d", &m_nSize);
		fflush(stdin);
		//printf("말의 갯수를 입력하세요 : ");
		//PlayerLimitSelect(m_nSize);
		if( m_nSize >= 2 && m_nSize <= 9) {
			m_nMap = initMazeSetting(m_nMap, m_nSize);
			printMaze(m_nMap, m_nSize,  NULL);
			m_nMap = InfinityMazeCreate(m_nMap, m_nSize);
			printMaze(m_nMap, m_nSize, NULL);
			Player = SetPlayer(m_nMap, m_nSize);
			printMaze(m_nMap, m_nSize,  Player);
			FindEndLine(m_nMap, m_nSize,  &m_lStack_Player, Player);
			mpush(&q, m_nMap, m_nSize);

      }
      else if (m_nSize == -1){
         temp=mpopmap(&q);;
         size = mpopsize(&q);
         printMaze(temp, size, Player);
      }
		else
		{
			printf("다시 입력하세요. ");
			for( i = 0; i < ((m_nSize*ROW)+1); i ++)
			{
				free(m_nMap[i]);
			}
			free(m_nMap);
			exit(0);
		}
	}



	return 0;
}

void FindEndLine(int **nMap, int nSize,  LinkedStackType* s, PLAYER* player)
{

	int count = 0;
	while(( player->X != player->EndX
		|| player->Y != player->EndY))
	{
		int r, c;
		r = player->X;
		c = player->Y;
		nMap[r][c] = PLAYER1;
		push_loc(nMap, s, r-1, c, player);
		push_loc(nMap, s, r+1, c, player);
		push_loc(nMap, s, r, c-1, player);
		push_loc(nMap, s, r, c+1, player);
		if( is_empty(s) )
		{
			printf("실패");
			return;
		}
		else
			*player = pop(s);

		system("cls");
		printf("x : %d y : %d\n", player->X, player->Y);
		printf("Endx : %d Endy : %d\n", player->EndX, player->EndY);
		printMaze(nMap, nSize, player);
		system("pause");
		count++;
	}
	printf("%d번째 움직임, 도착\n", count);
	//break;
}

void push_loc(int **nMap,   LinkedStackType* s, int r, int c, PLAYER* player)
{
	if( r < 0 || c < 0 ) return ;
	if( nMap[r][c] != WALL && nMap[r][c] != PLAYER1)
	{
		element tmp;
		tmp = *player;
		tmp.X = r;
		tmp.Y = c;
		push(s, tmp);
	}
}

PLAYER* SetPlayer(int **nMap, int nSize)
{
	int nStartRow = 0;
	int nStartCol = 0;
	int nEndRow = 0;
	int nEndCol = 0;

	PLAYER *sPlayer = (PLAYER*)malloc(sizeof(PLAYER));

	//시작지점 세팅
	for( ;; )
	{
		nStartRow  = (rand() % (nSize * 2)) + 1;
		if( nMap[nStartRow][nStartCol + 1] != WALL && nMap[nStartRow][nStartCol] == WALL)
		{
			sPlayer->StartX = nStartRow, sPlayer->StartY = nStartCol;	//플레이어의 시작지점 초기화
			nMap[sPlayer->StartX][sPlayer->StartY] = PLAYER1;				//맵 시작지점 표기
			sPlayer->X = sPlayer->StartX, sPlayer->Y = sPlayer->StartY+1;	//플레이어가 시작점 바로 앞에서 시작 하기위해 자리 세팅

			nMap[sPlayer->X][sPlayer->Y] = PLAYER1;	//플레이어 시작지점 초기화
			sPlayer->code = PLAYER1;

			break;
		}
	}
	//종료지점 세팅
	for( ;; )
	{
		nEndRow  = (rand() % (nSize * 2)) + 1;
		if( nMap[nEndRow][(nSize*COL)-1] != WALL && nMap[nEndRow][(nSize*COL)] == WALL)
		{
			sPlayer->EndX = nEndRow, sPlayer->EndY = (nSize*COL)-1;		//플레이어의 도착점 초기화
			nMap[sPlayer->EndX][sPlayer->EndY+1] = PLAYER1;				//맵 도착지점 표기
			break;
		}
	}

	return sPlayer;

}
int** initMazeSetting(int **nMap, int nSize)
{
	int i, j;
	nMap = (int**)malloc(sizeof(int*)*((nSize*ROW)+1));
	memset(nMap, WALL, sizeof(int*)*((nSize*ROW)+1));
	for( i = 0; i < ((nSize*ROW)+1); i++)
	{
		nMap[i] = (int*)malloc(sizeof(int)*((nSize*COL)+1));
		memset(nMap[i], WALL, sizeof(int*)*((nSize*COL)+1));
	}

	for( i = 0; i < ((nSize*ROW)+1); i++)
	{
		for( j = 0; j < ((nSize*COL)+1); j++)
		{
			if( i % 2 == 0 || j % 2 == 0)
				nMap[i][j] = WALL;
			else
				nMap[i][j] = PASS;
		}
	}

	return nMap;
}

void printMaze(int **nMap, int nSize,  PLAYER* player)
{
	int nPlayCounter = 2;
	int i, j;
	for( i = 0; i < nSize * ROW + 1; i++)
	{
		for( j = 0; j < (nSize * COL) + 1; j++)
		{
			if( nMap[i][j] == WALL)
				printf("#");
			else if( nMap[i][j] == PASS)
				printf(" ");
			else
			{
				if( nMap[i][j] == PLAYER1)		//시작점 도착점 표기
					printf("*");

			}
		}
		printf("\n");
	}
	printf("\n");

}


int WallChecking( int **nMap, int nRow, int nCol)
{
	//왼쪽오른쪽아래위가 벽이 있으면 1을 반환한다.
	if( nMap[nRow - 1][nCol] == WALL &&	//왼쪽
		nMap[nRow + 1][nCol] == WALL &&	//아래
		nMap[nRow][nCol + 1] == WALL &&	//오른쪽
		nMap[nRow][nCol - 1] == WALL )		//위
		return HAVE;

	return NOTHAVE;

}

int** InfinityMazeCreate(int **nMap, int nSize)
{
	int	 nVisited = 1, nRow = 1, nCol = 1;
	int	 nNeighbourCounter;
	int	 nNeighbour;
	int	 nRandom_number;
	int	 nCounter = 0;
	int	 nBackRow[MAX];
	int	 nBackCol[MAX];
	int	 nMoving = 0;
	int	 nNeighbourRow[UDLR];
	int	 nNeighbourCol[UDLR];
	int	 nUDLR_Step[UDLR];
	srand((unsigned int)time(0));

	//첫번째 셀부터 시작한다.
	nBackRow[ 0 ] = 1;
	nBackCol[ 0 ] = 1;

	while( nVisited < nSize * (nSize*(COL/2)))
	{
		nNeighbourCounter = -1;	//배열의 0번째시작하기 위해 -1로 초기화

		//위부터 검사 nRow가 0보다 크고 벽이 그 기준으로 상하좌우 벽이 있는경우
		if( nRow - 2 > 0 
			&& WallChecking(nMap, nRow - 2, nCol))
		{
			nNeighbourCounter++;	//배열의 0번째 
			nNeighbourRow[nNeighbourCounter] = nRow - 2;
			nNeighbourCol[nNeighbourCounter] = nCol;
			nUDLR_Step[nNeighbourCounter] = UP;
		}
		//아래쪽 검사 nRow가 사이즈를 넘지않고 상하좌우 벽이 있는경우
		if( nRow + 2 < (nSize * 2) + 1
			&& WallChecking(nMap, nRow + 2, nCol))
		{
			nNeighbourCounter++;	//배열의 0번째 
			nNeighbourRow[nNeighbourCounter] = nRow + 2;
			nNeighbourCol[nNeighbourCounter] = nCol;
			nUDLR_Step[nNeighbourCounter] = DOWN;
		}
		//왼쪽 검사 nCol가 0보다 크고 벽이 그 기준으로 상하좌우 벽이 있는경우
		if( nCol - 2 > 0 
			&& WallChecking(nMap, nRow , nCol -2))
		{
			nNeighbourCounter++;	//배열의 0번째 
			nNeighbourRow[nNeighbourCounter] = nRow;
			nNeighbourCol[nNeighbourCounter] = nCol - 2;
			nUDLR_Step[nNeighbourCounter] = LEFT;
		}
		//오른쪽 검사 nCol가 사이즈를 넘지않고 상하좌우 벽이 있는경우
		if( nCol + 2 < (nSize * 8) + 1 
			&& WallChecking(nMap, nRow, nCol + 2))
		{
			nNeighbourCounter++;	//배열의 0번째 
			nNeighbourRow[nNeighbourCounter] = nRow;
			nNeighbourCol[nNeighbourCounter] = nCol + 2;
			nUDLR_Step[nNeighbourCounter] = RIGHT;
		}


		if( nNeighbourCounter != -1)
		{
			nNeighbour = nNeighbourCounter +1;
			nRandom_number = rand() % nNeighbour;

			nRow = nNeighbourRow[nRandom_number];
			nCol = nNeighbourCol[nRandom_number];

			nCounter++;

			nBackRow[nCounter] = nRow;
			nBackCol[nCounter] = nCol;

			nMoving = nUDLR_Step[nRandom_number];

			switch( nMoving )
			{
			case UP:
				nMap[nRow + 1][nCol] = PASS;
				break;
			case DOWN:
				nMap[nRow - 1][nCol] = PASS;
				break;
			case LEFT:
				nMap[nRow][nCol + 1] = PASS;
				break;
			case RIGHT:
				nMap[nRow][nCol - 1] = PASS;
				break;
			default:
				break;
			}

			nVisited++;
		}
		else
		{
			nRow = nBackRow[nCounter];
			nCol = nBackCol[nCounter];
			nCounter --;
		}

	}

	return nMap;

}