
# !/bin/bash
# test_schedule.sh

source "./assert.sh"

reg_file="p3/registration-1.txt"
RESET="./p3/reset"
SCHEDULE="./p3/schedule"

# Set up an initial registration board.
actual=$((${RESET} ${reg_file}) 2>&1)
expected=""
assert_eq "$expected" "$actual" "MSG"

# Get a look at the registration info on Monday.
actual=$((${SCHEDULE} query Monday) 2>&1)
expected="Monday u u u u u u u u 9 0 0 8 0 0 0 0 u u u u u u u u"
assert_eq "$expected" "$actual" "MSG"

# make a new registration
actual=$((${SCHEDULE} register Monday 9) 2>&1)
expected="success"
assert_eq "$expected" "$actual" "MSG"

# See the result of the registration made in the other termial.
actual=$((${SCHEDULE} query Monday) 2>&1)
expected="Monday u u u u u u u u 9 1 0 8 0 0 0 0 u u u u u u u u"
assert_eq "$expected" "$actual" "MSG"

# Make another registration and have a look at the result.
actual=$((${SCHEDULE} register Monday 11) 2>&1\)
expected="success"
assert_eq "$expected" "$actual" "MSG"

actual=$((${SCHEDULE} query Monday) 2>&1)
expected="Monday u u u u u u u u 9 1 0 9 0 0 0 0 u u u u u u u u"
assert_eq "$expected" "$actual" "MSG"

# Cancel a registration from the other terminal and see the result.
actual=$((${SCHEDULE} cancel Monday 8) 2>&1)
expected="success"
assert_eq "$expected" "$actual" "MSG"

actual=$((${SCHEDULE} query Monday) 2>&1)
expected="Monday u u u u u u u u 8 1 0 9 0 0 0 0 u u u u u u u u"
assert_eq "$expected" "$actual" "MSG"

# Try a few invalid commands.
actual=$((${SCHEDULE} cancel) 2>&1)
expected="error"
assert_eq "$expected" "$actual" "MSG"

actual=$((${SCHEDULE} register Monday 0) 2>&1)
expected="error"
assert_eq "$expected" "$actual" "MSG"

actual=$((${SCHEDULE} registar Monday 8) 2>&1)
expected="error"
assert_eq "$expected" "$actual" "MSG"

actual=$((${RESET}) 2>&1)
expected="usage: reset <registration-file>"
assert_eq "$expected" "$actual" "MSG"

actual=$((${RESET} bad-filename) 2>&1)
expected="Invalid input file: bad-filename"
assert_eq "$expected" "$actual" "MSG"