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
         double* const sumFX, double* const sumFY,              \
         double* const sumVX, double* const sumVY)
{
  struct Distance distance;
  double force;
  while (conductor != NULL)
    {
      if (boid != conductor)
        {
          distance = getDistance (boid, conductor);
          if (distance.module < NEIGHBOR_DISTANCE)
            {
              boid -> neighbors++;
              force = getForce (distance.module);

              if (conductor -> type == ENDODERM &&      \
                  boid -> type == ENDODERM)
                {
                  *sumFX += BETA11 * force * distance.cosine;
                  *sumFY += BETA11 * force * distance.sine;
                  *sumVX += ALPHA11 * conductor -> velocity[X] / V0;
                  *sumVY += ALPHA11 * conductor -> velocity[Y] / V0;
                }

              else if (conductor -> type == ECTODERM && \
                       boid -> type == ECTODERM)
                {
                  *sumFX += BETA22 * force * distance.cosine;
                  *sumFY += BETA22 * force * distance.sine;
                  *sumVX += ALPHA22 * conductor -> velocity[X] / V0;
                  *sumVY += ALPHA22 * conductor -> velocity[Y] / V0;
                }

              else
                {
                  *sumFX += BETA12 * force * distance.cosine;
                  *sumFY += BETA12 * force * distance.sine;
                  *sumVX += ALPHA12 * conductor -> velocity[X] / V0;
                  *sumVY += ALPHA12 * conductor -> velocity[Y] / V0;
                  if (conductor -> type == ECTODERM) /* And boid->type==ENDO */
                    boid -> ectoNeighbors++;
                }
            }
        }
      conductor = conductor -> next;
    }
  if(boid -> neighbors != 0u)
    boid -> gamma = (double)boid -> ectoNeighbors / boid -> neighbors;
  else
    boid -> gamma = 1000000.0;
}

void
setNextVelocity (struct Boid* const boid, const struct Box box[])
{
  double sumVX = 0.0, sumVY = 0.0;    /* For alpha and velocity sum. */
  double uAngle, uX, uY;    /* For nu and random things. */
  double sumFX = 0.0, sumFY = 0.0;    /* For beta and force sum. */
  double sum, sumX, sumY;
  struct Boid *conductor;
  
  boid -> neighbors = 0;
  boid -> ectoNeighbors = 0;
  boid -> gamma = 0.0;

  /* check neighbors in the same box */
  conductor = box[boid -> boxID].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);
  
  /* check neighbors in the north box */
  conductor = box[getNorthBoxID (boid -> boxID)].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the south box */
  conductor = box[getSouthBoxID (boid -> boxID)].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the west box */
  conductor = box[getWestBoxID (boid -> boxID)].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the east box */
  conductor = box[getEastBoxID (boid -> boxID)].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the northeast box */
  conductor = box[getEastBoxID (getNorthBoxID (boid -> boxID))].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the southeast box */
  conductor = box[getEastBoxID (getSouthBoxID (boid -> boxID))].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the northwest box */
  conductor = box[getWestBoxID (getNorthBoxID (boid -> boxID))].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  /* check neighbors in the southwest box */
  conductor = box[getWestBoxID (getSouthBoxID (boid -> boxID))].first;
  makeSum(boid, conductor, &sumFX, &sumFY, &sumVX, &sumVY);

  uAngle = RANDOM_0 (PI * 2.0);
  uX = cos (uAngle);
  uY = sin (uAngle);

  sumX = (double) ETA * uX + sumVX + sumFX;
  sumY = (double) ETA * uY + sumVY + sumFY;
  sum = sqrt (pow (sumX, 2.0) + pow (sumY, 2.0));

  boid -> newVelocity[X] = V0 * sumX / sum;
  boid -> newVelocity[Y] = V0 * sumY / sum;
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
  struct Boid* const boid = parameters -> boid;  /* struct Boid array. */
  //struct Box* const box  = parameters  ->  box;  /* struct Box array. */

  unsigned int boidCount;

  for (boidCount = parameters -> left;
       boidCount < parameters -> right; ++boidCount)
    setNextPosition ((boid + boidCount));
  return NULL;
}

void*
callNextVelocityThread (void *input)
{
  const struct Parameters* const parameters = input;
  struct Boid* const boid = parameters -> boid;    /* struct Boid array. */
  struct Box* const box = parameters -> box;    /* struct Box array. */
  unsigned int boidCount;

  /*for (boidCount = parameters  ->  left;      \
     boidCount < parameters  ->  right;       \
     ++boidCount)
     checkLimits(boid+boidCount); */

  for (boidCount = parameters -> left;
       boidCount < parameters -> right; ++boidCount)
    setNextVelocity (boid + boidCount, box);
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
