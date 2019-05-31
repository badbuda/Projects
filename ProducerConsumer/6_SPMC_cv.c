/*******************************************************************************
* file name: Threads Condition Variable                                         *
*Created BY Bareket Sar Shalom                                                 *
* date: 14.1.19                                                                *
*******************************************************************************/
#define _DEFAULT_SOURCE /* usleep */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* "malloc" "free" */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CAPACITY 10
#define MESSAGE_SIZE sizeof(int)
#define MESSAGES 1000
#define CONSUMERS 5
#define SLEEP 2500

void *CosumerFunc(void *param);
void *ProducerFunc(void *param);
void MutexUnlock(void *mutex);

static volatile int is_empty = 1;
static volatile int finished_consumers = 0;
static pthread_mutex_t lock_message;
static sem_t sem_producer;
static pthread_cond_t cv;

int main()
{
    int i = 0;
    int message = 0;

    pthread_t producer;
    pthread_t *consumer = malloc(CONSUMERS * sizeof(pthread_t));

    if (!(consumer))
    {
        printf("\n malloc failed\n");
        exit(1);
    }

    if (0 != pthread_mutex_init(&lock_message, NULL))
    {
        printf("\n mutex init failed\n");
        exit(1);
    }

    if (0 != sem_init(&sem_producer, 0, 1))
    {
        printf("\n sem init failed\n");
        exit(1);
    }

    if (0 != pthread_cond_init(&cv, 0))
    {
        printf("\n pthread_cond_init init failed\n");
        exit(1);
    }

    for (i = 0; i < CONSUMERS; ++i)
    {
        if (0 != pthread_create(&consumer[i], NULL, CosumerFunc, &message))
        {
            fprintf(stderr, "Error - pthread_create()\n");
            exit(1);
        }
    }

    if (0 != pthread_create(&producer, NULL, ProducerFunc, &message))
    {
        fprintf(stderr, "Error - pthread_create()\n");
        exit(1);
    }

    if (0 != pthread_join(producer, NULL))
    {
        fprintf(stderr, "Error - pthread_join()\n");
        exit(1);
    }

    usleep(SLEEP);

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

    free(consumer);
    pthread_mutex_destroy(&lock_message);
    sem_destroy(&sem_producer);
    pthread_cond_destroy(&cv);

    return (0);
}

void *CosumerFunc(void *param)
{
    pthread_cleanup_push(MutexUnlock, (void *)(&lock_message));

    while (1)
    {
        if (0 != pthread_mutex_lock(&lock_message))
        {
            fprintf(stderr, "Error - pthread_mutex_lock()\n");
            exit(1);
        }

        while (is_empty)
        {
            pthread_cond_wait(&cv, &lock_message);
        }

        printf("thread %d: %d\n", (int)pthread_self() % 100, *(int *)param);

        ++finished_consumers;

        if (CONSUMERS == finished_consumers)
        {
            is_empty = 1;
            finished_consumers = 0;

            if (0 != sem_post(&sem_producer))
            {
                fprintf(stderr, "Error - sem_post()\n");
                exit(1);
            }
        }

        if (0 != pthread_mutex_unlock(&lock_message))
        {
            fprintf(stderr, "Error - pthread_mutex_unlock()\n");
            exit(1);
        }

        usleep(SLEEP);
    }

    pthread_cleanup_pop(1);
    return (NULL);
}

void *ProducerFunc(void *param)
{
    int i = 0;

    for (i = 0; i < MESSAGES; ++i)
    {
        if (0 != sem_wait(&sem_producer))
        {
            fprintf(stderr, "Error - sem_wait()\n");
            exit(1);
        }

        *(int *)param = i;

        is_empty = 0;

        if (0 != pthread_cond_broadcast(&cv))
        {
            fprintf(stderr, "Error - pthread_cond_broadcast()\n");
            exit(1);
        }

        /*  usleep(SLEEP); */
    }

    return (NULL);
}

void MutexUnlock(void *mutex)
{
    if (0 != pthread_mutex_unlock((pthread_mutex_t *)mutex))
    {
        fprintf(stderr, "Error - pthread_mutex_unlock()\n");
        exit(1);
    }
}
