SRCS=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIB:= 
PATHS:= 
CC:= g++
BIN:= /home/lemon/Git/searchEngine/bin/clinet

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LIB) $(PATHS)

%.o:%.cc
	$(CC) -c $^ -o $@ $(LIB) $(PATHS)


clean:
	${RM} ${OBJS} ${BIN} 
rebuild:clean OUT
