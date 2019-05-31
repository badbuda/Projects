#pragma once

/*******************************************************************************
* NAME: enum_rd58                                                              *
*                                                                              *
* PURPOSE: define custom enums for rd58 projects                               *
*                                                                              *
* AUTHOR: Yaakov Sidikman                                                      *
* DATE UPDATED:  14.11.18                                                      *
*******************************************************************************/


typedef enum {
	SUCCESS,
	FAIL,
	MEMORY_ALLOCATION_FAIL,
	ACTION_FAIL,
	STOP_TASK,
	TASK_FAIL
} status;
typedef enum {FALSE, TRUE} bool;
