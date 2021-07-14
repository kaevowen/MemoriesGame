#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <wiringPi.h>

// clear screen macro
#define refresh() printf("\033[H\033[J")

#define COLS 4
#define ROWS 4

int prev_mat[2] = {0, };
int dly = 1000 * 700;
int difficulty = 0;
int condition = 1;
int wait_until_marking_all = 1;
int round_score = 500;
int tmp_score = 0;
int total_score = 0;
int COL_PINS[COLS] = {0, 1, 2, 3}; 
int ROW_PINS[ROWS] = {22, 23, 24, 25};
char answer[5] = {0, };
char pressedKey = '\0';
/*
char keys[ROWS][COLS] = { 
   {'1', '2', '3', '4'},
   {'5', '6', '7', '8'},
   {'9', '0', 'A', 'B'},
   {'C', 'D', 'E', 'F'}
};
*/
char keys[8][8] = { 
   {'1', '0', '2', '0', '3', '0', '4', '0'},
   {'0', '0', '0', '0', '0', '0', '0', '0'},
   {'5', '0', '6', '0', '7', '0', '8', '0'},
   {'0', '0', '0', '0', '0', '0', '0', '0'},
   {'9', '0', '0', '0', 'A', '0', 'B', '0'},
   {'0', '0', '0', '0', '0', '0', '0', '0'},
   {'C', '0', 'D', '0', 'E', '0', 'F', '0'},
   {'0', '0', '0', '0', '0', '0', '0', '0'},
};

char cpu_answer[5] = {0, };
char your_answer[5] = {0, };
void init_keypad();
void printMatrix(int (*arr)[COLS*2]);
void generateRandomMatrix(int (*arr)[COLS*2], int n);

char get_key();
int findLowRow();
int CalcScore();

PI_THREAD(calcScore)
{
	while(condition)
	{
		while(round_score)
		{
			round_score--;
			if(round_score <= 0) break;
			delay(10);
		}
		delay(100);
	}
}

PI_THREAD(get_keypad_value)
{
	int cnt = 0;
	char prev_key;
	while(round_score)
	{
		char x = get_key();

      	if (x != prev_key && x != '\0')
		{
			your_answer[cnt++] = x;
		}
		else
			continue;
		
		prev_key = x;
		printf("%c %c ", x, prev_key);
		delay(100);

		if( cnt >= 5 )
		{
			tmp_score = round_score;
			wait_until_marking_all = 0;
			round_score = 0;
			printf("\n");
			break;
		}
	}
}

int main()
{
	int mat[COLS*2][ROWS*2] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};

	srand(time(NULL));
	wiringPiSetup();
	init_keypad();

	for(int i = 0 ; i < 5 ; i++)
	{
		refresh();
		printf("pattern %d\n", i+1);
		generateRandomMatrix(mat, i);
		printMatrix(mat);
		usleep(dly);
	}
	
	round_score = 500;
	piThreadCreate(calcScore);
	piThreadCreate(get_keypad_value);
	int correctCnt = 0;
	
	while(wait_until_marking_all)
	{
		delay(10);
	}
	
	printf("the answer is ");
	for(int i = 0 ; i < 5 ; i++)
		printf("%c ", cpu_answer[i]);
	printf("\n");

	for(int i = 0 ; i < 5 ; i++)
		if(cpu_answer[i] == your_answer[i])
			correctCnt++;

	printf("Score : %d * %d(correct answer) = %d\n",
			tmp_score, correctCnt, tmp_score*correctCnt);

	return 0;
}

void init_keypad()
{
    int i, j;

    for(i=0;i<COLS;i++)
    {
        pinMode(COL_PINS[i], OUTPUT);
        digitalWrite(COL_PINS[i], HIGH);
    }

    for(i=0;i<ROWS;i++)
    {
        pinMode(ROW_PINS[i], INPUT); 
        pullUpDnControl(ROW_PINS[i], PUD_UP);
    }
}

// check ROWS_PINS state and return. if not found return -1
int findLowRow()
{
    for (int r = 0; r < ROWS; r++)
	{
        if (digitalRead(ROW_PINS[r]) == LOW)
			return r;
	}

    return -1;
}

char get_key()
{
	int rowIndex;
	for (int c = 0; c < COLS; c++)
	{
		digitalWrite(COL_PINS[c], LOW);
		rowIndex = findLowRow();
		
		if (rowIndex > -1)
		{
			if (!pressedKey)
				pressedKey = keys[rowIndex][c];
			
			return pressedKey;
		
		}

        digitalWrite(COL_PINS[c], HIGH);
    }

    pressedKey = '\0';

    return pressedKey;
}
// filled random array value into 1
void generateRandomMatrix(int (*arr)[COLS*2], int n)
{
	int ori_x = rand() % (COLS + 2);
	int ori_y = rand() % (ROWS + 2);
	int mul_x = 0, mul_y = 0;

	// clear array
	for(int i = 0 ; i < COLS * 2 ; i++)
		for(int j = 0 ; j < ROWS * 2 ; j++)
			arr[i][j] = 0;

	printf("prev_mat_x : %d, prev_mat_y : %d\n", 
			prev_mat[0], prev_mat[1]);
	// if current value was same as previous value, callback himself
	if(ori_x == prev_mat[0] && ori_y == prev_mat[1])
		generateRandomMatrix(arr, n);
	else
	{
		mul_x = ori_x % 2 == 1 ? (ori_x+=1) : (ori_x+=0);
		mul_y = ori_y % 2 == 1 ? (ori_y+=1) : (ori_x+=0);

		arr[mul_y][mul_x]     = 1;
		arr[mul_y][mul_x+1]   = 1;
		arr[mul_y+1][mul_x]   = 1;
		arr[mul_y+1][mul_x+1] = 1;
		
		//Saving return values from keys matrix to answer[] referenced by arguments mul_y, mul_x
		answer[n] = keys[mul_y][mul_x];
		
		/*
		if(mul_x == 0)
		{
			answer[n] = keys[ori_y][ori_x];
		}
		else if (mul_x == 6)
		{
			answer[n] = keys[ori_y-2][ori_x-2];
		}
		else
		{
			answer[n] = keys[ori_y-1][ori_x-1];
		}
		*/
		
		prev_mat[0] = ori_y;
		prev_mat[1] = ori_x;
		printf("ori_x : %d, ori_y : %d\n", ori_x, ori_y);
		printf("mul_x : %d, mul_y : %d\n", mul_x, mul_y);
		//printf("answer is %c\n", answer[i]);
		//Global variable for counter 'i' increases
	}
}

void printMatrix(int (*arr)[COLS*2])
{	
	// ■□
	// filled black square if array value is 1
	int i, j = 0 ;
	for(i = 0 ; i < COLS*2 ; i++)
	{
		for(j = 0 ; j < ROWS*2 ; j++)
			arr[i][j] == 1 ? printf("■ ") : printf("□ ");

		puts("");
	}
		
}
