main: client server

server: server.o networking.o game.o
	gcc -o server server.o networking.o game.o

client: client.o networking.o
	gcc -o client client.o networking.o

client.o: client.c networking.h
	gcc -c client.c

server.o: server.c networking.h beta.h server.h
	gcc -c server.c

game.o: game.c game.h
	gcc -c game.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
