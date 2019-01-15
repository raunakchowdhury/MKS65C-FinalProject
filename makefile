main: client server

server: server.o networking.o beta.o
	gcc -o server server.o networking.o beta.o

client: client.o networking.o
	gcc -o client client.o networking.o

client.o: client.c networking.h
	gcc -c client.c

server.o: server.c networking.h beta.h server.h
	gcc -c server.c

beta.o: beta.c beta.h server.h
	gcc -c beta.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
