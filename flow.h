#include <pthread.h>
#include <unistd.h>

// Delay in microseconds between each execution of the main loop
extern int DELAY;

// Method to run in the timer thread, the format is defined by pthreads.
void *cycle(void *vargp);
