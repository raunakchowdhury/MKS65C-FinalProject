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

#define charMax 100

#define NRM  "\x1B[0m"
#define GRN  "\x1B[32m"
#define BLU  "\x1B[34m"

#define RED  "\x1B[31m" //contessa
#define YEL  "\x1B[33m" //ambassador
#define MAG  "\x1B[35m" //duke
#define CYN  "\x1B[36m" //captain
#define WHT  "\x1B[90m" //assassin
struct player {
  char name[charMax];
  int wealth;
  char i1[charMax];
  char i2[charMax];
};

int orders[1000];
int curPlayer;
int numPlayer;
int ocounter;
char input[charMax];
char endGame;
int court[15];
int counter;
int randLoc;
char cards[5][charMax];
char deadCards[5][charMax];
char turnActions[7][charMax];

struct player * people;

int myrand();
void getInput();
void setup();
void printInfo(int cur);
void printHistory(); //needs reworking

int tax(int cur);
int steal(int cur);
int assassinate(int cur);
int exchange(int cur);
int income(int cur);
int foreignAid(int cur);
int coup(int cur);

void reveal(int cur);
int challenge(int challenger, int challengee, int card);
void turn();
void gameEnd();

//challenging


struct subserver {
  int from_child;
  int to_child;
};

// subserver stuff
void play_game(int client_socket, int from_parent, int to_parent);

// server stuff
void master_game(struct subserver subservers[]);


