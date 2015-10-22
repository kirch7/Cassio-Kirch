#include <stdbool.h>

#ifndef BOX_STRUCT
struct Box
{
  unsigned int boxID;
  struct Boid* first;
  struct Boid* last;
};
#define BOX_STRUCT
#endif

void initializeBoxes (struct Box* const box);

void appendBoid (struct Boid* boid, struct Box* const box);

void removeBoid (struct Boid* boid, struct Box* const box);

unsigned int getSouthBoxID (const unsigned int centralBoxID);

unsigned int getNorthBoxID (const unsigned int centralBoxID);

unsigned int getWestBoxID (const unsigned int centralBoxID);

unsigned int getEastBoxID (const unsigned int centralBoxID);

bool isInEdge (unsigned int boxID);
