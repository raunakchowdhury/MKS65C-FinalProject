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

// 3 of each class
// stack
int deck[15];

//player struct
struct player {
  int wealth;
  int first_char;
  int second_char;
};

void play_game(int client_socket);
// int[] shuffle();

// void play_game();
