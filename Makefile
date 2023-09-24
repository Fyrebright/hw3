FORMATTER=~/.vscode/extensions/ms-vscode.cpptools-1.17.5-linux-x64/LLVM/bin/clang-format
STYLE="file:${HOME}/Documents/Class/6-F2023/CSC246/clang-format.txt"
TARGET=.
CC = gcc
CFLAGS = -Wall -g -std=c99

shortest-threads: p2/shortest-threads.c
	${CC} ${CFLAGS} p2/shortest-threads.c -o p2/shortest-threads -D_XOPEN_SOURCE=500 -lpthread -lm
format:
	${FORMATTER} p2/*.c p3/*.c p3/*.h --style=${STYLE} -i
clean:
	rm shortest-threads