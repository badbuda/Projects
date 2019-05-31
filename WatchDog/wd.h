#pragma once
#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

#include "enum_rd58.h"

/*******************************************************************************
* NAME: Watch Dog                                                              *
*                                                                              *
*Created BY Bareket Sar Shalom                                                 *
* DATE UPDATED:  22.01.19                                                      *
*******************************************************************************/

status KeepMeAlive(char *argv[]);
/*******************************************************************************
* Initiate watch dog                                                           *
*                                                                              *
* Parameters:                                                                  *
*  argv[]  - arguments of calling process                                      *
*                                                                              *
* Returns:                                                                     *
*  FAIL/SUCCESS                                                                *
*******************************************************************************/

void DNR();
/*******************************************************************************
* Terminates watch dog                                                         *
*                                                                              *
* Parameters:                                                                  *
*                                                                              *
* Returns:                                                                     *
*  none                                                                        *
*******************************************************************************/


#endif
