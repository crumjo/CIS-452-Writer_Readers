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
#include "memory.h"


int main (int argc, char **argv)
{
    key_t shmKey;
    char *path = "/Users/Josh/key";
    int shmId;
    int size = 4096;
    struct mem_seg *shmPtr;
    
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
    
    printf ("Reader attached to memory. Display: %d\n", shmPtr -> display);
    int turn;
    
    while (1)
    {
        turn = 0;
//        if (shmPtr -> display == )
        
        while ((shmPtr -> token == 1) || (shmPtr -> display != 0))
            ;
        shmPtr -> token = 1;
        printf("Reader found :%s: in shared memory.\n", shmPtr -> msg);
        shmPtr -> display++;
        shmPtr -> token = 0;
    }
    
    return 0;
}
