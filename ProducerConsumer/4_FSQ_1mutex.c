/*******************************************************************************
* file name: Threads FSQ                                                        *
*Created BY Bareket Sar Shalom                                                 *
* date: 14.1.19                                                                *
*******************************************************************************/
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h> /* "malloc" "free" "size_t" */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "c_buffer.h"
#define CAPACITY 2
#define MESSAGE_SIZE sizeof(int)
#define MESSAGES 10
#define PRODUCERS 4
#define CONSUMERS 3
#define SLEEP 2500
#define FALSE_SHARE_COEF 10

void *CosumerFunc(void *param);
void *ProducerFunc(void *param);

typedef struct consumer_params_s
{
    c_buffer_t *c_buff;
    int *message;
} consumer_params_t;

static pthread_mutex_t lock_buffer;
static sem_t sem_write;
static sem_t sem_read;

int main()
{
    int i = 0;
    int message[MESSAGES * FALSE_SHARE_COEF];

    c_buffer_t *c_buff = CBufferCreate(CAPACITY * MESSAGE_SIZE);

    pthread_t *producer = malloc(PRODUCERS * sizeof(pthread_t));
    pthread_t *consumer = malloc(CONSUMERS * sizeof(pthread_t));
    consumer_params_t *consumer_params = malloc(sizeof(consumer_params_t));
    consumer_params->c_buff = c_buff;
    consumer_params->message = message;

    if (!(c_buff && producer && consumer && consumer_params))
    {
        printf("\n malloc failed\n");
        exit(1);
    }

    for (; i < MESSAGES; ++i)
    {
        message[i * FALSE_SHARE_COEF] = i;
    }

    if (0 != pthread_mutex_init(&lock_buffer, NULL))
    {
        printf("\n mutex init failed\n");
        exit(1);
    }

    if (0 != sem_init(&sem_write, 0, CAPACITY))
    {
        printf("\n sem init failed\n");
        exit(1);
    }

    if (0 != sem_init(&sem_read, 0, 0))
    {
        printf("\n sem init failed\n");
        exit(1);
    }

    for (i = 0; i < CONSUMERS; ++i)
    {
        if (0 != pthread_create(&consumer[i], NULL, CosumerFunc, c_buff))
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

    while (0 != CBufferGetSize(c_buff))
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

    free(consumer_params);
    free(producer);
    free(consumer);
    pthread_mutex_destroy(&lock_buffer);
    sem_destroy(&sem_write);
    sem_destroy(&sem_read);
    CBufferDestroy(&c_buff);

    return (0);
}

void *CosumerFunc(void *param)
{
    int dest = 0;
    if (0 != pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
    {
        printf("\n pthread_setcanceltype failed\n");
        exit(1);
    }

    while (1)
    {
        if (0 != sem_wait(&sem_read))
        {
            fprintf(stderr, "Error - sem_wait()\n");
            exit(1);
        }

        if (0 != pthread_mutex_lock(&lock_buffer))
        {
            fprintf(stderr, "Error - pthread_mutex_lock()\n");
            exit(1);
        }

        CBufferRead((c_buffer_t *)param, &dest, MESSAGE_SIZE);
        printf("thread %d: %d\n", (int)pthread_self(), dest);

        if (0 != sem_post(&sem_write))
        {
            fprintf(stderr, "Error - sem_post()\n");
            exit(1);
        }

        if (0 != pthread_mutex_unlock(&lock_buffer))
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
    c_buffer_t *c_buff = ((consumer_params_t *)param)->c_buff;
    int *message = ((consumer_params_t *)param)->message;

    for (i = 0; i < MESSAGES; ++i)
    {
        if (0 != sem_wait(&sem_write))
        {
            fprintf(stderr, "Error - sem_wait()\n");
            exit(1);
        }

        if (0 != pthread_mutex_lock(&lock_buffer))
        {
            fprintf(stderr, "Error - pthread_mutex_lock()\n");
            exit(1);
        }

        if (MESSAGE_SIZE != CBufferWrite(c_buff, message + i * FALSE_SHARE_COEF, MESSAGE_SIZE))
        {
            printf("error\n");
        }

        if (0 != sem_post(&sem_read))
        {
            fprintf(stderr, "Error - sem_post()\n");
            exit(1);
        }

        if (0 != pthread_mutex_unlock(&lock_buffer))
        {
            fprintf(stderr, "Error - pthread_mutex_unlock()\n");
            exit(1);
        }

        usleep(SLEEP);
    }

    return (NULL);
}
