#include "flow.h"

int DELAY = 10000;

void *cycle(void *vargp) {
	usleep(DELAY);
	return NULL;
}
