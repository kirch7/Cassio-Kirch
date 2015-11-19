#include <stdio.h>
#include <time.h>
//#include <sys/stat.h>

#include "define.h"
#include "files.h"

char dateString[DATE_SIZE];

char*
getDate()
{
  return dateString;
}

void
setDate ()
{
  time_t now = time(NULL);
  struct tm* tmP = localtime(&now);
  strftime(dateString, DATE_SIZE, "%Y%m%d_%Hh%Mmin%Ssec", tmP);
}

void
putParametersToFile(FILE* file)
{
  fprintf(file, "#Modo 'um sistema' com alpha e beta fixos.\n"          \
          "#Compiled at %s %s\n"                                        \
          "#Runned at\t%s\n"                                            \
          "#ALPHA11 = %lf\n"                                            \
          "#ALPHA12 = %lf\n"                                            \
          "#ALPHA22 = %lf\n"                                            \
          "#BETA11  = %lf\n"                                            \
          "#BETA12  = %lf\n"                                            \
          "#BETA22  = %lf\n"                                            \
          "#N   =     %d\n"                                             \
          "#ENDOPROP = %lf\n"                                           \
          "#ETA =     %lf\n"                                            \
          "#V0  =     %lf\n"                                            \
          "#R0  =     %lf\n"                                            \
          "#DT  =     %lf\n"                                            \
          "#CORE_RADIUS =      %lf\n"                                   \
          "#ELASTIC_DISTANCE = %lf\n"                                   \
          "#INFINITE FORCE   = %lf\n"                                   \
          "#RANGE =  %lf\n"                                             \
          "#PROP  =  %lf\n"                                             \
          "#SEED  =  %u\n"                                              \
          "#BOXES =  %u\n"                                              \
          "#BOXES_IN_EDGE =    %u\n"                                    \
          "#STEPS =  %llu\n"                                            \
          "#EXIT_INTERVAL = %llu\n#\n"                                  \
          "#THREADS: %u\n",                                             \
          __DATE__, __TIME__, dateString,                               \
          ALPHA11, ALPHA12, ALPHA22,                                    \
          BETA11, BETA12, BETA22,                                       \
          N, ENDOPROP, ETA, V0,                                         \
          NEIGHBOR_DISTANCE, DT,                                        \
          CORE_RADIUS, ELASTIC_DISTANCE,                                \
          INFINITE_FORCE, RANGE,                                        \
          PROP, SEED, BOXES,                                            \
          BOXES_IN_EDGE, STEPS,                                         \
          EXIT_INTERVAL, NUM_THREADS);
}

FILE*
initializeGodFile ()
{
  FILE* dat;
  char fileName[FILENAME_SIZE];
  sprintf(fileName, "one_system_%s.dat", dateString);
  dat = fopen (fileName, "w");
  putParametersToFile(dat);
  return dat;
}

FILE*
initializeGammaFile()
{
  FILE* gammaFile;
  char fileName[FILENAME_SIZE];
  sprintf(fileName, "gamma_%s.dat", dateString);
  gammaFile = fopen(fileName, "w");
  putParametersToFile(gammaFile);
  return gammaFile;
}

#ifdef COUNT_NEIGHBORS
FILE*
initializeAverageNeighborsFile ()
{
  FILE* myFile;
  char fileName[FILENAME_SIZE];
  sprintf(fileName, "avgNeighbors_%s.dat", dateString);
  myFile = fopen(fileName, "w");
  putParametersToFile(myFile);
  return myFile;
}
#endif

FILE*
initializeStepAndTypeFile (const unsigned long long int step,   \
                           const cellType type)
{
  static FILE* dat;
  static char fileName[FILENAME_SIZE];
  sprintf(fileName, "%llu_%u.dat", step, type);
  dat = fopen(fileName, "w");
  return dat;
}

FILE* initializeFinalConfigurationFile()
{
  FILE* finalConfigurationFile;
  char fileName[FILENAME_SIZE];
  sprintf(fileName, "final_%s.dat", dateString);
  finalConfigurationFile = fopen(fileName, "w");
  putParametersToFile(finalConfigurationFile);
  return finalConfigurationFile;
}

void
printParameters()
{
  putParametersToFile(stdout);
}
