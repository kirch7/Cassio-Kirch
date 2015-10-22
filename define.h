#define X        0
#define Y        1
#define Z        2
#define PI       3.14159265359

//#define PLOT_EXIT_FILES
#define GAMMA_FILE

#define ENDOPROP               0.25
#define N                      2000
#define RANGE                  200.0
#define PROP                   0.35
#define SEED                   13049        /* Must be unsigned int. */
#define DIMENSIONS             2
#define BOXES                  22500
#define BOXES_IN_EDGE          150
#define NEIGHBOR_DISTANCE      1.32              /* r0 */
#define CORE_RADIUS            0.4          /* rc */
#define ELASTIC_DISTANCE       1.0               /* re */
#define INFINITE_FORCE         10000.0
#define ETA                    1.0
#define V0                     0.1
#define ALPHA11                0.01
#define ALPHA12                0.01
#define ALPHA22                0.01
#define BETA11                 3.83
#define BETA12                 2.53
#define BETA22                 2.50
#define DT                     1.0
#define DATE_SIZE              24           /* Size of string */
#define FILENAME_SIZE          128           /* Size of string */
#define STEPS                  10000000000
#define EXIT_INTERVAL          1000
#define NUM_THREADS            8

#define RANDOM_0(max) (((double)rand()/RAND_MAX) * max)

#define absDistance(x1, y1, x2, y2) sqrt(pow ((x2) - (x1), 2) +       \
                                         pow ((y2) - (y1), 2))
