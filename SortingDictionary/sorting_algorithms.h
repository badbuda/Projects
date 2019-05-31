#include <stdlib.h> /* size_t */

#include "enum_rd58.h"
#include "function_pointers.h"

#ifndef __SORTING_ALGORITHMS_H__
#define __SORTING_ALGORITHMS_H__

/*******************************************************************************
* NAME: Sorting Algorithms                                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  9.12.18                                                       *
*******************************************************************************/

void BubbleSort(void *elements, size_t element_size, size_t element_count,
                compare_function_t compare);
/*******************************************************************************
* Sorts the data with bubble sort algorithm                                    *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/
void InsertionSort(void *elements, size_t element_size, size_t element_count,
                   compare_function_t compare);
/*******************************************************************************
* Sorts the data with insertion sort algorithm                                 *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/
void SelectionSort(void *elements, size_t element_size, size_t element_count,
                   compare_function_t compare);
/*******************************************************************************
* Sorts the data with selection sort algorithm                                 *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/

typedef size_t (*key_to_num_t)(void *element, void *param);

status CountingSort(void *elements, size_t element_size,size_t element_count,
                  key_to_num_t key_to_num, void *param, size_t range);
/*******************************************************************************
* Sorts the data with counting sort algorithm                                  *
* Parameters:                                                                  *
*                                                                              *
* key_to_num - function that converts data to index                             *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
* param - parameter for key_to_num function                                    *
* range - the range of element values from zero to range                       *
*******************************************************************************/

status RadixSort(void *elements, size_t element_size, size_t element_count,
               key_to_num_t key_to_num, void *param, size_t byte_count);
/*******************************************************************************
* Sorts the data with radix sort algorithm                                     *
* Parameters:                                                                  *
*                                                                              *
* key_to_num - function that convert data to index                             *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
* param - parameter for key_to_num function                                    *
* byte_count - the number of bytes in the max numbers (four bytes at most)     *
*******************************************************************************/

status MergeSort(void *elements, size_t element_size, size_t element_count,
				compare_function_t compare);
/*******************************************************************************
* Sorts the data with Merge sort algorithm                                     *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/

status HeapSort(void *elements, size_t element_size, size_t element_count,
				compare_function_t compare);
/*******************************************************************************
* Sorts the data with Heap sort algorithm                                      *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/

status QuickSort(void *elements, size_t element_size, size_t element_count,
				compare_function_t compare);
/*******************************************************************************
* Sorts the data with Quick sort algorithm                                     *
* Parameters:                                                                  *
*                                                                              *
* compare - comparison function                                                *
* elements - data to be sorted                                                 *
* element_size - size of the element                                           *
* element_count - number of elements                                           *
*******************************************************************************/

int * BSearchIterative(int sorted_array[], size_t array_size, int key);
/*******************************************************************************
* Searches for key in sorted array & returns pointer to key, NULL if not found *
* Parameters:                                                                  *
*                                                                              *
* sorted_array - int array to be searched                                      *
* array_siz - number of ints in array                                          *
* key - key to be searched for in array                                        *
*******************************************************************************/

int * BSearchRecursive(int sorted_array[], size_t array_size, int key);
/*******************************************************************************
* Searches for key in sorted array & returns pointer to key, NULL if not found *
* Parameters:                                                                  *
*                                                                              *
* sorted_array - int array to be searched                                      *
* array_siz - number of ints in array                                          *
* key - key to be searched for in array                                        *
*******************************************************************************/


#endif
