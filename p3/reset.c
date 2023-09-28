#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include "common.h"

// Print out an error message and exit.
static void fail(char const *message)
{
  fprintf(stderr, "%s\n", message);
  exit(1);
}

// Print out a usage message and exit.
static void usage()
{
  fprintf(stderr, "usage: reset <registration-file>\n");
  exit(1);
}

void loadRegistration(const char *filename, RegState *reg)
{
  FILE *regFile = fopen(filename, "r");

  // Iterate over ROW_MAX days in file
  for (int i = 0; i < ROW_MAX; i++) {
    // Remove day and whitespace from beginning of line and put into RegState
    fscanf(regFile, " %10s ", reg->dayNames[i]);

    // Iterate over COL_MAX hours
    for (int j = 0; j <= COL_MAX; j++) {
      // Assign next character to corresponding day,hour index, consuming whitespace
      fscanf(regFile, "%c ", &reg->daySchedules[i][j]);
    }
  }
}

int main(int argc, char *argv[])
{
  // Check file argument was supplied
  if (argc != 2)
    usage();

  // Load registration from specified file into struct
  RegState reg;
  loadRegistration(argv[1], &reg);

  // Generate token
  key_t shmToken = ftok(ABCOSTE2_HOME, PROJ_ID);

  // Allocate shared memory region for RegState
  int shmId = shmget(shmToken, sizeof(RegState), IPC_CREAT);
  if (shmId == -1)
    fail("Error allocating shared memory for RegState");

  // Attach shared memory and assign pointer to it
  void *smBuffer = shmat(shmId, NULL, 0);

  // Read registration struct into shared memory
  memcpy(smBuffer, &reg, sizeof(RegState));

  // Release the reference to the shared memory
  shmdt(smBuffer);

  return EXIT_SUCCESS;
}
