#define X        0
#define Y        1
#define Z        2
#define PI       3.14159265359

//#define PLOT_EXIT_FILES
#define ENDO_GAMMA
//#define ECTO_GAMMA
//#define COUNT_NEIGHBORS

#define ENDOPROP               0.25
#define N                      800u
#define RANGE                  150.0
#define PROP                   0.35
#define SEED                   13049u        /* Must be unsigned int. */
#define DIMENSIONS             2u
#define BOXES                  22500u
#define BOXES_IN_EDGE          150u
#define NEIGHBOR_DISTANCE      1.0//1.32             /* r0 */
#define CORE_RADIUS            0.4               /* rc */
#define ELASTIC_DISTANCE       1.0               /* re */
#define INFINITE_FORCE         10000.0
#define ETA                    1.0
#define V0                     0.01
#define ALPHA11                0.1
#define ALPHA12                0.1
#define ALPHA22                0.1
#define BETA11                 0.3
#define BETA12                 0.16
#define BETA22                 0.15
#define DT                     1.0
#define DATE_SIZE              36u           /* Size of string */
#define FILENAME_SIZE          128u           /* Size of string */
#define STEPS                  5000000ull
#define EXIT_INTERVAL          1000ull
#define NUM_THREADS            8u

#define RANDOM_0(max) (((double)rand()/RAND_MAX) * max)

#define absDistance(x1, y1, x2, y2) sqrt(pow ((x2) - (x1), 2) +       \
                                         pow ((y2) - (y1), 2))
