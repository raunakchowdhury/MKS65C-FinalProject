forking: client fserver

fserver: forking_server.o networking.o game.o
	gcc -o server forking_server.o networking.o game.o

client: client.o networking.o
	gcc -o client client.o networking.o

client.o: client.c networking.h
	gcc -c client.c

forking_server.o: forking_server.c networking.h
	gcc -c forking_server.c

game.o: game.c game.h
	gcc -c game.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
