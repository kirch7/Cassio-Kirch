#include <math.h>
#include <stdbool.h>
#include "define.h"
#include "box.h"
#include "boid.h"
#include "distance.h"

struct Distance
getDistance (const struct Boid* const boid2, const struct Boid* const boid1)
{
  struct Distance distance;
  double temp;
  
  /* Distância normal. */
  distance.module = absDistance (boid1 -> position[X],  \
                                 boid1 -> position[Y],  \
                                 boid2 -> position[X],  \
                                 boid2 -> position[Y]);
  if (distance.module < NEIGHBOR_DISTANCE)
    {
      distance.sine = (boid2 -> position[Y] - boid1 -> position[Y]) /   \
        distance.module;
      distance.cosine = (boid2 -> position[X] - boid1 -> position[X]) / \
        distance.module;
      return distance;
    }
  
  else if ((isInEdge (boid1 -> boxID) == true) ||       \
           (isInEdge (boid2 -> boxID) == true))
    {
      
      /* 1o boid à esquerda. */
      temp = boid1 -> position[X] + RANGE;
      distance.module = absDistance (temp,                              \
                                     boid1 -> position[Y],              \
                                     boid2 -> position[X],              \
                                     boid2 -> position[Y]);
      if (distance.module < NEIGHBOR_DISTANCE)
        {
          distance.sine = (boid2 -> position[Y] - boid1 -> position[Y]) / \
            distance.module;
          distance.cosine = (boid2 -> position[X] - temp) / distance.module;
          return distance;
        }
      
      /* 1o boid à direita */
      temp = boid1 -> position[X] - RANGE;
      distance.module = absDistance (temp,                              \
                                     boid1 -> position[Y],              \
                                     boid2 -> position[X],              \
                                     boid2 -> position[Y]);
      if (distance.module < NEIGHBOR_DISTANCE)
        {
          distance.sine = (boid2 -> position[Y] - boid1 -> position[Y]) / \
            distance.module;
          distance.cosine = (boid2 -> position[X] - temp) / distance.module;
          return distance;
        }
      
      /* 1o boid abaixo */
      temp = boid1 -> position[Y] + RANGE;
      distance.module = absDistance (boid1 -> position[X],              \
                                     temp,                              \
                                     boid2 -> position[X],              \
                                     boid2 -> position[Y]);
      if (distance.module < NEIGHBOR_DISTANCE)
        {
          distance.sine = (boid2 -> position[Y] - temp) / distance.module;
          distance.cosine = (boid2 -> position[X] - boid1 -> position[X]) / \
            distance.module;
          return distance;
        }
      
      /* 1o boid acima */
      temp = boid1 -> position[Y] - RANGE;
      distance.module = absDistance (boid1 -> position[X],              \
                                     temp,                              \
                                     boid2 -> position[X],              \
                                     boid2 -> position[Y]);
      if (distance.module < NEIGHBOR_DISTANCE)
        {
          distance.sine = (boid2 -> position[Y] - temp) / distance.module;
          distance.cosine = (boid2 -> position[X] - boid1 -> position[X]) / \
            distance.module;
          return distance;
        }
      
      /* não vizinhos */
      else
        {
          distance.sine = 0.0;
          distance.cosine = 0.0;
          distance.module = NEIGHBOR_DISTANCE * 2.0;
          return distance;
        }
    }
  
  /* não vizinhos */
  else
    {
      distance.sine = 0.0;
      distance.cosine = 0.0;
      distance.module = NEIGHBOR_DISTANCE * 2.0;
      return distance;
    }
}

/* Supõe-se que a função só será chamada para boids vizinhos. */
double
getForce (const double distance)
{
  if (distance < CORE_RADIUS)
    return INFINITE_FORCE;
  //else if (distance > NEIGHBOR_DISTANCE)
  //return 0.0;
  else
    return (1.000000 - (distance / ELASTIC_DISTANCE));
}
