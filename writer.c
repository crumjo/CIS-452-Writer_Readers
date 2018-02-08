/**
 * Echo process communication between a single writer and two readers.
 * 
 * @authors Joshua Crum & Dylan Shoup
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>



int main (int argc, char **argv)
{
	int shmId;
	int size = 4096;
	char *shmPtr;

	/* Create the shared memory segment. */	
	if ((shmId = shmget (IPC_PRIVATE, size, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
	{
		perror("Error: Unable to obtain shared memory.\n");
		exit (1);
	}

	/* Attach the process to the shared memory. */
	if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1)
	{
		perror("Error: Unable to attach to shared memory.\n");
		exit (1);
	}

	
 	
}
