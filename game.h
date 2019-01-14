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
#define MAX 5 //adjust this val for num of players
#define charMax 1024
// 3 of each class
// stack
int deck[15];

struct client {
  int client_socket;
  char name[charMax];
};

void announce(struct client clients[], int num_players, char * msg);

// server stuff
void master_game(struct client clients[]);
