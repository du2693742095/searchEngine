SRCS=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
LIB:=
CC:= g++
BIN:=main

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LIB)

%.o:%.cpp
	$(CC) -c $^ -o $@ $(LIB)


clean:
	${RM} ${OBJS} ${BIN} 
rebuild:clean OUT
