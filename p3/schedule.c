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
static void fail(char const *message, const bool printErrno)
{
  fprintf(stderr, "%s\n", message);

  // Optionally print errno
  if(printErrno)
    fprintf(stderr, "errno: %d\n", errno);
  
  exit(EXIT_FAILURE);
}

char parseDay(char *str, RegState *reg)
{
  // Loop through names in registration until match is found
  for (int i = 0; i < ROW_MAX; i++) {
    if (strcmp(str, reg->dayNames[i]) == 0)
      return i;
  }

  // No matching name
  printf("error\n");
  exit(EXIT_FAILURE);
}

void registerEvent(const int day, const int hour, RegState *reg)
{
  // Return error if no webinar or it is full
  if (reg->daySchedules[day][hour] == 'u' ||
      reg->daySchedules[day][hour] >= '9') {
    printf("error\n");
  } else {
    // Otherwise, increment and report sucess
    reg->daySchedules[day][hour]++;
    printf("success\n");
  }
}

void cancelEvent(const int day, const int hour, RegState *reg)
{
  // Report error if no webinar or no participants
  if (reg->daySchedules[day][hour] == 'u' ||
      reg->daySchedules[day][hour] <= '0') {
    printf("error\n");
  } else {
    // Otherwise, decrement and report sucess
    reg->daySchedules[day][hour]--;
    printf("success\n");
  }
}

void queryDay(const int day, const RegState *reg)
{
  printf("%s", reg->dayNames[day]);

  // Print out the values for the hours
  for (int i = 0; i <= COL_MAX; i++) {
    printf(" %c", reg->daySchedules[day][i]);
  }
  // Add newline
  printf("\n");
}

int main(int argc, char *argv[])
{
  // Check at least command and day arguments supplied
  if (argc < 3) {
    printf("error\n");
    exit(EXIT_FAILURE);
  }

  // Get key to shared memory
  key_t shmToken = ftok(ABCOSTE2_HOME, PROJ_ID);
  int shmId = shmget(shmToken, sizeof(RegState), 0);
  if (shmId == -1)
    fail("Error getting token of shared memory", true);

  // Attach shared memory and cast as a pointer to a RegState struct
  RegState *regBuffer = (RegState *)shmat(shmId, NULL, 0);

  // Get index of day in registration
  int dayVal = parseDay(argv[2], regBuffer);

  // Handle query command first since it has less arguments
  if (strcmp(argv[1], "query") == 0) {
    // add command byte and send it with the day value

    queryDay(dayVal, regBuffer);
    shmdt(regBuffer);
    exit(EXIT_SUCCESS);
  }

  // Exit if wrong number of arguments
  if (argc != 4) {
    printf("error\n");
    shmdt(regBuffer);
    exit(EXIT_SUCCESS);
  }

  // Parse integer from argument checking that entire argument is consumed, and that
  // hour value is within range
  int hourVal, consumed;
  if (!sscanf(argv[3], "%d%n", &hourVal, &consumed) ||
      strlen(argv[3]) != consumed || hourVal < 0 || hourVal > 23) {
    printf("error\n");
    shmdt(regBuffer);
    exit(EXIT_FAILURE);
  }

  // Execute appropriate command
  if (strcmp(argv[1], "register") == 0) {
    registerEvent(dayVal, hourVal, regBuffer);
  } else if (strcmp(argv[1], "cancel") == 0) {
    cancelEvent(dayVal, hourVal, regBuffer);
  } else {
    printf("error\n");
  }

  shmdt(regBuffer);
  return EXIT_SUCCESS;
}
