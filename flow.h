#include <pthread.h>
#include <unistd.h>

extern int DELAY;
void *cycle(void *vargp);
