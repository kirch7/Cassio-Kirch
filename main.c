#include <stdio.h>
#include <stdlib.h>

#include "define.h"
#include "comp_errors.h"
#include "files.h"
#include "boid.h"
#include "box.h"
#include "nextstep.h"
#include "main.h"

double
getGamma (const struct Boid* const boids)
{
  double gamma = 0.0;
  unsigned int endoBoids = (unsigned)(ENDOPROP*N);
  unsigned int boidCount = endoBoids;

  do
    {
      boidCount--;
      if (boids[boidCount].gamma <= 1.01)
        gamma += boids[boidCount].gamma/endoBoids;
      if (boids[boidCount].type == ECTODERM)
        printf("Ecto cell treated as a endo one. %u\n", boidCount);
    }
  while(boidCount != 0);

  return gamma;
}

void
one_system ()
{
  struct Boid  boid[N];
  struct Box   box[BOXES];
  unsigned int boidCount, boxID, threadCount;
  unsigned long long int step, continuousStep = 0;
  //FILE* dat = initializeSingleFile();
  FILE* finalConfigurationFile;
  char finalConfigurationFileName[FILENAME_SIZE];
#ifdef PLOT_EXIT_FILES
  FILE* endoFile;
  FILE* ectoFile;
#endif
#ifdef GAMMA_FILE
  FILE* gammaFile = initializeGammaFile();
#endif
  
  /* Set the pthread_create parameters. */
  struct Parameters parametersStruct[NUM_THREADS];
  void* parameters[NUM_THREADS];

  for (threadCount=0; threadCount<NUM_THREADS; ++threadCount)
    {
      parametersStruct[threadCount].threadID = threadCount;
      parametersStruct[threadCount].left  = threadCount *       \
        ((int)N/(int)NUM_THREADS);
      parametersStruct[threadCount].right = (threadCount+1) *   \
        ((int)N/(int)NUM_THREADS);
      if (threadCount == NUM_THREADS-1)
        parametersStruct[threadCount].right += N%NUM_THREADS;
      parametersStruct[threadCount].boid = boid;
      parametersStruct[threadCount].box  = box;
      parameters[threadCount] = &(parametersStruct[threadCount]);
    }

  for (boidCount=0; boidCount<N; boidCount++)
    {
      initializeBoid (&boid[boidCount]);
      checkLimits(&(boid[boidCount]));
    }
  
  initializeBoxes(box);
  for (boidCount=0; boidCount<N; boidCount++)
    appendBoid(&(boid[boidCount]), box);

  for (step=0; step<STEPS; ++step)
    {
      nextStep(parameters);
      for (boidCount=0; boidCount<N; boidCount++)
        {
          checkLimits(&(boid[boidCount]));
          boxID = getBoxID(boid[boidCount]);
          if (boid[boidCount].boxID != boxID)
            {
              removeBoid(&boid[boidCount], box);
              appendBoid(&boid[boidCount], box);
            }
        }
      if (step%EXIT_INTERVAL == 0)
        {
          printf("Step: %llu\n", step);
#ifdef PLOT_EXIT_FILES
          endoFile = initializeStepAndTypeFile (continuousStep, ENDODERM);
          ectoFile = initializeStepAndTypeFile (continuousStep, ECTODERM);
          for (boidCount=0; boidCount<N; boidCount++)
            {
              /*fprintf(dat, "%llu\t" "%llu\t" "%u\t" "%u\t" "%lf\t" "%lf\n", \
                continuousStep, step, boid[boidCount].type,             \
                boidCount, boid[boidCount].position[X],                 \
                boid[boidCount].position[Y]);
              */
              
              if (boid[boidCount].type == ENDODERM)
                fprintf(endoFile, "%u\t" "%u\t" "%lf\t" "%lf\n",      \
                        boid[boidCount].type,                         \
                        boidCount, boid[boidCount].position[X],       \
                        boid[boidCount].position[Y]);
              
              else
                fprintf(ectoFile, "%u\t" "%u\t" "%lf\t" "%lf\n",      \
                        boid[boidCount].type,                         \
                        boidCount, boid[boidCount].position[X],       \
                        boid[boidCount].position[Y]);
            }
          fclose(endoFile);
          fclose(ectoFile);
#endif
#ifdef GAMMA_FILE
          fprintf(gammaFile, "%llu\t%lf\n", step, getGamma(boid));
#endif
          ++continuousStep;
        }
    }
#ifdef GAMMA_FILE
  fclose (gammaFile);
#endif
  //fclose (dat);

  sprintf(finalConfigurationFileName, "final_%s.dat", getDate());
  finalConfigurationFile = fopen(finalConfigurationFileName, "w");
  for (boidCount=0; boidCount<N; boidCount++)
    {
      fprintf(finalConfigurationFile,                                 \
              "%u\t" "%u\t" "%lf\t" "%lf\n",                          \
              boid[boidCount].type, boidCount,                        \
              boid[boidCount].position[X],                            \
              boid[boidCount].position[Y]);
    }
  fclose (finalConfigurationFile);
}

int
main ()
{
  int returned = checkParameters();
  if (returned != 0)
    return returned;
  setDate();
  srand(time(NULL));
  one_system();
  return 0;
}
