#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "define.h"
#include "boid.h"

unsigned int
getBoxID (struct Boid boid)
{
  unsigned int tmpBoxID;

  boid.position[X] += 0.50000*RANGE;
  boid.position[Y] += 0.50000*RANGE;

  tmpBoxID = (unsigned int) (boid.position[X] *                         \
                             ((double) BOXES_IN_EDGE/RANGE)) +     \
    BOXES_IN_EDGE * (unsigned int) (boid.position[Y] *                  \
                                   (double) BOXES_IN_EDGE/RANGE);
  if (boid.position[X] == RANGE)
    tmpBoxID -= 1;
  if (boid.position[Y] == RANGE)
    tmpBoxID -= BOXES_IN_EDGE;
  return tmpBoxID;
}

void
initializeCircleCorrectly(struct Boid* const boid)
{
  double angle, radius;
  angle  = RANDOM_0(PI*2.0);
  radius = sqrt(RANDOM_0(1.0)) *                \
    sqrt(N * 0.137832224) * ELASTIC_DISTANCE;
  /* R^2 = re^2 * N * sqrt(3) / (4 pi) */
  boid -> position[X] = radius * cos(angle);
  boid -> position[Y] = radius * sin(angle);
}

void
initializeCircle (struct Boid* const boid)
{
  double angle, radius;
  angle               = RANDOM_0(PI*2.0);
  radius              = sqrt(RANDOM_0(1.0)) * RANGE * PROP * 0.500;
  boid -> position[X] = radius*cos(angle);
  boid -> position[Y] = radius*sin(angle);
}

void
initializeRectangle (struct Boid* const boid)
{
  boid -> position[X] = -PROP*RANGE*0.5 + RANDOM_0(PROP * RANGE);
  boid -> position[Y] = -PROP*RANGE*0.5 + RANDOM_0(PROP * RANGE);
}

void
initializeBoid (struct Boid* const boid)
{
  static unsigned int boidCount = 0;
  static unsigned int endoBoids = (unsigned)(ENDOPROP*N);
  initializeCircleCorrectly (boid);
  boid -> velocity[X]  = 0.0;
  boid -> velocity[Y]  = 0.0;
  boid -> boxID        = getBoxID (*boid);
  boid -> next         = NULL;
  boid -> previous     = NULL;

#if defined (ENDO_GAMMA) || defined (ECTO_GAMMA)
  boid -> gamma = 0.0;
#endif
  
  if (boidCount < endoBoids)
    boid -> type = ENDODERM;
  else
    boid -> type = ECTODERM;
  ++boidCount;
}
