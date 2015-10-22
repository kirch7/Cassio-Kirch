#include "boid.h"
#include "box.h"

#ifndef PARAMETERS_STRUCT
struct Parameters
{
  unsigned int threadID;
  unsigned int left;
  unsigned int right;
  struct Boid* boid;
  struct Boid* firstEndoBoid;
  struct Box*  box;
};
#define PARAMETERS_STRUCT
#endif
