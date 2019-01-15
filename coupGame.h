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

#define charMax 4096

#define NRM  "\x1B[0m"
#define GRN  "\x1B[32m"
#define BLU  "\x1B[34m"

struct player {
  char name[charMax];
  int wealth;
  char i1[charMax];
  char i2[charMax];
  int revealed;
};

int curPlayer;
int numPlayer;
char line[charMax];
char input[charMax];
char endGame;
int court[15];
int counter;
int randLoc;
char cards[5][charMax];
char turnActions[7][charMax];
char actions[7][charMax];
char accepted[10][charMax];
struct player * people;

void getInput(int cur);
void print(int client);
void printInfo(int cur);

int myrand();
int findLoc(int value);
void check(int cur, int size);

void tax(int cur);
void steal(int cur);
void assassinate(int cur);
void exchange(int cur);
void income(int cur);
void foreignAid(int cur);
void coup(int cur);

void reveal(int cur);
int challenge(int challenger, int challengee, int card);
int block(int cur, int def, int ans);
int chooseAction();
void actionEffect(int ans);
void turn();
void gameEnd();

void preSetup();
void playersJoin();
void addPlayers(int num, char names[5][256]);
void setup();
void runGame();
//challenging
