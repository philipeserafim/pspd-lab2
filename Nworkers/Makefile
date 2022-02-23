client:
	gcc tcpClient.c -o bin/client -I./inc -lm

server:
	gcc tcpServer.c -o bin/server -I./inc
	
clean:
	rm -rf bin/

all:
	make clean
	mkdir -p bin/
	make client
	make server

runS:
	bin/server 127.0.0.1 8000

runC:
	bin/client 127.0.0.1 8000
