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
#include <sys/wait.h>
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
    
    printf ("Deleting shared memory segment...\n");
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("Deallocate failed.\n");
        exit (1);
    }
    
    sleep (1);
    printf ("Program will now exit.\n");
    exit (0);
}


/**
 *
 */
int main (int argc, char **argv)
{
    signal(SIGINT, sigHandler);
    key_t shmKey;
    char *path = "key";
    
    /* Create the key. */
    shmKey = ftok (path, 'x');
    
    /* Create the shared memory segment. */
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
    
    /**
     * I basically used the turn variable and display variable to traverse
     * the writer, then 1 reader, and then the other in order every time.
     */
    
    shmPtr -> display = 0;
    
    int turn = 0;
    while (1)
    {
        
        while(shmPtr ->display!=turn)
            ;
        //shmPtr -> token = 1;
        printf("Enter a message: ");
        char wmsg[32];
        fgets (wmsg, 32, stdin);
        
        /* Remove trailing new line. */
        int len = strlen (wmsg);
        if (wmsg[len - 1] == '\n')
        {
            wmsg[len - 1] = '\0';
        }
        
        /* Write to shared memory. */
        printf ("Writing message '%s' to shared memory...\n", wmsg);
        memcpy (shmPtr -> msg, wmsg, sizeof(wmsg));
        shmPtr -> display++;
    }
    return 0;
}
