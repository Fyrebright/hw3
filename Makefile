FORMATTER=~/.vscode/extensions/ms-vscode.cpptools-1.17.5-linux-x64/LLVM/bin/clang-format
STYLE="file:${HOME}/Documents/Class/6-F2023/CSC246/clang-format.txt"
TARGET=.
CC = gcc
CFLAGS = -Wall -g -std=c99
LD_FLAGS = -D_XOPEN_SOURCE=500 -lpthread -lm

shortest-threads: p2/shortest-threads.c
	${CC} ${CFLAGS} p3/shortest-threads.c -o p2/shortest-threads ${LD_FLAGS}
schedule: p3/common.h
	${CC} ${CFLAGS} p3/schedule.c -o p3/schedule ${LD_FLAGS}
	${CC} ${CFLAGS} p3/reset.c -o p3/reset ${LD_FLAGS}
format:
	${FORMATTER} p2/*.c p3/*.c p3/*.h --style=${STYLE} -i
clean:
	rm shortest-threads