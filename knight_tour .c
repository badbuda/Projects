/*******************************************************************************
*Created BY Bareket Sar Shalom                                                 *
*Description: 								      				
* KnightTour -
*the program  simulates a chess bord size N X N				       *
*the program find a valid course for the knight, so it will step only once     *
*on each square 							       *
*******************************************************************************/
#include <stdio.h> /************printf*****************************************/
#include <stdlib.h> /*************qsort****************************************/
#include <time.h>/*************time_t******************************************/

#include "bit_array.h"
/******************************************************************************/
typedef enum {SUCCESS, FAILED, OUTOFTIME} status;
#define SIZE 8
#define TIMER 5
#define KNIGHT_TOUR_FAILED 0
#define TIMEOUT -1
#define NO_MOVE 0
#define FIRST_RESULT_INDEX 0
#define COUNT_LUT_POSITION 9
#define START_POSITION 1
#define POSSIBLE_MOVES_NUM 8
#define MAX_STEPS_IN_ROW 2
#define BOARD_SIZE SIZE*SIZE
/******************************************************************************/
int lut_steps[BOARD_SIZE + 1][10];
/******************************************************************************/
void PossibleMoves();
status KnightTour(int row, int column, unsigned long BitsLut,
int result_array[], int result_index, time_t timer);
int *Start(int begin_index, int BitsLut, int result_array[]);
/******************************************************************************/
int IsLimit(int row, int column)
{
	return((0 < row && 0 < column) && (row <= SIZE && column <= SIZE));
}
/******************************************************************************/
static int CompareInt(const void* before, const void* after)
{
	if (0 == *(int*)before || 0 == *(int*)after)
	{
		return 0;
	}

	return ((lut_steps[*(int*)before])[COUNT_LUT_POSITION] - 
				(lut_steps[*(int*)after])[COUNT_LUT_POSITION]);
}
/******************************************************************************/
void Print(int *result_array)
{
	int i = 0, j = 0;

	for (i = 0; i < SIZE; ++i)
	{
		for (j = 0; j < SIZE; ++j)
		{
			printf(" %d", result_array[(i * SIZE) + j]);
		}
	}

	printf("\n");
}
/******************************************************************************/
int *Start(int begin_index, int BitsLut, int result_array[])
{
    int row = 0;
    int column = 0;
		status status;
    result_array[0] = begin_index;

    row = begin_index % SIZE;
    column = ((begin_index - row + SIZE)/SIZE);

		status = KnightTour(row, column, BitsLut, result_array,
																								FIRST_RESULT_INDEX, time(NULL));

		if (status == FAILED)
		{
			result_array[0] = KNIGHT_TOUR_FAILED;
		}

		if (status == OUTOFTIME)
		{
			result_array[0] = TIMEOUT;
		}

  	return (result_array);
}
/******************************************************************************/
status KnightTour(int row, int column, unsigned long BitsLut,
						int result_array[], int result_index, time_t timer)
{
  int move_result = 1;
  int index = 0;
	int i = 0;
	status status = SUCCESS;

	index = SIZE * (column - 1) + row;
	result_array[result_index] = index;
	BitsLut = BitArraySetOn(BitsLut, index);

	if (time(NULL) - timer > TIMER)
	{
		return (OUTOFTIME);
	}

	if (BitArrayCountOn(BitsLut) == BOARD_SIZE)
	{
	return (SUCCESS);
	}

	for (i = 0, move_result = (lut_steps[index])[0]; move_result != NO_MOVE; ++i,
	 						move_result = (lut_steps[index])[i])
	{
		if (BitArrayIsOff(BitsLut, move_result))
		{
			row = move_result % SIZE;
			column = ((move_result - row + SIZE) / SIZE);
			status = KnightTour(row, column, BitsLut, result_array,
			result_index + 1, timer);
			if (status != FAILED)
			{
				return (status);
			}
		}
	}
	return (FAILED);
}
/******************************************************************************/
void Rearange()
{
	int j = 0, i = 0;
	int count = 0;

	for (i = START_POSITION; i <= BOARD_SIZE; ++i)
	{
		for (j = 0; j < POSSIBLE_MOVES_NUM; ++j)
		{
			if ((lut_steps[i])[j] != NO_MOVE)
			{
				++count;
			}
		}
		(lut_steps[i])[COUNT_LUT_POSITION] = count;
		count = 0;
	}

	for (i = START_POSITION; i <= BOARD_SIZE; ++i)
	{
		qsort(lut_steps[i], POSSIBLE_MOVES_NUM, sizeof(int), CompareInt);
	}
}
/******************************************************************************/
void PossibleMoves()
{
	int i = 0, j = 0, k = 0;
	int next_possible_row = 0, next_possible_column = 0, index = 0;
	int current_row = 0, current_column = 0;
	int row_move[POSSIBLE_MOVES_NUM] = {2, 1, -1, -2, -2, -1,  1,  2 };
	int column_move[POSSIBLE_MOVES_NUM] = {1, 2,  2,  1, -1, -2, -2, -1 };

	for (i = START_POSITION; i <= BOARD_SIZE; ++i)
	{
		k = 0;
		for (j = 0; j < POSSIBLE_MOVES_NUM; ++j)
		{
			/*get the x,y of the position you are stending on*/
			current_row = i % SIZE;
			current_column = ((i - current_row + SIZE) / SIZE);

			/*find the x,y of the next possible position from where you are stending*/
			next_possible_row = current_row + row_move[j];
			next_possible_column = current_column + column_move[j];

			index = SIZE * (next_possible_column - 1) + next_possible_row;

			/*spetial check for the last element in the each row*/
			if (0 == (i % SIZE) && i != START_POSITION)
			{
				current_row = index % SIZE;
				current_column = ((index - current_row + SIZE) / SIZE);
				/*check that next element distance is max 2 places away*/
				if (IsLimit(current_row, current_column) && (SIZE <= current_row + MAX_STEPS_IN_ROW))
				{
					(lut_steps[i])[k] = index;
					++k;
				}
			}
			else if (IsLimit(next_possible_row,next_possible_column))
			{
				(lut_steps[i])[k] = index;
				++k;
			}
		}
	}
	Rearange();
}
/******************************************************************************/
int main()
{
	int result_array[BOARD_SIZE];
	int i = 0;
	static unsigned long BitsLut = 0;

	PossibleMoves();

	for (i = START_POSITION; i <= BOARD_SIZE; ++i)
	{
		Start(i, BitsLut, result_array);
		printf("start point: %d|", i);

		if (result_array[0] != KNIGHT_TOUR_FAILED && result_array[0] != TIMEOUT)
		{
			Print(result_array);
		}
		else if (result_array[0] == KNIGHT_TOUR_FAILED)
		{
			printf("NO SOLUTION\n");
		}
		else
		{
			printf("OUT OF TIME\n");
		}
	}

	return (0);
}
