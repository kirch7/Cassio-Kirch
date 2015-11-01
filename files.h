#include "boid.h"


FILE* initializeGodFile ();

FILE* initializeStepAndTypeFile (const unsigned long long int step,     \
                                 const cellType type);

#if defined (ENDO_GAMMA) || defined (ECTO_GAMMA)
FILE* initializeGammaFile();
#endif

#ifdef COUNT_NEIGHBORS
FILE* initializeAverageNeighborsFile ();
#endif
  
FILE* initializeFinalConfigurationFile();

void setDate ();

char* getDate();

void printParameters();
