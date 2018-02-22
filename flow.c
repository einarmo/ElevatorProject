#include "flow.h"

DELAY = 10000;

void *cycle(void *vargp) {
    usleep(DELAY);
    return NULL;
}
