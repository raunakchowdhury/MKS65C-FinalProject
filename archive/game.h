# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/shm.h>
# include <sys/ipc.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/sem.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>


#define BUFFER_SIZE 256
#define MAX 2 //adjust this val for num of players

// 3 of each class
// stack
int deck[15];

//player struct
struct player {
  int wealth;
  int first_char;
  int second_char;
};

struct subserver {
  int from_child;
  int to_child;
};

// subserver stuff
void play_game(int client_socket, int from_parent, int to_parent);

// server stuff
void master_game(struct subserver subservers[]);

// int[] shuffle();

// void play_game();
