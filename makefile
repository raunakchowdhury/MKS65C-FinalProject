main: setup_client setup_server

rserver:
	./server

rclient:
	./client $(args)

setup_server: server.o networking.o coupGame.o
	gcc -o server server.o networking.o coupGame.o

setup_client: client.o networking.o
	gcc -o client client.o networking.o

client.o: client.c networking.h
	gcc -c client.c

server.o: server.c networking.h coupGame.h server.h
	gcc -c server.c

coupGame.o: coupGame.c coupGame.h server.h
	gcc -c coupGame.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
