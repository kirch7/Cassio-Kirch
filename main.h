#include "boid.h"
#include "box.h"

#ifndef PARAMETERS_STRUCT
struct Parameters
{
  unsigned int threadID;
  unsigned int left;
  unsigned int right;
  struct Boid* boids;
  struct Boid* firstEndoBoid;
  struct Box*  boxes;
};
#define PARAMETERS_STRUCT
#endif
