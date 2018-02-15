/**
 * Echo process communication between a single writer and two readers.
 * 
 * @authors Joshua Crum & Dylan Shoup
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>



int main (int argc, char **argv)
{
    key_t shmKey;
    char *path = "/Users/Josh/key";
	int shmId;
	int size = 4096;
	char *shmPtr;
    
    /* Create the key. */
    shmKey = ftok (path, 'x');

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

    printf("Enter a message: ");
    char msg[32];
    fgets (msg, 32, stdin);
    
    /* Remove trailing new line. */
    int len = strlen (msg);
    if (msg[len - 1] == '\n')
    {
        msg[len - 1] = '\0';
    }
    
    /* Write to shared memory. */
    printf ("Writing message '%s' to shared memory...\n", msg);
    memcpy (shmPtr, msg, sizeof(msg));
 	
    /* Deallocate after done accepting input. */
}
