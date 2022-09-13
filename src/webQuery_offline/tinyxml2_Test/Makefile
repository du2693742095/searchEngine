SRCS=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIB:=
CC:= g++
BIN:=main

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LIB)

%.o:%.cc
	$(CC) -c $^ -o $@ $(LIB)


clean:
	${RM} ${OBJS} ${BIN} 
rebuild:clean OUT
