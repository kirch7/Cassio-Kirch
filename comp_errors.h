#if (BOXES_IN_EDGE*BOXES_IN_EDGE)!=BOXES
#error : BOXES_IN_EDGE**2 must be equal to BOXES.
#endif

/*#if (BOXES_IN_EGDE*NEIGHBOUR_DISTANCE)>(2*RANGE)
#error : Only the 8 neighbour boxes can contain neighbour boids.
#endif
*/

int
checkParameters()
{
  if (DIMENSIONS != 2)
    return 10;
  if (NEIGHBOR_DISTANCE < ELASTIC_DISTANCE)
    return -10;
  if (ELASTIC_DISTANCE < CORE_RADIUS)
    return -11;
  if (V0*DT > CORE_RADIUS)
    return -19;
  
  else
    return 0;
}
