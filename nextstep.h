#include <pthread.h>
#include "define.h"
#include "main.h"


pthread_t thread[NUM_THREADS];

void initializeThreads();

void checkLimits(struct Boid* const boid);

void nextStep (void* const parameters[]);
