#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>

volatile int i = 500;
int cond = 1;

PI_THREAD(t1)
{
	while(cond)
	{
		while(i)
		{
			i--;
			delay(10);
			if(i<=0) break;
		}
		delay(10);
	}
}


int main()
{
	piThreadCreate(t1);
	while(i)
	{
		printf("%d\n", i);
		delay(10);
	}
	
	printf("check variable i...\n");
	delay(1000);
	i = 1000;
	while(i)
	{
		printf("%d\n", i);
		delay(10);
	}
	printf("End of Loop\n");
}
