#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <err.h>

#include "define.h"
#include "boid.h"
#include "box.h"
#include "distance.h"
#include "nextstep.h"

void
checkLimits (struct Boid* const boid)
{
  while (boid -> position[X] < -(RANGE / 2.0))
    boid -> position[X] += RANGE;
  while (boid -> position[X] > (RANGE / 2.0))
    boid -> position[X] -= RANGE;
  while (boid -> position[Y] < -(RANGE / 2.0))
    boid -> position[Y] += RANGE;
  while (boid -> position[Y] > (RANGE / 2.0))
    boid -> position[Y] -= RANGE;
}

void
makeSum (struct Boid* const boid, const struct Boid* conductor, \
         double* const sumF, double* const sumV)
/* Where 'sumF' and 'sumV' are arrays. */
{
  struct Distance distance;
  double force;
  while (conductor != NULL)
  {
    if (boid != conductor)
    {
      distance = getDistance (boid, conductor);
      if (distance.module <= NEIGHBOR_DISTANCE)
      {
        //boid -> neighbors++;
        force = getForce (distance.module);
        
        if (conductor -> type == ENDODERM &&    \
            boid -> type == ENDODERM)
        {
          sumF[X] += BETA11 * force * distance.cosine;
          sumF[Y] += BETA11 * force * distance.sine;
          sumV[X] += ALPHA11 * conductor -> velocity[X] / V0;
          sumV[Y] += ALPHA11 * conductor -> velocity[Y] / V0;
        }
        
        else if (conductor -> type == ECTODERM &&       \
                 boid -> type == ECTODERM)
        {
          sumF[X] += BETA22 * force * distance.cosine;
          sumF[Y] += BETA22 * force * distance.sine;
          sumV[X] += ALPHA22 * conductor -> velocity[X] / V0;
          sumV[Y] += ALPHA22 * conductor -> velocity[Y] / V0;
        }
        
        else
        {
          sumF[X] += BETA12 * force * distance.cosine;
          sumF[Y] += BETA12 * force * distance.sine;
          sumV[X] += ALPHA12 * conductor -> velocity[X] / V0;
          sumV[Y] += ALPHA12 * conductor -> velocity[Y] / V0;
        }

#if defined(ENDO_GAMMA) || defined(ECTO_GAMMA) || defined(COUNT_NEIGHBORS)
        if (conductor -> type == ECTODERM)
          (boid -> ectoNeighbors)++;
        else /* Assume ENDODERM. */
          (boid -> endoNeighbors)++;
#endif

      }
    }
    conductor = conductor -> next;
  }
}

void
setNextVelocity (struct Boid* const boid, const struct Box* boxes)
/* Where 'boid' is address of a single struct and 'boxes' is a array. */
{
  double sumV[] = {0.0, 0.0};    /* For alpha and velocity sum. */
  double uAngle, u[2];    /* For eta and random things. */
  double sumF[] = {0.0, 0.0};    /* For beta and force sum. */
  double sum, sumX, sumY;
  struct Boid *conductor;

#if defined (ENDO_GAMMA) || defined (ECTO_GAMMA) || defined (COUNT_NEIGHBORS)
  boid -> endoNeighbors = 0u;
  boid -> ectoNeighbors = 0u;
  boid -> gamma = 0.0;
#endif
  
  /* check neighbors in the same box */
  conductor = boxes[boid -> boxID].first;
  makeSum(boid, conductor, sumF, sumV);
  
  /* check neighbors in the north box */
  conductor = boxes[getNorthBoxID (boid -> boxID)].first;
    makeSum(boid, conductor, sumF, sumV);
  
  /* check neighbors in the south box */
  conductor = boxes[getSouthBoxID (boid -> boxID)].first;
    makeSum(boid, conductor, sumF, sumV);
  
  /* check neighbors in the west box */
  conductor = boxes[getWestBoxID (boid -> boxID)].first;
    makeSum(boid, conductor, sumF, sumV);
  
  /* check neighbors in the east box */
  conductor = boxes[getEastBoxID (boid -> boxID)].first;
    makeSum(boid, conductor, sumF, sumV);
  
  /* check neighbors in the northeast box */
  conductor = boxes[getEastBoxID (getNorthBoxID (boid -> boxID))].first;
    makeSum(boid, conductor, sumF, sumV);

  /* check neighbors in the southeast box */
  conductor = boxes[getEastBoxID (getSouthBoxID (boid -> boxID))].first;
    makeSum(boid, conductor, sumF, sumV);

  /* check neighbors in the northwest box */
  conductor = boxes[getWestBoxID (getNorthBoxID (boid -> boxID))].first;
    makeSum(boid, conductor, sumF, sumV);

  /* check neighbors in the southwest box */
  conductor = boxes[getWestBoxID (getSouthBoxID (boid -> boxID))].first;
    makeSum(boid, conductor, sumF, sumV);

  uAngle = RANDOM_0 (PI * 2.0);
  u[X] = cos (uAngle);
  u[Y] = sin (uAngle);

  sumX = (ETA * u[X]) + sumV[X] + sumF[X];
  sumY = (ETA * u[Y]) + sumV[Y] + sumF[Y];
  sum = sqrt (pow (sumX, 2.0) + pow (sumY, 2.0));

  boid -> newVelocity[X] = V0 * sumX / sum;
  boid -> newVelocity[Y] = V0 * sumY / sum;

#if defined (ENDO_GAMMA) || defined (ECTO_GAMMA)
  if (boid -> ectoNeighbors != 0u || boid -> endoNeighbors != 0u)
    boid -> gamma = (double)boid -> ectoNeighbors / \
      (boid -> endoNeighbors + boid -> ectoNeighbors);
#endif
}

void
setNextPosition (struct Boid* const boid)
{
  boid -> position[X] += boid -> newVelocity[X] * DT;
  boid -> position[Y] += boid -> newVelocity[Y] * DT;
  boid -> velocity[X] = boid -> newVelocity[X];
  boid -> velocity[Y] = boid -> newVelocity[Y];
}

void*
callNextPositionThread (void* const input)
{
  const struct Parameters* const parameters = input;
  struct Boid* const boids = parameters -> boids;  /* struct Boid array. */
  //struct Box* const boxes  = parameters  ->  boxes;  /* struct Box array. */

  unsigned int boidCount;

  for (boidCount = parameters -> left;
       boidCount < parameters -> right; ++boidCount)
  {
    setNextPosition (boids + boidCount);
    checkLimits(boids + boidCount);
  }
  return NULL;
}

void*
callNextVelocityThread (void *input)
{
  const struct Parameters* const parameters = input;
  struct Boid* const boids = parameters -> boids;    /* struct Boid array. */
  struct Box* const boxes = parameters -> boxes;    /* struct Box array. */
  unsigned int boidCount;

  for (boidCount = parameters -> left;
       boidCount < parameters -> right; ++boidCount)
    setNextVelocity (boids + boidCount, boxes);
  return NULL;
}

void
nextStep (void* const parameters[])
{
  unsigned int threadID;

  for (threadID = 0; threadID < NUM_THREADS; ++threadID)
    pthread_create (&thread[threadID], NULL, callNextVelocityThread,
            parameters[threadID]);
  for (threadID = 0; threadID < NUM_THREADS; ++threadID)
    pthread_join (thread[threadID], NULL);
  for (threadID = 0; threadID < NUM_THREADS; ++threadID)
    pthread_create (&thread[threadID], NULL, callNextPositionThread,
            parameters[threadID]);
  for (threadID = 0; threadID < NUM_THREADS; ++threadID)
    pthread_join (thread[threadID], NULL);
}
