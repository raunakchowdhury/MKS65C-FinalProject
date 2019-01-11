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
int ocounter = 0;
char input[charMax];
char endGame = 1;
//court: duke 0-2, captain 3-5, captain 6-8, ambassador 9-11, contessa 12-14
//0 unassigned (still in court), 10-14 (i1), 20-24 (i2),
int court[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int counter;
int randLoc;
char cards[5][charMax] = {"\x1B[35mDUKE\x1B[0m",
	   "\x1B[36mCAPTAIN\x1B[0m",
	   "\x1B[90mASSASSIN\x1B[0m",
	   "\x1B[33mAMBASSADOR\x1B[0m",
	   "\x1B[31mCONTESSA\x1B[0m"};
char turnActions[7][charMax] = {"tax", "steal", "assassinate",
		"exchange", "income", "foreign-aid",
		"coup"};
char accepted[10][charMax] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
struct player * people;

int myrand();
void getInput();
void setup();
int findLoc(int value);
void printInfo(int cur);
void printHistory(); //needs reworking
void check(int size);

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


