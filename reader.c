/**
 * Echo process communication between a single writer and two readers.
 *
 * @authors Joshua Crum & Dylan Shoup
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "memory.h"


int shmId;
int size = 4096;
struct mem_seg *shmPtr;


/**
 *
 */
void sigHandler (int sigNum)
{
    printf (" interrupt received.\n");
    
    /* Detatch and delete shared segment. */
    printf ("Detatching shared memory segment...\n");
    if (shmdt (shmPtr) < 0)
    {
        perror ("Detatch failed.\n");
        exit (1);
    }
    
    sleep (1);
    printf ("Program will now exit.\n");
    exit (0);
}


int main (int argc, char **argv)
{
    signal(SIGINT, sigHandler);
    key_t shmKey;
    char *path = "key";
    int size = 4096;
    
    /* Create the key. */
    shmKey = ftok (path, 'x');
    
    /* Get the shared memory segment. */
    if ((shmId = shmget (shmKey, size, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
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
    
    printf ("Reader attached to memory.\n");
    
    int turn = shmPtr -> display;
    
    while (1)
    {
        while(shmPtr->display!=turn)
            ;
        printf("Reader found :%s: in shared memory.\n", shmPtr -> msg);
        shmPtr -> display++;
        if(turn == 2)
        {
            shmPtr -> display = 0;
        }
    }
    
    return 0;
}
