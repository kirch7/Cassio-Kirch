#include "boid.h"

FILE* initializeSingleFile ();

FILE* initializeStepAndTypeFile (const unsigned long long int step,     \
                                 const cellType type);
FILE* initializeGammaFile();

void setDate ();

char* getDate();
