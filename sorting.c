/*******************************************************************************
*Created BY Bareket Sar Shalom                                                 *
*Date: 9.12.18                                                                *
*******************************************************************************/
#include <stdlib.h>/*******************malloc free*****************************/
#include <stdio.h>/*******************malloc free*****************************/
#include <assert.h>/***********************assert******************************/
#include <string.h> /* memcpy */
#include "sorting_algorithms.h"
/******************************************************************************/
#define RIGHT_CHILD(i) ((i * 2) + 2)
#define LEFT_CHILD(i) ((i * 2) + 1)
#define BYTE_RANGE 256
#define THRESHOLD 10
#define CURR (void*)(char*)array + (element_size * i)
#define LEFT_SIDE (char*)array + (element_size * LEFT_CHILD(i))
#define ARRAY_END array_size - 1
#define ARRAY_BEGIN 0
#define HALF_ELEMENT_COUNT (element_count / 2)
#define MASK (i*8)) & 0xFF
#define LOW_HIGH_AVAREGE (low +high) / 2

/******************************************************************************/
void Swap (void *element1, void *element2, size_t elements_size)
{
	char *temp = malloc(elements_size);
	assert(element1 && element2);

	if (NULL == temp)
	{
		return;
	}

	memcpy(temp, element2, elements_size);
	memcpy(element2, element1, elements_size);
	memcpy(element1, temp, elements_size);

	free(temp);
}
/******************************************************************************/
void BubbleSort(void *elements, size_t element_size, size_t element_count,
                compare_function_t compare)
{
	size_t i = 0;
	bool continue_swapping = TRUE;
	assert(elements && compare);

	while (continue_swapping)
	{
		continue_swapping = FALSE;

		for (i = 0; i < element_count - 1; ++i)
		{
			if (compare((char*)elements + i * element_size,
															(char *)elements + ((i + 1) * element_size)) > 0)
			{
				Swap (((char*)elements + (i * element_size)),
										((char*)elements + ((i + 1) * element_size)), element_size);

					continue_swapping = TRUE;
			}
		}
	}
}
/******************************************************************************/
void InsertionSort(void *elements, size_t element_size, size_t element_count,
                   																	compare_function_t compare)
{
	size_t i, j;
	assert(elements && compare);

	for (i = 1; i < element_count; ++i)
	{
	j = i;
		while (j > 0 && compare((char*)elements + j * element_size,
															(char *)elements + ((j - 1) * element_size)) < 0)
		{
			Swap (((char*)elements + (j * element_size)),
						 ((char*)elements + ((j - 1) * element_size)), element_size);
			--j;
		}
	}
}
/******************************************************************************/
void SelectionSort(void *elements, size_t element_size, size_t element_count,
                   																		compare_function_t compare)
{
	size_t i, j;
	void *min;
	assert(elements && compare);

	for (i = 0; i < element_count - 1; ++i)
	{
		min = (char*)elements + i * element_size;

		for (j = i + 1; j < element_count; ++j)
		{
			if (compare(min,(char *)elements + (j * element_size)) > 0)
			{
				min = (char *)elements + (j * element_size);
			}
		}

		Swap (((char*)elements + (i * element_size)), (min),
																																element_size);
	}
}
/******************************************************************************/
static void HeapifyDown(void *array, size_t element_count, size_t i, size_t element_size,
																											compare_function_t compare)
{
	void *left_side = NULL;
	void *right_side = NULL;
	void *current = NULL;
	size_t largest = i;
	assert(array && compare);

	if (element_count <= LEFT_CHILD(i))
	{
		return ;
	}

	left_side = (char*)array + (element_size * LEFT_CHILD(i));
	right_side = (char*)array + (element_size * RIGHT_CHILD(i));
	current = (char*)array + (element_size * i);

	if (compare(current, left_side) < 0)
	{
		largest = LEFT_CHILD(i);
	}

	if (RIGHT_CHILD(i) < element_count)
	{
		if (compare((char*)array + (element_size * largest), right_side) < 0)
		{
			largest = RIGHT_CHILD(i);
		}
	}

	if (largest != i)
	{
		Swap(current, (char*)array + (element_size * largest),	element_size);
		HeapifyDown(array, element_count, largest, element_size, compare);
	}
}
/******************************************************************************/
status HeapSort(void *elements, size_t element_size, size_t element_count,
                   compare_function_t compare)
{
	int i = 0, j;
	assert(elements && compare);

	for (i = HALF_ELEMENT_COUNT - 1; i >= 0; --i)
	{
		HeapifyDown(elements, element_count, i, element_size, compare);
	}

	for (j = element_count; j > 0 ; --j)
	{
		void *current = (char*)elements + (element_size * (j - 1));
		Swap(elements, current, element_size);
		--element_count;
		HeapifyDown(elements, element_count, 0, element_size, compare);
	}

	return (SUCCESS);
}
/******************************************************************************/
static int Partition(void *array, size_t low, size_t high, size_t element_size,
	compare_function_t compare)
{
	void* pivot = (char*)array + (element_size * (high -1));
	size_t j = low , i = (low - 1);
	assert(array && compare);

	for (	; j < high; j++)
	{
		if (compare((char*)array + (element_size * j), pivot) < 0)
		{
			++i;
			Swap((char*)array + (element_size * i),
											((char*)array + (element_size * j)), element_size);
		}
	}

	Swap((char*)array + (element_size * (i + 1)),
							((char*)array + (element_size * (high - 1))), element_size);

	return (i + 1);
}
/******************************************************************************/
static void QuickSortWrapper(void *array, size_t low, size_t high, size_t element_size,
																											compare_function_t compare)
{
	assert(array && compare);

	if (low < high)
	{
		int Part = Partition(array, low, high,  element_size, compare);
		QuickSortWrapper(array, low, Part, element_size, compare);
		QuickSortWrapper(array, Part + 1, high, element_size, compare);
	}
}
/******************************************************************************/
status QuickSort(void *elements, size_t element_size, size_t element_count,
        																							compare_function_t compare)
	{
		QuickSortWrapper(elements, 0, element_count, element_size, compare);

		return (SUCCESS);
	}
/******************************************************************************/

static void Merge(void *left, void *right, void *middle, void *elements,
	 					size_t element_count, size_t element_size, compare_function_t compare)
{
	void *left_p = left;
	void *middle_p = middle;
	void *elements_p = elements;
	assert(left && right && middle && elements && compare);

	while ((middle > left_p) && (middle_p <= right))
	{
		if (0 >= compare(left_p, middle_p))
		{
		  memcpy(elements_p, left_p, element_size);
		  left_p = (char*)left_p + element_size;
		  elements_p = (char*)elements_p + element_size;
		}
		else
		{
		  memcpy(elements_p, middle_p, element_size);
		  middle_p = (char*)middle_p + element_size;
		  elements_p = (char*)elements_p + element_size;
		}
	}

	while (middle > left_p)
	{
		memcpy(elements_p, left_p, element_size);
		left_p = (char*)left_p + element_size;
		elements_p = (char*)elements_p + element_size;
	}

	while (middle_p <= right)
	{
		memcpy(elements_p, middle_p, element_size);
		middle_p = (char*)middle_p + element_size;
		elements_p = (char*)elements_p + element_size;
	}

	memcpy(left, elements, (element_count * element_size));

}
/******************************************************************************/
static void SplitArray(void *left, void *right, size_t element_count,
								size_t element_size, void *elements, compare_function_t compare)
{
	size_t size = HALF_ELEMENT_COUNT;
	void *middle = (char*)left + (size * element_size);
	assert(left && right && elements && compare);

	if (right > left)
	{
		SplitArray(left, (char*)middle - element_size, size, element_size, elements, compare);
		SplitArray(middle , right, element_count - size, element_size, elements, compare);
		Merge(left, right, middle, elements, element_count, element_size, compare);
	}
}
/******************************************************************************/
status MergeSort(void *elements, size_t element_size, size_t element_count,
																											compare_function_t compare)
{
	void *new_elements = malloc(element_count * element_size);
	void *right = (char*)elements + ((element_count - 1)* element_size);
	assert(elements && compare);

	if (NULL == new_elements)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	SplitArray(elements, right, element_count, element_size, new_elements, compare);
	free(new_elements);

	return (SUCCESS);
}
/******************************************************************************/
status CountingSort(void *elements, size_t element_size,size_t element_count,
                  key_to_num_t key_to_num, void *param, size_t range)
{
	size_t i;
	char *element_pointer = elements;
	size_t *lut_array = NULL;
	char *sorted_array = NULL;
	size_t element_position;
	assert(elements && key_to_num);

	lut_array = calloc (range + 1, sizeof(size_t));

	if (NULL == lut_array)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}
	sorted_array = calloc (element_count, element_size);

	if (NULL == sorted_array)
	{
		free(lut_array);
		return (MEMORY_ALLOCATION_FAIL);
	}

	for (i = 1; i <= element_count ; ++i)
	{
 		++lut_array[key_to_num(element_pointer, param)];
		element_pointer = element_pointer + element_size;
	}

	for (i = 1; i <= range ; ++i)
	{
		lut_array [i] = lut_array[i] + lut_array[i - 1];
	}

	element_pointer = element_pointer - element_size;

	for (i = element_count; i > 0 ; --i)
	{
		element_position = lut_array[key_to_num(element_pointer, param)] - 1;
		--lut_array[key_to_num(element_pointer, param)];
		memcpy(sorted_array + element_position * element_size, 
							element_pointer, element_size);

		element_pointer -= element_size;
	}

	memcpy(elements, sorted_array, (element_count)*(element_size));
	free(lut_array);
	free(sorted_array);

	return (SUCCESS);
}
/******************************************************************************/
typedef struct param_and_key_s
{
	void *param;
	int index;
	key_to_num_t key_to_num;
}param_and_key_t;

size_t KeyToNumWrapper (void *element, void *param_and_key)
{
	void* param = ((param_and_key_t*)param_and_key)->param;
	int i = ((param_and_key_t*)param_and_key)->index;
	key_to_num_t key_to_num = ((param_and_key_t*)param_and_key)->key_to_num;

	return ((key_to_num(element,param) >>  MASK);
}
/******************************************************************************/
status RadixSort(void *elements, size_t element_size, size_t element_count,
				        key_to_num_t key_to_num, void *param, size_t byte_count)
{
	size_t i;
	param_and_key_t *param_and_key = NULL;
	assert(elements && key_to_num);

	param_and_key = malloc(sizeof(param_and_key_t));

	if (NULL == param_and_key)
	{
		return (MEMORY_ALLOCATION_FAIL);
	}

	param_and_key->param = param;
	param_and_key->key_to_num = key_to_num;

	for (i = 0; i < byte_count; ++i)
	{
		param_and_key->index = i;

		CountingSort(elements, element_size, element_count, KeyToNumWrapper,
			 					param_and_key, BYTE_RANGE);
	}

	free(param_and_key);

	return (SUCCESS);
}
/******************************************************************************/
static int * BSearchIterativeWrapper(int sorted_array[], size_t low, size_t high, int key)
{
	int	middle = 0;
	assert(sorted_array);

	while (low <= high)
	{
			 middle = low + (high - low) / 2;

			if (sorted_array[middle] == key)
			{
				return (sorted_array + middle);
			}

			if (sorted_array[middle] < key)
			{
				low = middle + 1;
			}
			else
			{
				high = middle - 1;
			}
	}

	return (NULL);

}
/******************************************************************************/
int * BSearchIterative(int sorted_array[], size_t array_size, int key)
{
	assert(sorted_array);

	return (BSearchIterativeWrapper(sorted_array, ARRAY_BEGIN, ARRAY_END, key));
}
/******************************************************************************/
static int * BSearchRecursiveWrapper(int sorted_array[], size_t low, size_t high,
	 																																			int key)
{
	int	middle = 0;
	assert(sorted_array);

	if (low > high)
	{
		return (NULL);
	}

	middle = LOW_HIGH_AVAREGE;

	if (sorted_array[middle] == key)
	{
		return (sorted_array + middle);
	}

	else if (key < sorted_array[middle])
	{
		return (BSearchRecursiveWrapper(sorted_array, low, middle - 1, key));
	}
	else
	{
		return (BSearchRecursiveWrapper(sorted_array, middle + 1, high, key));
	}
}
/******************************************************************************/
int * BSearchRecursive(int sorted_array[], size_t array_size, int key)
{
	assert(sorted_array);

	return (BSearchRecursiveWrapper(sorted_array, ARRAY_BEGIN, ARRAY_END, key));
}
