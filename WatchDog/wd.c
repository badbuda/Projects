/*******************************************************************************
* NAME: WD                                                                   *
*                                                                              *
*Created BY Bareket Sar Shalom                                                 *
* DATE UPDATED:  22.01.19                                                      *
*******************************************************************************/
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE
#include <stdio.h> /* fprintf */
#include <fcntl.h> /* "assert" */
#include <semaphore.h> /* "assert" */
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/******************************************************************************/
#include "wd.h"
#include "task_scheduler.h"
#include "IsPrintVersion.h"
/******************************************************************************/
#define TIME_NO_SIGNAL_RECIVED 3
#define FAIL -1
/******************************************************************************/
volatile bool dnr_exit = FALSE;
volatile bool flag = FALSE;
volatile bool stop_flag = FALSE;
static volatile int counter = 0;
static task_scheduler_t *scheduler;
pthread_t thread_wd;
char *argv_copy[4] = {"./wd_main.out", NULL, "", NULL};
pid_t child_id;
pid_t signal_pid;
sem_t *sem;
char* wdenv;
/******************************************************************************/
void SigHandlerDNR(int sig);
void SigHandlerLive(int sig);
void Resurrection();

/******************************************************************************/
static void DestroyFunction(void *param)
{
	(void)param;
}

static status SendSignal(void *param)
{
	(void)param;

	kill(signal_pid, SIGUSR1);
	++counter;

	if (isPrintVersion)
		printf("[pid: %d][After SendSignal][Counter Value %d][signal id %d]\n", getpid(), counter, signal_pid);

	if (TRUE == dnr_exit)
	{
		if (isPrintVersion)
			printf("[pid: %d][received dnr signal][Counter Value %d][signal id %d]\n", getpid(), counter, signal_pid);

		TaskSchedulerStop(scheduler);
	}

	if (counter >= TIME_NO_SIGNAL_RECIVED)
	{
		if (isPrintVersion)
			printf("[pid: %d][Counter is grater than signal received - Process is dead][Counter Value %d][signal id %d]\n", getpid(), counter, signal_pid);

		/*printf("dead %d\n\n", signal_pid);*/
		Resurrection();
		counter = 0;
	}

	return (SUCCESS);
}
/******************************************************************************/
void SigHandlerLive(int sig)
{
	(void)sig;

	printf("SigHandlerLive %d\n", getpid());

	counter = 0;

	if (isPrintVersion)
		printf("[pid: %d][SigHandlerLive][Counter Value %d]\n", getpid(), counter);
}
/******************************************************************************/
void SigHandlerDNR(int sig)
{
	(void)sig;
	dnr_exit = TRUE;
}


void CleanUp(void *param)
{
	if (isPrintVersion)
		printf("[pid: %d][CleanUp][Started]\n", getpid());
	TaskSchedulerDestroy(scheduler);
	sem_unlink("/wdthread");
	sem_close(sem);

	if (isPrintVersion)
		printf("[pid: %d][CleanUp][Finished]\n", getpid());
}


void *WDScheduler(void *param)
{
	(void)param;
	int sval = 0;
	int semRetVal = 0;

	if (isPrintVersion)
		printf("[pid: %d][WDScheduler][Started]\n", getpid());

	pthread_cleanup_push(CleanUp, NULL);
	scheduler = TaskSchedulerCreate();
	TaskSchedulerAddTask(scheduler, time(NULL), 1, SendSignal, DestroyFunction, NULL);

	if (isPrintVersion)
	{
		semRetVal = sem_getvalue(sem, &sval);
		if (semRetVal != 0)
			exit(-1);

		printf("[pid: %d][WDScheduler][before semaphore][flag = %d ][sem value %d]\n", getpid(), flag, sval);
	}


	/*Sychronize the wd and TFS*/
	if (flag)
	{
		printf("[pid: %d][WDScheduler][before semaphore WAIT][flag = %d ]\n", getpid(), flag);
		sem_wait(sem);
		printf("[pid: %d][WDScheduler][after semaphore WAIT][flag = %d ]\n", getpid(), flag);
	}
	else
	{
		printf("[pid: %d][WDScheduler][before semaphore POST][flag = %d ]\n", getpid(), flag);
		sem_post(sem);
		printf("[pid: %d][WDScheduler][after semaphore POST][flag = %d ]\n", getpid(), flag);
	}

	if (isPrintVersion)
	{
		semRetVal = sem_getvalue(sem, &sval);
		if (semRetVal != 0)
			exit(-1);

		printf("[pid: %d][WDScheduler][After semaphore][flag = %d ][sem value %d]\n", getpid(), flag, sval);
	}


	if (isPrintVersion)
		printf("[pid: %d][WDScheduler][Waiting for each other using a semaphore]\n", getpid());

	if (SUCCESS != TaskSchedulerStart(scheduler))
	{
		return (NULL);
	}

	stop_flag = TRUE;

	if (isPrintVersion)
		printf("[pid: %d][WDScheduler][before pthread_cleanup_pop]\n", getpid());

	pthread_cleanup_pop(1);

	if (isPrintVersion)
		printf("[pid: %d][WDScheduler][Finished]\n", getpid());

	return (NULL);
}
/******************************************************************************/
status KeepMeAlive(char *argv[])
{
	if (isPrintVersion)
		printf("[pid: %d][Keep Me Alive][Enter Keep Me Alive]\n", getpid());

	mode_t semMode =  O_CREAT;
	/*unsigned int semInitValue = 0644;*/
	unsigned int semInitValue = 0;

	struct sigaction process1;
	struct sigaction process2;
	memset(&process1, 0, sizeof process1);
	memset(&process2, 0, sizeof process2);

	wdenv = getenv("WDENV");

	process2.sa_handler = &SigHandlerLive;
	process1.sa_handler = &SigHandlerDNR;

	sigemptyset(&process2.sa_mask);
	sigemptyset(&process1.sa_mask);

	process1.sa_flags = 0;
	process2.sa_flags = 0;

	sigaction(SIGUSR2, &process1, NULL);
	sigaction(SIGUSR1, &process2, NULL);

	/*sem = sem_open("/wdthread", semMode, semInitValue);*/
	sem = sem_open("/w13s23dq", semMode, 0644, semInitValue);
	/*sem = sem_open("/w12sdf3dasdq", semMode, semInitValue);*/
	/*sem_init(sem, 1, 0);*/
	if (sem == SEM_FAILED)
	{
		printf("[pid: %d][Keep Me Alive][Creating semaphore failed]\n", getpid());
		exit(-1);
	}

	if (isPrintVersion)
		printf("[pid: %d][Keep Me Alive][Finished Creating semaphore]\n", getpid());

	if (NULL == wdenv)
	{
		/*First time running */
		if (isPrintVersion)
			printf("[pid: %d][Keep Me Alive][Create environment variable and fork]\n", getpid());

		setenv("WDENV", "1", 1);
		argv_copy[1] = argv[0];

		child_id = fork();

		if (0 > child_id)
		{
			if (isPrintVersion)
				printf("[pid: %d][Keep Me Alive][Fork failed]\n", getpid());
			return (ACTION_FAIL);
		}

		if (child_id == 0)
		{
			/*This is executed by forked thread*/
			if (isPrintVersion)
				printf("[pid: %d][Keep Me Alive][Execute from fork][%s]\n", getpid(), argv_copy[0]);
			if (execvp(argv_copy[0], argv_copy) < 0)
			{
				printf("execvp ERROR");
				if (isPrintVersion)
					printf("[pid: %d][Keep Me Alive][execvp ERROR]\n", getpid());
				exit(-1);
				return (FAIL);
			}
		}
		else
		{
			/*This is executed by father thread*/
			signal_pid = child_id;
			flag = TRUE;
		}
	}
	else
	{
		/* non first time */
		argv_copy[1] = argv[0];
		argv_copy[0] = argv[1];
		signal_pid = getppid();
	}

	if (isPrintVersion)
		printf("[pid: %d][Keep Me Alive][before pthread_create for scheduler]\n", getpid());

	if (FAIL == pthread_create(&thread_wd, NULL, WDScheduler, NULL))
	{
		if (isPrintVersion)
			printf("[pid: %d][Keep Me Alive][pthread_create failed]\n", getpid());

		exit(1);
	}

	if (isPrintVersion)
		printf("[pid: %d][Keep Me Alive][Exited Keep Me Alive]\n", getpid());

	return (SUCCESS);
}
/******************************************************************************/
void DNR()
{
	if (isPrintVersion)
		printf("[pid: %d][DNR][Started DNR]\n", getpid());

	kill(signal_pid, SIGUSR2);
	TaskSchedulerStop(scheduler);
	pthread_join(thread_wd, NULL);

	if (isPrintVersion)
		printf("[pid: %d][DNR][Finished DNR]\n", getpid());
}
/******************************************************************************/
void Resurrection()
{
	pid_t old_pid;

	child_id = fork();

	if (child_id < 0)
	{
		printf("ERROR fork");
		exit(1);
	}

	old_pid = signal_pid;
	signal_pid = child_id;

	if (0 == child_id)
	{
		if (execvp(argv_copy[0], argv_copy) < 0)
		{
			printf("execvp ERROR");
		}
	}
	else
	{
		waitpid(old_pid, NULL, 0);
	}
}
/******************************************************************************/
