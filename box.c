#include <stdio.h>
#include <stdbool.h>
//#include <err.h>

#include "define.h"
#include "boid.h"
#include "box.h"

void
initializeBoxes (struct Box* const box) /* The input is an array. */
{
  unsigned int boxCount;
  for (boxCount=0; boxCount<BOXES; ++boxCount)
    {
      box[boxCount].boxID = boxCount;
      box[boxCount].first = NULL;
      box[boxCount].last  = NULL;
    }
}

bool
isInEdge (unsigned int boxID)
{
  if      (boxID < BOXES_IN_EDGE)
    return true;
  else if (boxID >= BOXES - BOXES_IN_EDGE)
    return true;
  else if (boxID % BOXES_IN_EDGE == 0)
    return true;
  else if (boxID % BOXES_IN_EDGE == BOXES_IN_EDGE - 1)
    return true;
  else
    return false;
}

/* Append boid to the end of its list. */
void
appendBoid (struct Boid* const boid, struct Box* const box)
{
  struct Boid* conductor;
  boid -> boxID = getBoxID(*boid);
  conductor = box[boid -> boxID].first;
  if (conductor == NULL)
    box[boid -> boxID].first = boid;
  else
    {
      while (conductor -> next != 0)
        conductor = conductor -> next;
      conductor -> next = boid;
    }
  boid -> previous  = conductor;
  boid -> next = NULL;
  box[boid -> boxID].last = boid;
}

void
removeBoid (struct Boid* const boid, struct Box* const box)
{
  struct Boid* boidPointer;
  if (boid == box[boid -> boxID].first)
    box[boid -> boxID].first = boid -> next;
  if (boid == box[boid -> boxID].last)
    box[boid -> boxID].last = boid -> previous;
  boidPointer = boid -> previous;
  if (boidPointer != 0)
    boidPointer -> next = boid -> next;
  boidPointer = boid -> next;
  if (boidPointer != 0)
    boidPointer -> previous = boid -> previous;
}

unsigned int
getSouthBoxID (const unsigned int centralBoxID)
{
  if (centralBoxID >= BOXES_IN_EDGE)
    return (centralBoxID - BOXES_IN_EDGE);
  else
    return (BOXES - BOXES_IN_EDGE + centralBoxID);
}

unsigned int
getNorthBoxID (const unsigned int centralBoxID)
{
  if (centralBoxID < BOXES - BOXES_IN_EDGE)
    return (centralBoxID + BOXES_IN_EDGE);
  else
    return (centralBoxID + BOXES_IN_EDGE - BOXES);
}

unsigned int
getWestBoxID (const unsigned int centralBoxID)
{
  if (centralBoxID % BOXES_IN_EDGE != 0)
    return (centralBoxID - 1);
  else
    return (centralBoxID + BOXES_IN_EDGE - 1);
}

unsigned int
getEastBoxID (const unsigned int centralBoxID)
{
  if (centralBoxID % BOXES_IN_EDGE != BOXES_IN_EDGE - 1)
    return (centralBoxID + 1);
  else
    return (centralBoxID - BOXES_IN_EDGE + 1);
}

