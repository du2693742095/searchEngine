OBJS:=$(patsubst %.cc, %, $(wildcard *.cc))
LIB:= -llog4cpp -lpthread
CC:= g++

OUT: ${OBJS}

%:%.cc
	$(CC) $^ -o $@ $(LIB)


clean:
	${RM} ${OBJS}
rebuild:clean OUT
