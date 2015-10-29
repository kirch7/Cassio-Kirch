#ifndef DERM_TYPE_ENUM
typedef enum {ENDODERM, ECTODERM} cellType;
#define DERM_TYPE_ENUM
#endif


#ifndef BOID_STRUCT
struct Boid
{
  struct Boid* next;         /* Considering on the same box... */
  struct Boid* previous;     /* ... (same list).               */
  unsigned int boxID;
  unsigned int neighbors;
  unsigned int ectoNeighbors;
  cellType type;             /* Endo or ectoderm. */
  double   gamma;
  double   position[2];
  double   velocity[2];
  double   newVelocity[2];
};
#define BOID_STRUCT
#endif

unsigned int getBoxID (struct Boid boid);

void initializeBoid (struct Boid* const boid);

