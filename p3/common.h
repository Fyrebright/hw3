// Maximum number of rows.
#define ROW_MAX 7

// Maximum length of the day name.
#define DAYNAME_MAX 10

// Length (number of blocks) in a day's schedule
#define COL_MAX 23

// Define values for seeding `ftok()`
#define ABCOSTE2_HOME "/mnt/ncsudrive/a/abcoste2"
#define PROJ_ID 5

// Representation for the schedule, defined here so both programs
// can use it.
typedef struct {
  // List of names for each day
  char dayNames[ROW_MAX][DAYNAME_MAX];
  // List of schedules for each day
  char daySchedules[ROW_MAX][COL_MAX];
} RegState;
