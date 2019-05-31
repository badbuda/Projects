/*******************************************************************************
* NAME: 24/7                                                                   *
*                                                                              *
*Created BY Bareket Sar Shalom                                                 *
* DATE UPDATED:  22.01.19                                                      *
*******************************************************************************/
#include <stdio.h> /* fprintf */
#include <pthread.h> /* fprintf */
#include <unistd.h>

#include "wd.h" /* "malloc" "free" "size_t" */



int main(int argc, char *argv[])
{
	KeepMeAlive(argv);

	sleep(15);
	sleep(15);
	sleep(15);
	sleep(15);

	DNR();

	sleep(15);

	sleep(15);

/*  while(1)
  {
    sleep(2);
    printf("I love lola 24/7\n");
 }*/
}
