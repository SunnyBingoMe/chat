include ./Make.defines

PROGS =	chatClient.a chatServer.a

all:	${PROGS}

chatClient.a:	chatClient.o clientThread.o
		${CC} ${CFLAGS} -o $@ $^

clientThread.a:	clientThread.o
		${CC} ${CFLAGS} -o $@ $^

chatServer.a:	chatServer.o serverThread.o
		${CC} ${CFLAGS} -o $@ $^

serverThread.a:	serverThread.o
		${CC} ${CFLAGS} -o $@ $^


clean:
		rm -f ${PROGS} ${CLEANFILES}

