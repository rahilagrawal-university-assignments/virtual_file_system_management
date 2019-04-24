#include "opt-synchprobs.h"
#include <types.h>  /* required by lib.h */
#include <lib.h>    /* for kprintf */
#include <synch.h>  /* for P(), V(), sem_* */
#include <thread.h> /* for thread_fork() */
#include <test.h>

#include "dining_driver.h"

/*
 * Declare any data structures you might need to synchronise 
 * your forks here.
 */

struct semaphore *forks[NUM_PHILOSOPHERS]; 
struct semaphore *fork; 


/*
 * Take forks ensures mutually exclusive access to two forks
 * associated with the philosopher.
 * 
 * The left fork number = phil_num
 * The right fork number = (phil_num + 1) % NUM_PHILOSPHERS
 */

void take_forks(unsigned long phil_num)
{
    P(fork);
    P(forks[phil_num]);
    P(forks[(phil_num + 1) % NUM_PHILOSOPHERS]);
}


/*
 * Put forks releases the mutually exclusive access to the
 * philosophers forks.
 */

void put_forks(unsigned long phil_num)
{
    V(forks[phil_num]);
    V(forks[(phil_num + 1) % NUM_PHILOSOPHERS]);
    V(fork);
}

/* 
 * Create gets called before the philosopher threads get started.
 * Insert any initialisation code you require here.
 */

void create_forks()
{
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        forks[i] = sem_create("forks", 1);
        if(!forks[i]) panic("create_forks: couldn't create semaphore\n");
    }
    fork = sem_create("fork", NUM_PHILOSOPHERS/2);
    if(!fork) panic("create_forks: couldn't create semaphore\n");
}


/*
 * Destroy gets called when the system is shutting down.
 * You should clean up whatever you allocated in create_forks()
 */

void destroy_forks()
{
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) sem_destroy(forks[i]);
    sem_destroy(fork);
}
