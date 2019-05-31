/*******************************************************************************
* NAME: WD_MAIN                                                                   *
*                                                                              *
*Created BY Bareket Sar Shalom                                                 *
* DATE UPDATED:  22.01.19                                                      *
*******************************************************************************/
#include <stdio.h> /* fprintf */
#include <pthread.h>
#include <unistd.h>
#include "wd.h" /* "malloc" "free" "size_t" */

extern int stop_flag;

int main(int argc, char *argv[])
{
	(void)argc;

	KeepMeAlive(argv);
	sleep(15);
	while (stop_flag == 0)
	{
		sleep(14);
		printf("WD MAIN\n");
	}


	return (0);
}
