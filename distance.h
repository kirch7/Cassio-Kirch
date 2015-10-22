#ifndef DISTANCE_STRUCT
struct Distance
{
  double module;
  double sine;
  double cosine;
};
#define DISTANCE_STRUCT
#endif

double getForce (const double distance);

struct Distance getDistance (const struct Boid* const boid2,    \
                             const struct Boid* const boid1);
