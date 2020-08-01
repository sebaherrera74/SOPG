/*TP1 de la materia Sistemas operativos de propositos generales*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "reader.h"
#include "writer.h"


int main(void)
{
    pid_t pid;
	
    //int rv;
   // int sarasa;

    switch(pid = fork())
        {
		case -1:
		perror("fork"); /* something went wrong */
		exit(1);
		break;

		/* Proceso Hijo :Readers */
		case 0:
		printf(" Este es el proceso Hijo \n");
		printf(" My PID is %d\n", getpid());
		/*Aqui va el proceso reader*/
                reader();  
                break;

                /* Proceso Padre :Writers */
		default:
		printf("Este es el proceso padre!\n");
		printf(" My PID is %d\n", getpid());
                /*Aqui va proceso writer*/
                writer();  
                break;
	}
}

