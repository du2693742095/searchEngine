SRCS=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIB:= -pthread
PATHS:= -I ../include
CC:= g++
BIN:= ../bin/server

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LIB) $(PATHS)

%.o:%.cc
	$(CC) -c $^ -o $@ $(LIB) $(PATHS)


clean:
	${RM} ${OBJS} ${BIN} 
rebuild:clean OUT
