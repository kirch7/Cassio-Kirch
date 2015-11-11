#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define.h"
#include "comp_errors.h"
#include "files.h"
#include "boid.h"
#include "box.h"
#include "nextstep.h"
#include "main.h"

#ifdef ENDO_GAMMA
  #define GAMMA_FILE
#endif

#ifdef ECTO_GAMMA
  #ifndef GAMMA_FILE
    #define GAMMA_FILE
  #endif
#endif

#ifdef COUNT_NEIGHBORS
double
getAverageNeighborsNo(const struct Boid* const boid)
/* Where 'boid' is address of the first element of an array. */
{
  unsigned int boidCount;
  double averageNeighborsNo = 0.0;
  for (boidCount = 0; boidCount < N; ++boidCount)
    averageNeighborsNo += ((double)boid[boidCount].endoNeighbors +  \
                           (double)boid[boidCount].ectoNeighbors);
  averageNeighborsNo /= N;
  return averageNeighborsNo;
}
#endif

#if defined(ENDO_GAMMA) || defined(ECTO_GAMMA)
double
getGamma (const struct Boid* const boids)
{
  double gamma = 0.0;
  unsigned int boidCount;
  unsigned int endoBoids = (unsigned)(ENDOPROP*N);
  //  printf("EndoBoids: %u\n", endoBoids);
  
#ifdef ENDO_GAMMA
  for (boidCount = endoBoids; boidCount < N; --boidCount)
  /* Considering overflow. */
    gamma += boids[boidCount].gamma;
#endif
  //  printf("BoidCount: %u\n", boidCount);
  

#ifdef ECTO_GAMMA
  for (boidCount = endoBoids + 1; boidCount < N; ++boidCount)
    gamma += boids[boidCount].gamma;
#endif

#ifdef ENDO_GAMMA
  #ifdef ECTO_GAMMA
  gamma /= N;
  #else
  gamma /= endoBoids;
  #endif
#else
  #ifdef ECTO_GAMMA
  gamma /= (N-endoBoids);
  #endif
#endif
  
  return gamma;
}
#endif /* #if defined(ENDO_GAMMA) || defined(ECTO_GAMMA) */

void
one_system ()
{
  struct Boid  boids[N];
  struct Box   boxes[BOXES];
  unsigned int boidCount, boxID, threadCount;
  unsigned long long int step, continuousStep = 0;

  FILE* godFile = initializeGodFile();
  FILE* finalConfigurationFile;

#ifdef PLOT_EXIT_FILES
  FILE* endoFile;
  FILE* ectoFile;
#endif

#ifdef GAMMA_FILE
  FILE* gammaFile = initializeGammaFile();
#endif

#ifdef COUNT_NEIGHBORS
  FILE* averageNeighborsFile = initializeAverageNeighborsFile();
#endif
  
  /* Set the pthread_create parameters. */
  struct Parameters parametersStruct[NUM_THREADS];
  void* parameters[NUM_THREADS];

  for (threadCount=0; threadCount<NUM_THREADS; ++threadCount)
  {
    parametersStruct[threadCount].threadID = threadCount;
    parametersStruct[threadCount].left  = threadCount * \
      ((int)N/(int)NUM_THREADS);
    parametersStruct[threadCount].right = (threadCount+1) *     \
      ((int)N/(int)NUM_THREADS);
    if (threadCount == NUM_THREADS-1)
      parametersStruct[threadCount].right += N%NUM_THREADS;
    parametersStruct[threadCount].boids = boids;
    parametersStruct[threadCount].boxes  = boxes;
    parameters[threadCount] = &(parametersStruct[threadCount]);
  }
  
  for (boidCount=0; boidCount<N; boidCount++)
  {
    initializeBoid (&boids[boidCount]);
    checkLimits(&(boids[boidCount]));
  }
  
  initializeBoxes(boxes);
  for (boidCount=0; boidCount<N; boidCount++)
    appendBoid(&(boids[boidCount]), boxes);
  
  for (step=0; step <= STEPS; ++step)
  {
    nextStep(parameters);
    for (boidCount=0; boidCount<N; boidCount++)
    {
      checkLimits(&(boids[boidCount]));
      boxID = getBoxID(boids[boidCount]);
      if (boids[boidCount].boxID != boxID)
      {
        removeBoid(&boids[boidCount], boxes);
        appendBoid(&boids[boidCount], boxes);
      }
    }
    if (step%EXIT_INTERVAL == 0)
    {
      printf("Step: %llu\n", step);

#ifdef COUNT_NEIGHBORS
      fprintf(averageNeighborsFile, "%llu\t%lf\n", step, getAverageNeighborsNo(boids));
#endif
      
#ifdef PLOT_EXIT_FILES
      endoFile = initializeStepAndTypeFile (continuousStep, ENDODERM);
      ectoFile = initializeStepAndTypeFile (continuousStep, ECTODERM);
      for (boidCount=0; boidCount<N; boidCount++)
      {
        /*fprintf(godFile, "%llu\t" "%llu\t" "%u\t" "%u\t"              \
          "%lf\t" "%lf\n", continuousStep, step,                        \
          boids[boidCount].type, boidCount,                              \
          boids[boidCount].position[X],                                  \
          boids[boidCount].position[Y]);
        */
        
        if (boids[boidCount].type == ENDODERM)
          fprintf(endoFile, "%u\t" "%u\t" "%lf\t" "%lf\n",            \
                  boids[boidCount].type,                               \
                  boidCount, boids[boidCount].position[X],             \
                  boids[boidCount].position[Y]);
        
        else
          fprintf(ectoFile, "%u\t" "%u\t" "%lf\t" "%lf\n",            \
                  boids[boidCount].type,                               \
                  boidCount, boids[boidCount].position[X],             \
                  boids[boidCount].position[Y]);
      }
      fclose(endoFile);
      fclose(ectoFile);
#endif
#ifdef GAMMA_FILE
      fprintf(gammaFile, "%llu\t%lf\n", step, getGamma(boids));
#endif
      ++continuousStep;
    }
  }
#ifdef GAMMA_FILE
  fclose (gammaFile);
#endif

#ifdef COUNT_NEIGHBORS
  fclose(averageNeighborsFile);
#endif
  
  fclose (godFile);
    
  finalConfigurationFile = initializeFinalConfigurationFile();
  for (boidCount=0; boidCount<N; boidCount++)
  {
    fprintf(finalConfigurationFile,                                   \
            "%u\t" "%u\t" "%lf\t" "%lf\n",                            \
            boids[boidCount].type, boidCount,                          \
            boids[boidCount].position[X],                              \
            boids[boidCount].position[Y]);
  }
  fclose (finalConfigurationFile);
}

void
printHelp()
{
  printf("-h\t" "Show this help menu.\n");
  printf("-p\t" "Show the parameters compiled.\n");
  
}

int
main (int argc, char** argv)
{
  setDate();
  if (argc > 1)
  {
    if (strcmp(argv[1], "-p") == 0)
      printParameters();
    else
      printHelp();
  }
  else
  {
    int returned = checkParameters();
    if (returned != 0)
      return returned;
    srand(time(NULL));
    one_system();
  }
  return 0;
}
