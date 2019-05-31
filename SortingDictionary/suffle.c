/*******************************************************************************
*Description: Shuffelling and sorting a dictionary                             *
*Created BY Bareket Sar Shalom                                                 *
*Date: 15.11.18                                                                *
*******************************************************************************/
#include <stdlib.h>/*******************malloc *********************************/
#include <stdio.h>/*******************printf **********************************/
#include <time.h>/*******************time **********************************/
#include <string.h>/*******************strcmp *********************************/
#include <assert.h>/***********************assert******************************/
#include <sys/types.h>
#include <pthread.h>

#include "enum_rd58.h"
#include "function_pointers.h"
#include "sorting_algorithms.h"
/******************************************************************************/
#define THREAD_NUMBER 6
#define WORDS_PER_THREAD (dictionary_array_num_lines / THREAD_NUMBER)
#define WORDS_LEFT (dictionary_array_num_lines % THREAD_NUMBER)
#define LOOPS_NUMBER 100
#define IS_ODD rand() % 2 != 0
#define ERROR -1
/******************************************************************************/
int dictionary_array_num_lines = 0;
char **dictionary_array;

static void Merge(char **subarray1, char **subarray2, size_t subarray1_count,
                  size_t subarray2_count)
{
    size_t i = 0, j = 0, k = 0;
    char **temp_array = (char **)malloc(sizeof(char *)
                                        * (subarray1_count + subarray2_count));

    while (i < subarray1_count && j < subarray2_count)
    {
        if (strcmp(*(subarray1 + i), *(subarray2 + j)) >= 0)
        {
            *(temp_array + k) = *(subarray2 + j);
            ++j;
        }
        else
        {
            *(temp_array + k) = *(subarray1 + i);
            ++i;
        }

        ++k;
    }

    if (i < subarray1_count)
    {
        memcpy((temp_array + k), (subarray1 + i),
               sizeof(char *) * (subarray1_count - i));
    }
    else
    {
        memcpy((temp_array + k), (subarray2 + j),
               sizeof(char *) * (subarray2_count - j));
    }

    memcpy(subarray1, temp_array, (subarray1_count + subarray2_count) * sizeof(char *));
    free(temp_array);
}
/******************************************************************************/
void PrintArr()
{
	int i = 0;
	for (i = 0; i < dictionary_array_num_lines; ++i)
	{
		printf("%s", dictionary_array[i]);
	}
}
/******************************************************************************/
int ShuffleChar(const void* before, const void* after)
{
	if (IS_ODD)
	{
		return (*((char **)before) - *((char **)after));
	}

	return (*((char **)after) - *((char **)before));
}
/******************************************************************************/
void Shuffle()
{
	MergeSort(dictionary_array, sizeof(char *), dictionary_array_num_lines,	ShuffleChar);
}
/******************************************************************************/
int CompareChar(const void* before, const void* after)
{
	return strcmp(*(char**)before , *(char**)after);
}
/******************************************************************************/
void *SortFunction(void *arr_to_swap)
{
	MergeSort(arr_to_swap, sizeof(char *), WORDS_PER_THREAD, CompareChar);

	return (NULL);
}
/******************************************************************************/
void *LastSort(void *arr_to_swap)
{
	MergeSort(arr_to_swap, sizeof(char *), WORDS_PER_THREAD + WORDS_LEFT,	CompareChar);

	return (NULL);
}
/******************************************************************************/
char **GenerateDictionaryData()
{
	FILE *dictionary;
	int i;
	char ch;

	dictionary = fopen("american-english", "r");

	if (NULL == dictionary)
	{
		return (NULL);
	}

	while(!feof(dictionary))
	{
		ch = fgetc(dictionary);

		if(ch == '\n')
		{
			dictionary_array_num_lines++;
		}
	}

	dictionary_array = calloc(sizeof(char *),dictionary_array_num_lines);
	fseek(dictionary,0,SEEK_SET);

	for(i = 0; i < dictionary_array_num_lines ; i++)
	{
		char * line_char = NULL;
		size_t length = 0;

	if  (ERROR != getline(&line_char, &length, dictionary))
	{
			char* word;
			word = calloc(sizeof(char),(length - 1));
			strncpy(word, line_char,length - 1);
			dictionary_array[i] = word;
		}

		free(line_char);
	}

	fclose(dictionary);

	return dictionary_array;
}
/******************************************************************************/
int main()
{
	int i = 0, j = 0;
	pthread_t threads_arr[THREAD_NUMBER];
	time_t start, end;
	int final_time;

	GenerateDictionaryData();

	time(&start);

	for (; i < LOOPS_NUMBER; ++i)
	{
		Shuffle();

		for (j = 0; j < THREAD_NUMBER - 1; ++j)
		{
			if (0 != pthread_create(&threads_arr[j], NULL, SortFunction,
					      (char **)dictionary_array + WORDS_PER_THREAD * j))
			{
				return (0);
			}
		}

		if (0 != pthread_create(&threads_arr[j], NULL, LastSort,
				      (char **)dictionary_array + WORDS_PER_THREAD * j))
		{
			return (0);
		}

		for (j = 0; j < THREAD_NUMBER; ++j)
		{
			if (0 != pthread_join(threads_arr[j], NULL))
			{
				return (0);
			}
		}

		for (j = 1; j < THREAD_NUMBER - 1; ++j)
		{
			Merge(dictionary_array, (char **)dictionary_array + j * WORDS_PER_THREAD,
						        j * WORDS_PER_THREAD, WORDS_PER_THREAD);
		}

		Merge(dictionary_array, (char **)dictionary_array + j * WORDS_PER_THREAD,
			j * WORDS_PER_THREAD, ((THREAD_NUMBER > 1) * WORDS_PER_THREAD) +
					    dictionary_array_num_lines % THREAD_NUMBER);
	}

	time(&end);
	final_time = end - start;

	PrintArr();
	printf(" total time is: %d", final_time);

	return (0);
}
