/*Includes*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>



#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300

/*defino signalNumber*/
volatile sig_atomic_t signalNumber;


void handler_sig1(int sig)
{
	signalNumber = 1;
}

void handler_sig2(int sig)
{
	signalNumber = 2;
}

int main(){

//printf(" Proceso Writer\n");
//printf(" PRUEBA WRITER\n");

    char outputBuffer[BUFFER_SIZE];
    uint32_t bytesWrote;
    int32_t returnCode, fd;
    struct sigaction sa;
    
/* Accion de señal : SIGUSR1 */
    	sa.sa_handler = handler_sig1;
	sa.sa_flags = 0; // SA_RESTART; //

    if( sigemptyset( &sa.sa_mask ) == -1)
    {
        perror( "sigemptyset" );
        exit( 1 );
    }
    if( sigaction( SIGUSR1, &sa, NULL ) == -1 )
    {
        perror( "sigaction" );
        exit( 1 );
    }
	
/* Accion de señal : SIGUSR2 */
	sa.sa_handler = handler_sig2;
	sa.sa_flags = 0; // SA_RESTART; //

    if( sigemptyset( &sa.sa_mask ) == -1)
    {
        perror( "sigemptyset" );
        exit( 1 );
    }
    if( sigaction( SIGUSR2, &sa, NULL ) == -1 )
    {
        perror( "sigaction" );
        exit( 1 );
    }
	

    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for readers...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a reader--type some stuff\n");

    /* Loop forever */
   while (1)
   {
                
        /* Get some text from console */
   fgets(outputBuffer, BUFFER_SIZE, stdin);
  
   char auxBuffer[BUFFER_SIZE+sizeof( "DATA:" )];   

    if( signalNumber != 0 )
        {
            sprintf( auxBuffer, "SIGN:%d\n", signalNumber); //Agrego SIGN: si es un dato 
            signalNumber = 0;
        }
    else   
        {
          sprintf( auxBuffer, "DATA:%s", outputBuffer );//Agrego DATA: si es un dato 
        } 
        

        /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
	if ((bytesWrote = write(fd,  auxBuffer, strlen(auxBuffer)-1)) == -1)
        {
		perror("write");
        }
        else
        {
		printf("writer: wrote %d bytes\n", bytesWrote);
        }
}	
return 0;
}


