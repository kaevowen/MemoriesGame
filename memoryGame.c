#include <stdio.h>
#include <wiringPi.h>

#define COLS 4
#define ROWS 4

int COL_PINS[COLS] = {0, 1, 2, 3};
int ROW_PINS[ROWS] = {22, 23, 24, 25};
char pressedKey = '\0';
char keys[ROWS][COLS] = {
   {'1', '2', '3', 'A'},
   {'4', '5', '6', 'B'},
   {'7', '8', '9', 'C'},
   {'*', '0', '#', 'D'}
};
int arr[5] = {0, };

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

PI_THREAD(t1)
{
	char prev_key;
	int cnt = 0;
	while(1)
	{
      char x = get_key();

      if (x != prev_key && x != '\0')
	  {
         printf("pressed: %c\n", x);
		 arr[cnt] = x;
		 cnt++;
	  }
      else
         continue;

      delay(1);
	  prev_key = x;
	  if(cnt==5) break;
   }

}
int main()
{
	wiringPiSetup();
	init_keypad();
	char prev_key;
		
	piThreadCreate(t1);
	while(1)
	{
		delay(10000);
		break;
	}

	for(int i = 0 ; i < 5 ; i++)
	{
		printf("%c ", arr[i]);
	}
	return 0;
}
