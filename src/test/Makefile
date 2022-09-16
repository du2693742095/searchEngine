SRCS=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIB:= 
PATHS:= -I /home/lemon/Git/searchEngine/include/
CC:= g++
BIN:= a.out

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LIB) $(PATHS)

%.o:%.cc
	$(CC) -c $^ -o $@ $(LIB) $(PATHS)


clean:
	${RM} ${OBJS} ${BIN} 
rebuild:clean OUT
