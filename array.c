#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(NULL));

	int arr[8][8] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};

	int ori_arr[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	
	int x = rand() % 6;
	int y = rand() % 6;
	// ori_arr[f][s] = 1;
	printf("%d %d\n", x, y);

	int f = x % 2 == 1 ? (x+=1) : (x+=0) ;
	int s = y % 2 == 1 ? (y+=1) : (y+=0) ;
	// int mul_x = f % 2 == 1;
	// int mul_y = ;
	/*
	printf("Original Array\n");
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
		{	
			printf("%d ", ori_arr[i][j]);
		}
		puts("");
	}
	*/
	printf("%d %d\n", f, s);

	arr[f][s] = 1;
	arr[f][s+1] = 1;
	arr[f+1][s] = 1;
	arr[f+1][s+1] = 1;

	printf("multiplied Array\n");
	for(int i = 0 ; i < 8 ; i++)
	{
		for(int j = 0 ; j < 8 ; j++)
		{	
			printf("%d ", arr[i][j]);
		}
		puts("");
	}
}
