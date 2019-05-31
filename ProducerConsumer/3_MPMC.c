/*******************************************************************************
* file name: Threads Multiple Producer Multiple Consumer                        *
*Created BY Bareket Sar Shalom                                                 *
* date: 14.1.19                                                                *
*******************************************************************************/
#define _DEFAULT_SOURCE /* usleep */

#include <stdio.h>
#include <stdlib.h> /* "malloc" "free" "size_t" */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "queue.h"
#include "enum_rd58.h"

#define MESSAGES 260
#define PRODUCERS 4
#define CONSUMERS 3
#define SLEEP 2500
#define FALSE_SHARE_COEF 10

void *CosumerFunc(void *param);
void *ProducerFunc(void *param);

typedef struct consumer_params_s
{
	queue_t *q;
	int *message;
} consumer_params_t;

static pthread_mutex_t lock_queue;
static sem_t sem_dequeue;

int main()
{
	int i = 0;
	int message[MESSAGES * FALSE_SHARE_COEF];

	queue_t *q = QueueCreate();

	pthread_t *producer = malloc(PRODUCERS * sizeof(pthread_t));
	pthread_t *consumer = malloc(CONSUMERS * sizeof(pthread_t));
	consumer_params_t *consumer_params = malloc(sizeof(consumer_params_t));
	consumer_params->q = q;
	consumer_params->message = message;

	if (!(consumer && producer && consumer_params && q))
	{
		printf("\n malloc failed\n");
		exit(1);
	}

	for (; i < MESSAGES; ++i)
	{
		message[i * FALSE_SHARE_COEF] = i;
	}

	if (0 != pthread_mutex_init(&lock_queue, NULL))
	{
		printf("\n mutex init failed\n");
		exit(1);
	}

	if (0 != sem_init(&sem_dequeue, 0, 0))
	{
		printf("\n sem init failed\n");
		exit(1);
	}

	for (i = 0; i < CONSUMERS; ++i)
	{
		if (0 != pthread_create(&consumer[i], NULL, CosumerFunc, q))
		{
			fprintf(stderr, "Error - pthread_create()\n");
			exit(1);
		}
	}

	for (i = 0; i < PRODUCERS; ++i)
	{
		if (0 != pthread_create(&producer[i], NULL, ProducerFunc, consumer_params))
		{
			fprintf(stderr, "Error - pthread_create()\n");
			exit(1);
		}
	}

	for (i = 0; i < PRODUCERS; ++i)
	{
		if (0 != pthread_join(producer[i], NULL))
		{
			fprintf(stderr, "Error - pthread_join()\n");
			exit(1);
		}
	}

	while (FALSE == QueueIsEmpty(q))
	{
	}

	for (i = 0; i < CONSUMERS; ++i)
	{
		if (0 != pthread_cancel(consumer[i]))
		{
			fprintf(stderr, "Error - pthread_cancel()\n");
			exit(1);
		}
	}

	for (i = 0; i < CONSUMERS; ++i)
	{
		if (0 != pthread_join(consumer[i], NULL))
		{
			fprintf(stderr, "Error - pthread_join()\n");
			exit(1);
		}
	}

	free(producer);
	free(consumer);
	free(consumer_params);
	pthread_mutex_destroy(&lock_queue);
	sem_destroy(&sem_dequeue);
	QueueDestroy(&q);

	return (0);
}

void *CosumerFunc(void *param)
{
	if (0 != pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
	{
		printf("\n pthread_setcanceltype failed\n");
		exit(1);
	}

	while (1)
	{
		if (0 != sem_wait(&sem_dequeue))
		{
			fprintf(stderr, "Error - sem_wait()\n");
			exit(1);
		}

		if (0 != pthread_mutex_lock(&lock_queue))
		{
			fprintf(stderr, "Error - pthread_mutex_lock()\n");
			exit(1);
		}

		printf("thread%d: %d\n", (int)pthread_self(), *(int *)QueueDequeue((queue_t *)param));

		if (0 != pthread_mutex_unlock(&lock_queue))
		{
			fprintf(stderr, "Error - pthread_mutex_unlock()\n");
			exit(1);
		}
	}

	return (NULL);
}

void *ProducerFunc(void *param)
{
	int i = 0;
	queue_t *q = ((consumer_params_t *)param)->q;
	int *message = ((consumer_params_t *)param)->message;

	for (i = 0; i < MESSAGES; ++i)
	{
		if (0 != pthread_mutex_lock(&lock_queue))
		{
			fprintf(stderr, "Error - pthread_mutex_lock()\n");
			exit(1);
		}

		if (SUCCESS != QueueEnqueue(q, message + i * FALSE_SHARE_COEF))
		{
			fprintf(stderr, "Error - MEMORY_ALLOCATION_FAIL()\n");
			exit(1);
		}

		if (0 != sem_post(&sem_dequeue))
		{
			fprintf(stderr, "Error - sem_post()\n");
			exit(1);
		}

		if (0 != pthread_mutex_unlock(&lock_queue))
		{
			fprintf(stderr, "Error - pthread_mutex_unlock()\n");
			exit(1);
		}

		usleep(SLEEP);
	}

	return (NULL);
}
