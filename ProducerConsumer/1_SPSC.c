/*******************************************************************************
* file name: Threads Single Producer Single Consumer                            *
*Created BY Bareket Sar Shalom                                                 *
* date: 14.1.19                                                                *
*******************************************************************************/
#define _DEFAULT_SOURCE /* usleep */

#include <stdio.h>
#include <stdlib.h> /* "size_t" */
#include <pthread.h>
#include <unistd.h> /* usleep */

#define SLEEP 2500
#define MESSAGES 10

void *Cosumer(void *param);

static volatile int is_empty = 1;

int main()
{
	int i = 0;
	int message = 0;
	pthread_t consumer;

	if (0 != pthread_create(&consumer, NULL, Cosumer, &message))
	{
		fprintf(stderr, "Error - pthread_create()\n");
		exit(1);
	}

	for (i = 0; i < MESSAGES; ++i)
	{
		message = i;
		is_empty = 0;
		usleep(SLEEP);
	}

	if (0 != pthread_cancel(consumer))
	{
		fprintf(stderr, "Error - pthread_cancel()\n");
		exit(1);
	}

	if (0 != pthread_join(consumer, NULL))
	{
		fprintf(stderr, "Error - pthread_join()\n");
		exit(1);
	}

	return 0;
}

void *Cosumer(void *param)
{
	while (1)
	{
		pthread_testcancel();

		if (0 == is_empty)
		{
			printf("%d\n", *(int *)param);
			is_empty = 1;
		}
	}

	return (NULL);
}
