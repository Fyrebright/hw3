#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <sys/types.h>
#include <limits.h>
#include <float.h>
#include <pthread.h>

typedef struct {
  int workers;
  int index;
  float minValue;
  bool report;
} WorkerInfo;

// Maximum width of each basis vector.
#define DMAX 100

// Type that can be used to represent a basis vector.
typedef int BVec[DMAX];

// Actual length of each vector in the basis.
int n;

// List of all vectors defining the lattice.
BVec *basis;

// Number of vectors representing the basis.
int bCount = 0;

// Current capacity of the basis array
int bCap = 0;

// Print out an error message and exit.
static void fail(char const *message)
{
  fprintf(stderr, "%s\n", message);
  exit(1);
}

// Print out a usage message, then exit.
static void usage()
{
  printf("usage: short <workers>\n");
  printf("       short <workers> report\n");
  exit(1);
}

// Read the input list of basis vectors.
void readBasis()
{
  // Determine the size of the basis
  if (scanf("%d", &n) != 1)
    fail("Can't read vector length");

  // Set up initial list and capacity.
  bCap = 5;
  basis = (BVec *)malloc(bCap * sizeof(BVec));

  // Read all the basis vectors from the input.
  int v;
  while (scanf("%d", &v) == 1) {
    // Grow the basis array if needed.
    if (bCount >= bCap) {
      bCap *= 2;
      basis = (BVec *)realloc(basis, bCap * sizeof(BVec));
    }

    // Fill in the next basis vector.
    basis[bCount][0] = v;
    for (int i = 1; i < n; i++)
      if (scanf("%d", &(basis[bCount][i])) != 1)
        fail("Can't read vector element");
    bCount += 1;
  }
}

float vecSumMag(int x, int y, int z)
{
  float sum = 0;

  // Sum all elements to the power of two
  for (int i = 0; i < n; i++) {
    sum += powf(basis[x][i] + basis[y][i] + basis[z][i], 2.F);
  }

  return sqrt(sum);
}

void *vecWorker(WorkerInfo *info)
{
  // Use index value to iterate over this worker's assigned set of starting basis
  // vectors and sum all combinations of vectors with higher index
  for (int i = info->index; i < bCount - 2; i += info->workers) {
    for (int j = i + 1; j < bCount - 1; j++) {
      for (int k = j + 1; k < bCount; k++) {
        float mag = vecSumMag(i, j, k);

        // Print report value if specified
        if (info->report) {
          printf("%d : | B[%d] + B[%d] + B[%d] | = %.3f\n", info->index, i, j,
                 k, mag);
        }

        // Replace minimum value if necessary
        info->minValue = fmin(info->minValue, mag);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  bool report = false;
  int workers = 4;

  // Parse command-line arguments.
  if (argc < 2 || argc > 3)
    usage();

  if (sscanf(argv[1], "%d", &workers) != 1 || workers < 1)
    usage();

  // If there's a second argument, it better be the word, report
  if (argc == 3) {
    if (strcmp(argv[2], "report") != 0)
      usage();
    report = true;
  }

  readBasis();

  // Create a list to hold the pthread type for each worker
  pthread_t threads[workers];
  // And another for their corresponding info structs
  WorkerInfo infos[workers];

  for (int i = 0; i < workers; i++) {
    infos[i].index = i;
    infos[i].minValue = FLT_MAX;
    infos[i].workers = workers;
    infos[i].report = report;

    // Create new thread from info; if return != 0 creation failed
    if (pthread_create(&threads[i], NULL, vecWorker, NULL))
      fail("Thread creation failed");
  }

  // Wait on children and track lowest magnitude value
  float minMag = FLT_MAX;
  for (int i = 0; i < workers; i++) {
    // Wait for thread to exit; return value not needed
    pthread_join(threads[i], NULL);

    // Update minimum value if needed
    minMag = fmin(minMag, infos[i].minValue);
  }

  printf("Shortest Vector: %.3f\n", minMag);

  // Release resources and exit
  free(basis);
  return EXIT_SUCCESS;
}
