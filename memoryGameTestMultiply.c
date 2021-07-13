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
int dly = 1000 * 1000;
int difficulty = 0;

int COL_PINS[COLS] = {0, 1, 2, 3}; 
int ROW_PINS[ROWS] = {22, 23, 24, 25};

char pressedKey = '\0';

char keys[ROWS][COLS] = { 
   {'1', '2', '3', '4'},
   {'5', '6', '7', '8'},
   {'9', '0', 'A', 'B'},
   {'C', 'D', 'E', 'F'}
};

char answer[5] = {0, };

void init_keypad();
void printMatrix(int (*arr)[COLS*2]);
void generateRandomMatrix(int (*arr)[COLS*2], int n);
char get_key();
int findLowRow();

int main()
{
	int i;	
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

	for(i = 0 ; i < 1 ; i++)
	{
		refresh();
		printf("\t\t\tpattern %d\n", i+1);
		generateRandomMatrix(mat, i);
		printMatrix(mat);
		usleep(dly);
	}
	
	printf("the answer is ");
	for(i = 0 ; i < 5 ; i++)
		printf("%c ", answer[i]);
	printf("\n");
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
        if (digitalRead(ROW_PINS[r]) == LOW)
			return r;

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
	int ori_x = rand() % (COLS+2);
	int ori_y = rand() % (ROWS+2);
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

		prev_mat[0] = ori_y;
		prev_mat[1] = ori_x;
	printf("ori_x : %d, ori_y : %d\n", ori_x, ori_y);
		printf("mul_x : %d, mul_y : %d\n", mul_x, mul_y);
	}
}

void printMatrix(int (*arr)[COLS*2])
{	
	// ■□
	// filled black square if array value is 1
	int i, j = 0 ;
	printf("\n\n\n\n\n\n");
	for(i = 0 ; i < COLS*2 ; i++)
	{
		printf("\t\t\t ");
		for(j = 0 ; j < ROWS*2 ; j++)
		{
			arr[i][j] == 1 ? printf("■ ") : printf("□ ");
		}
		puts("");
	}
		
}


