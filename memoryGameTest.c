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
void printMatrix(int (*arr)[COLS]);
void generateRandomMatrix(int (*arr)[COLS], int n);
char get_key();
int findLowRow();

int main()
{
	int i;	
	int mat[COLS][ROWS] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	srand(time(NULL));

	for(i = 0;i < 5 ; i++)
	{
		refresh();
		printf("pattern %d\n", i+1);
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
    int r;

    for (r = 0; r < ROWS; r++)
	{
        if (digitalRead(ROW_PINS[r]) == LOW)
			return r;
	}

    return -1;
}

char get_key()
{
	int rowIndex;
	int c;

	for (c = 0; c < COLS; c++)
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
void generateRandomMatrix(int (*arr)[COLS], int n)
{
	int f = rand() % COLS;
	int s = rand() % ROWS;
	int i, j;

	// clear array
	for(i = 0 ; i < COLS ; i++)
		for(j = 0 ; j < ROWS ; j++)
			arr[i][j] = 0;
	
	// if current value was same as previous value, callback himself
	if(f == prev_mat[0] && s == prev_mat[1])
		generateRandomMatrix(arr, n);

	else
	{
		arr[f][s] = 1;
		answer[n] = keys[f][s];
		prev_mat[0] = f;
		prev_mat[1] = s;
	}
}

void printMatrix(int (*arr)[COLS])
{	
	// ■□
	// filled black square if array value is 1
	int i, j = 0 ;
	for(i = 0 ; i < COLS ; i++)
	{
		for(j = 0 ; j < ROWS ; j++)
		{
			arr[i][j] == 1 ? printf("■ ") : printf("□ ");
		}
		puts("");
	}
		
}


