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
#include "beta.h"

int orders[1000];
int curPlayer = 0;
int numPlayer = 0;
int ocounter = 0;
char input[charMax];
char endGame = 1;
int court[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//court: duke 0-2, captain 3-5, captain 6-8, ambassador 9-11, contessa 12-14
//0 unassigned (still in court), 10-14 (i1), 20-24 (i2),
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
struct player * people;

//random number from 0-14
int myrand() {
  int randD = open("/dev/random", O_RDONLY);
  if(randD < 0)
    return 0;
  else {
    int *number = calloc(1, sizeof(int));
    int result = read(randD, number, sizeof number);
    if (result < 0)
      return 0;
    *number = abs(*number) % 15;
    return *number;
  }
}

//reads stdin
void getInput() {
  printf("%s- ", BLU);
  fgets(input, sizeof(input), stdin);
  printf("%s", NRM);
  input[strlen(input) - 1] = 0; //removes newLine
  if(strcmp(input, "exit") == 0)
    exit(0);
}

//populates people and court
void setup() {
  for(counter = 0; counter < numPlayer; counter++) {
    //populates name, wealth
    struct player cur = people[counter];
    printf("player %d, please enter your name:\n", counter);
    getInput();
    strcpy(people[counter].name, input);
    people[counter].wealth = 2;

    //populates court, i1, i2
    randLoc = myrand();
    while(court[randLoc] != 0)
      randLoc = myrand();
    court[randLoc] = 10 + counter; //i1
    strcpy(people[counter].i1, cards[randLoc / 3]);
    randLoc = myrand();;
    while(court[randLoc] != 0)
      randLoc = myrand();
    court[randLoc] = 20 + counter; //i2
    strcpy(people[counter].i2, cards[randLoc / 3]);
  }
}

void printInfo(int cur) {
  printf("-----player info-----\n");
  for(counter = 0; counter < numPlayer; counter++) {
    if(counter == cur)
    printf("%splayer %s: $%d, %s, %s%s\n", GRN, people[counter].name,
	   people[counter].wealth, people[counter].i1, people[counter].i2, NRM);
    else {
      printf("player %s: $%d, ", people[counter].name,
	     people[counter].wealth);
      if(people[counter].i1[0] == 'D')
	printf("%s, ", people[counter].i1);
      else
	printf("unknown, ");
      if(people[counter].i2[0] == 'D')
	printf("%s\n", people[counter].i2);
      else
	printf("unknown\n");
    }
  }
  printf("\n");
}

void printHistory() {
  //redisgn with char array... or file where every pub action is written
  printf("history:\n");
  for(counter = 0; counter < ocounter; counter += 2)
    printf("action %d: player %s,  %s\n", 1 + counter/2,
	   people[orders[counter]].name, turnActions[orders[counter+1]]);
}

int tax(int cur) {
  people[cur].wealth += 3;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
}

int steal(int cur) {
  //steal:
  //you attempted to steal from c
  //waiting for blocks/challenges
  ; //choose who to steal
  //do it

  //blocking
  //_ attempted to steal from you
  //allow, challenge, block with captain, block with ambassador

  //__ attempted to block with captain
  //allow, challenge
}

int assassinate(int cur) {
  //assassinate:

  //_  attempted to assassinate you
  //allow, block with contessa, challenge

  //You attempted to block with contessa
  //Waiting for challenges

  //_ attempted to block with contessa
  ; //choose who
}

int exchange(int cur) {
  
  printf("court status:\n");
  for(counter = 0; counter < 15; counter++) {
    printf("%d: %d\n", counter, court[counter]);
  }
  /* printf("you attempted to exchange\nWaiting for blocks/challenges"); */
  /* char options[4][charMax]; */
  /* //options[2] = ; find deck location */
  /* //options[3] = ; */
  /* if(people[cur].i1[0] == 'D') */
  /*   options[2] = ""; */
  /* if(people[cur].i2[0] == 'D') */
  /*   options[3] = ""; */
  
  /* randLoc = myrand(); */
  /* while(court[randLoc] != 0) */
  /*   randLoc = myrand(); */
  /* //court[randLoc] = 10 + counter; //i1 */
  /* //strcpy(people[counter].i1, cards[randLoc % 3]); */
  /* options[0] = randLoc; */

  /* randLoc = myrand(); */
  /* while(court[randLoc] != 0) */
  /*   randLoc = myrand(); */
  /* options[1] = randLoc; */
  

  /* printf("choose a role to keep:\n"); */

  /* printf("choose another role to keep:\n"); */
  /* ; */
}

int income(int cur) {
  if(people[cur].wealth == 10)
    ;
  people[cur].wealth++;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
  // if cant do... return 1 for fail and ask again?
}

int foreignAid(int cur) {
  if(people[cur].wealth == 10)
    ;
  people[cur].wealth += 2;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;

  //_ attempted to draw foreign-aid
  // allow, block with duke

  //you attempted to block with duke
  //waiting for challenges

  //b attempted to block with duke
  //allow, challenge
}

int coup(int cur) {
  if(people[cur].wealth >= 7) {
    people[cur].wealth -= 7;
    printf("coup:\n");
    for(counter = 0; counter < numPlayer; counter++) {
      if(counter < cur)
	printf("%d. player %s   ", counter, people[counter].name);
      else if(counter == cur)
	;
      else
	printf("%d. player %s   ", counter - 1, people[counter].name);
    }
    printf("\n");
    getInput();
    int tAns = atoi(input);
    printf("you staged a coup...\nwaiting for player %s to reveal an influence...\n", people[tAns].name);
    if(tAns >= cur)
      tAns++;
    //reveal(int cur)  tAns
    printf("\n\nplayer %s, player %s staged a coup on you\nyou must reveal an influence:\n", people[tAns].name, people[cur].name);
    reveal(tAns);
  }
  else
    ; //for cannot coup too little wealth
}


void reveal(int cur) {
  int tempN = 0;
  //printing choices avaiable
  if(people[cur].i1[0] != 'D') {
    printf("%d. %s   ", tempN, people[cur].i1);
    tempN += 1;
  }
  if(people[cur].i2[0] != 'D') {
    printf("%d. %s   ", tempN, people[cur].i2);
    tempN += 2;
  }
  printf("\n");
  getInput();
  int choice = atoi(input);
  char dead[charMax] = "DEAD ";
  if(tempN == 3) { //when both influences are still alive
    if(choice == 0) {
      strcat(dead, people[cur].i1);
      strcpy(people[cur].i1, dead);
    }
    else {
      strcat(dead, people[cur].i2);
      strcpy(people[cur].i2, dead);
    }
  } else if(tempN == 1) { //if i2 is DEAD
    strcat(dead, people[cur].i1);
    strcpy(people[cur].i1, dead);  
  } else { //if i1 is DEAD
    strcat(dead, people[cur].i2);
    strcpy(people[cur].i2, dead);
  }
}

// 0 for challenger failed, 1 for challenger succeeded
int challenge(int challenger, int challengee, int card) {
  if(strcmp(people[challengee].i1, cards[card]) ||
     strcmp(people[challengee].i2, cards[card])) { //failed challege
    printf("\nyou failed to challenge player %s\nyou must reveal an influence: \n", people[challenger].name);
    reveal(challenger);
    return 0;
  } else { //successful challenge
    printf("\nplayer %s, player %s successfully challenged you\nyou must reveal an influence:\n", people[challengee].name, people[challenger].name);
    reveal(challengee);
    return 1;
  }
}


void turn() {
  printf("your turn!\n%d: player %s\n0. tax   1. steal   2. assassinate   3. exchange   4. income   5. foreign-aid   6. coup\nenter the action you wish to do:\n", curPlayer, people[curPlayer].name);
  getInput();
  //needed for printing chat hist?
  orders[ocounter] = curPlayer;
  ocounter++;
  int ans = atoi(input);
  orders[ocounter] = ans;
  //switches???
  if(ans == 0) { //tax
    tax(curPlayer);
  } else if(ans == 1) { //steal
    steal(curPlayer);
  } else if(ans == 2) { //assassinate
    assassinate(curPlayer);
  } else if(ans == 3) { // exchange
    exchange(curPlayer);
  } else if(ans == 4) { // income
    income(curPlayer);
  } else if(ans == 5) { // foreign-aid
    foreignAid(curPlayer);
  } else if(ans == 6) { //coup *need to limit this action for only if >=$7
    coup(curPlayer);
  } else {
    ;
  }
  //instead of ans < 4, more specific
  if(ans < 4) {
    for(counter = 1; counter < numPlayer ; counter++) {
      int temp = (counter + curPlayer) % numPlayer;
      printf("\nplayer %s: player %s attempted to %s\n", people[temp].name, people[curPlayer].name, turnActions[ans]);
      printf("0. allow   1. challenge   2. ...\n");
      getInput();
      int tAns = atoi(input);
      if(tAns == 0) {
	;
      } else if(tAns == 1) { //challenge
	int result;
	result = challenge(temp, curPlayer, ans);
	//need to break for loop;
	//need to undo action of challenged
      } else if(tAns == 2) {
	;
      } else {
	;
      }
    }
  }
  if(ans == 5)
    ;
}

void gameEnd() {
  int deadPlayers = 0;
  int winner;
  for(counter = 0; counter < numPlayer; counter++) {
    if(people[counter].i1[0] == 'D' && people[counter].i2[0] == 'D')
      deadPlayers++;
    else
      winner = counter;
  }
  if(deadPlayers + 1 == numPlayer) { //only one man standing
    printf("player %s won!!!\n", people[winner].name);
    printf("-----GAME END-----\n");
    endGame = 0;
  }
}

//limit inputs to only acceptable answers
int main() {
  printf("-----COUP-----\n");
  printf("enter \'exit\' if you ever wish to close the game\n\n\n");
  printf("enter the number of players: (max 5)\n");
  getInput();
  numPlayer = atoi(input); //only let 2-5
  people = calloc(numPlayer, sizeof(struct player));
  setup();
  while(endGame) {
    //play action
    //challenge
    //block action
    //undo action
    printf("\n\n\n");
    printInfo(curPlayer);
    if(people[curPlayer].i1[0] == 'D' && people[curPlayer].i2[0] == 'D') {
      curPlayer++;
      curPlayer = curPlayer % numPlayer;
    } else {
      turn();
      printf("\n\n\n");
      printHistory();
      ocounter++;
      curPlayer++;
      curPlayer = curPlayer % numPlayer;
    }
    printf("\n\n\n");
    gameEnd();
  }
}
