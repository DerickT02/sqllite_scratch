SRC_FILES = main.c
CC = gcc

all:
	${CC} ${SRC_FILES}  -o sqls
	./sqls db
	
