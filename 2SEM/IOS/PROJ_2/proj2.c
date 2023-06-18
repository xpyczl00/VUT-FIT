/**
 * @file proj2.c
 * @author Lukasz Pycz xpyczl00
 * @brief Second project for IOS, shared memory using semaphores
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define MMAP(pointer)                                                                                          \
    {                                                                                                          \
        (pointer) = mmap(NULL, sizeof(*(pointer)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); \
    }
#define UNMAP(pointer)                        \
    {                                         \
        munmap((pointer), sizeof((pointer))); \
    }
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

sem_t *mutex = NULL;
sem_t *wotor_sem = NULL;
sem_t *air_sem = NULL;
sem_t *increment = NULL;
sem_t *bond_sem = NULL;
sem_t *barrier = NULL;
sem_t *Bmutex = NULL;

int *wotor = NULL;
int *air = NULL;
int *counter = NULL;
int *mol_count = NULL;
int *molecules = NULL;
int *increment_c = NULL;
int *end = NULL;
int *max_molecules = NULL;

FILE *file;

void init()
{

    file = fopen("proj2.out", "w");
    setbuf(file, NULL);

    if (file == NULL)
    {
        printf("Error!");
        exit(1);
    }

    if ((mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        printf("sem_open: mutex ");
        exit(EXIT_FAILURE);
    }

    if ((wotor_sem = sem_open("/wotor", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        printf("sem_open: wotor");
        exit(EXIT_FAILURE);
    }

    if ((air_sem = sem_open("/air", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        printf("sem_open: air");
        exit(EXIT_FAILURE);
    }
    if ((increment = sem_open("/increment", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        printf("sem_open: air");
        exit(EXIT_FAILURE);
    }

    if ((bond_sem = sem_open("/bond", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        printf("sem_open: air");
        exit(EXIT_FAILURE);
    }
    if ((Bmutex = sem_open("/Bmutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        printf("sem_open: air");
        exit(EXIT_FAILURE);
    }
    if ((barrier = sem_open("/barrier", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        printf("sem_open: air");
        exit(EXIT_FAILURE);
    }

    MMAP(mol_count);
    MMAP(air);
    MMAP(wotor);
    MMAP(counter);
    MMAP(molecules);
    MMAP(increment_c);
    MMAP(end);
    MMAP(max_molecules);
}
void clean_up()
{
    fclose(file);

    if (sem_close(mutex) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/mutex") == -1)
    {
        perror("sem_unlink mutex");
    }
    if (sem_close(air_sem) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/air") == -1)
    {
        perror("sem_unlink air");
    }
    if (sem_close(wotor_sem) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/wotor") == -1)
    {
        perror("sem_unlink water");
    }
    if (sem_close(increment) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/increment") == -1)
    {
        perror("sem_unlink incerment");
    }
    if (sem_close(bond_sem) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/bond") == -1)
    {
        perror("sem_unlink bond");
    }
    if (sem_close(Bmutex) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/Bmutex") == -1)
    {
        perror("sem_unlink Bmutex");
    }
    if (sem_close(barrier) == -1)
    {
        perror("sem_close");
    }

    if (sem_unlink("/barrier") == -1)
    {
        perror("sem_unlink barrier");
    }

    UNMAP(air);
    UNMAP(wotor);
    UNMAP(counter);
    UNMAP(molecules);
    UNMAP(mol_count);
    UNMAP(increment_c);
    UNMAP(end);
    UNMAP(max_molecules);
}
//function for random time waiting
void my_sleep(int delay)
{
    srand(time(0));
    if (delay != 0)
    {
        usleep(rand() % delay * 1000);
    }
}
//barrier waiting for 2 H and one O
void barrier_wait(int TB)
{
    sem_wait(increment);
    (*mol_count)++;
    sem_post(increment);
    //if all are present releases the barrier for all of them
    if ((*mol_count) % 3 == 0)
    {
        my_sleep(TB);
        sem_post(barrier);
        sem_post(barrier);
        sem_post(barrier);
    }
    //waits for release signal 
    sem_wait(barrier);
}

void create_air(int order, int TI, int TB)
{
    
    // waiting random time from given interval
    my_sleep(TI);

    sem_wait(increment);
    fprintf(file, "%d: O %d: going to queue\n", (*counter)++, order);
    sem_post(increment);

    sem_wait(mutex);
    (*air)++;

    if ((*wotor) >= 2)
    {
        sem_wait(increment);
        (*molecules)++;
        sem_post(increment);

        sem_post(wotor_sem);
        sem_post(wotor_sem);
        (*wotor) -= 2;
        sem_post(air_sem);
        (*air)--;
    }
    else
    {
        sem_post(mutex);
    }
    if ((*max_molecules) == 0)
    {
        sem_post(air_sem);
    }

    sem_wait(air_sem);

    //if all molecules  are already done releases other waiting elements 
    if (((*end) == 9) || (*max_molecules) == 0)
    {
        my_sleep(TB);

        sem_wait(increment);
        fprintf(file, "%d: O %d: not enough H\n", (*counter)++, order);
        sem_post(increment);

        sem_post(air_sem);

        exit(0);
    }

    sem_wait(increment);
    fprintf(file, "%d: O %d: creating molecule %d\n", (*counter)++, order, (*molecules));
    sem_post(increment);

    //barrier for molecule making
    barrier_wait(TB);

    sem_wait(increment);
    fprintf(file, "%d: O %d: molecule %d created\n", (*counter)++, order, (*molecules));
    sem_post(increment);

    sem_post(mutex);
    
    //if all molecules  are already done releases other waiting elements
    if ((*molecules) >= (*max_molecules))
    {
        (*end) = 9;
        sem_post(wotor_sem);
        sem_post(air_sem);
    }

    exit(0);
}

void create_wotor(int order, int TI, int TB)
{
    // waiting random time from given interval
    my_sleep(TI);

    sem_wait(increment);
    fprintf(file, "%d: H %d: going to queue\n", (*counter)++, order);
    sem_post(increment);

    sem_wait(mutex);
    (*wotor)++;

    if ((*wotor) >= 2 && (*air) >= 1)
    {

        sem_wait(increment);
        (*molecules)++;
        sem_post(increment);

        sem_post(wotor_sem);
        sem_post(wotor_sem);
        (*wotor) -= 2;
        sem_post(air_sem);
        (*air)--;
    }
    else
    {
        sem_post(mutex);
    }

    if ((*max_molecules) == 0)
    {
        sem_post(wotor_sem);
    }

    sem_wait(wotor_sem);


    //if all molecules  are already done releases other waiting elements
    if (((*end) == 9) || (*max_molecules) == 0)
    {
        my_sleep(TB);

        sem_wait(increment);
        fprintf(file, "%d: H %d: not enough O or H\n", (*counter)++, order);
        sem_post(increment);

        sem_post(wotor_sem);

        exit(0);
    }

    sem_wait(increment);
    fprintf(file, "%d: H %d: creating molecule %d\n", (*counter)++, order, (*molecules));
    sem_post(increment);
    
    //barirer waiting for elements to molecule
    barrier_wait(TB);

    sem_wait(increment);
    fprintf(file, "%d: H %d: molecule %d created\n", (*counter)++, order, (*molecules));
    sem_post(increment);
    
    //if all molecules  are already done releases other waiting elements
    if ((*molecules) >= (*max_molecules))
    {
        (*end) = 9;
        sem_post(wotor_sem);
        sem_post(air_sem);
    }
    exit(0);
}

void oxygen_process_gen(int NO, int TI, int TB)
{

    for (int i = 1; i <= NO; i++)
    {
        //new process
        pid_t H_pid = fork();

        if (H_pid < 0)
        {
            fprintf(stderr, "problem with fork\n");
            exit(1);
        }
        // if child process
        else if (H_pid == 0)
        {

            sem_wait(increment);
            fprintf(file, "%d: O %d: started\n", (*counter)++, i);
            sem_post(increment);

            // creating O
            create_air(i, TI, TB);
        }
    }
}

void hydrogen_process_gen(int NH, int TI, int TB)
{

    for (int i = 1; i <= NH; i++)
    {
        // new process
        pid_t H_pid = fork();

        if (H_pid < 0)
        {
            fprintf(stderr, "problem with fork\n");
            exit(1);
        }
        // if a child process
        else if (H_pid == 0)
        {
            sem_wait(increment);
            fprintf(file, "%d: H %d: started\n", (*counter)++, i);
            sem_post(increment);

            // creating H
            create_wotor(i, TI, TB);
        }
    }
}

int main(int argc, char **argv)
{
    //############################## checking arguments #############################################################################################
    if (argc != 5)
    {
        fprintf(stderr, "Wrong argument count");
        exit(1);
    }

    int NO = atoi(argv[1]);
    int NH = atoi(argv[2]);
    int TI = atoi(argv[3]);
    int TB = atoi(argv[4]);

    if (NO <= 0 || NH <= 0)
    {
        fprintf(stderr, "invalid hydrogen or oxid count");
        exit(1);
    }
    if (TI < 0 || TI > 1000 || TB < 0 || TB > 1000)
    {
        fprintf(stderr, "invalid time");
        exit(1);
    }
    //#################################################################################################################################################

    // initialization
    init();
    (*counter)++;

    // variable for expected molecules
    (*max_molecules) = MIN((NH / 2), NO);

    // calling generators
    oxygen_process_gen(NO, TI, TB);
    hydrogen_process_gen(NH, TI, TB);

    // waiting for all processes to die
    for (int i = 0; i < NH + NO; i++)
    {
        wait(0);
    }
    // cleaning up
    clean_up();

    return 0;
}