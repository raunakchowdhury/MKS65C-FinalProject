#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <pwd.h>

struct players {char name[100]; int money; char i1[100]; char i2[100];};

int myrand() {
  int randD = open("/dev/random", O_RDONLY);
  if(randD < 0)
    return 0;
  else {
    int *number = calloc(1, sizeof(int));
    int result = read(randD, number, sizeof number);
    if (result < 0)
      return 0;
    return *number;
  }
}

int main() {
  printf("-----COUP-----\n");
  printf("enter \'9\' if you ever wish to exit\n\n");
  int *orders = calloc(1000, sizeof(int));
  int curPlayer = 0;
  //add choices
  int numPlayer = 3; // at most 5?
  int ocounter = 0;
  char input[100];
  char endGame = 1;
  int *deck = calloc(15, sizeof(int));
  int counter;
  int randLoc;
  char cards[5][100] = {"duke", "assassin", "ambassador",
			"captain", "contessa"};
  char deadCards[5][100] = {"DEAD duke", "DEAD assassin", "DEAD ambassador",
			    "DEAD captain", "DEAD contessa"};
  char turnActions[7][100] = {"0. tax", "1. steal", "2. assassinate",
			      "3. exchange", "4. income", "5. foreign-aid",
			      "6. coup"};

  //duke 0-2, assassin 3-5, ambassador 6-8, captain 9-11, contessa 12-14
  //deck legend
  // 0 unassigned (still in deck), 10-14 (player i1), 20-24 (i2),
  struct players people[numPlayer]; //calloc?
  for(counter = 0; counter < numPlayer; counter++) {
    struct players cur = people[counter];
    char n = 'a' + counter;
    char na[2] = {n, 0};
    strcpy(people[counter].name, na);
    people[counter].money = 8;
    
    randLoc = abs(myrand()) % 15;
    while(deck[randLoc] != 0)
      randLoc = abs(myrand()) % 15;
    //printf("%d\n", randLoc);
    deck[randLoc] = 10 + counter;
    strcpy(people[counter].i1, cards[randLoc % 3]);
    
    randLoc = abs(myrand()) % 15;
    while(deck[randLoc] != 0)
      randLoc = abs(myrand()) % 15;
    //printf("%d\n", randLoc);
    deck[randLoc] = 20 + counter;
    strcpy(people[counter].i2, cards[randLoc % 3]);
  }
  printf("player info:\n");
  for(counter = 0; counter < numPlayer; counter++) {
    printf("%d: player %s, $%d, %s, %s\n", counter, people[counter].name,
	   people[counter].money, people[counter].i1, people[counter].i2);
  }
  //printf("deck status:\n");
  //for(counter = 0; counter < 15; counter++) {
  //  printf("%d: %d\n", counter, deck[counter]);
  //}
  
  //endGame = 0;
  while(endGame) { //change to while(not all dead) for(each player)
    //need to change so that u skip dead players
    printf("\n\n\n\n\n");
    if(people[curPlayer].i1[0] == 'D' && people[curPlayer].i2[0] == 'D') {
      curPlayer++;
      if(curPlayer == numPlayer)
	curPlayer = 0;
    } else {
      printf("your turn!\n%d: player %s\n0. tax   1. steal   2. assassinate   3. exchange   4. income   5. foreign-aid   6. coup\nenter the action you wish to do:\n", curPlayer, people[curPlayer].name);
      fgets(input, sizeof(input), stdin);
      if(input[0] == '9')
	exit(0);
      //needed for printing chat hist?
      orders[ocounter] = curPlayer; //whole turn order thing neccessary?
      ocounter++;
      int ans = input[0] - 48; //lazy
      orders[ocounter] = ans;

      if(ans == 0) { //tax
	people[curPlayer].money += 3;
	if(people[curPlayer].money > 10)
	  people[curPlayer].money = 10;
      } else if(ans == 1) { //steal
	;
      } else if(ans == 2) { //assassinate
	;
      } else if(ans == 3) { // exchange
	;
      } else if(ans == 4) { // income
	people[curPlayer].money++;
	if(people[curPlayer].money > 10)
	  people[curPlayer].money = 10;
      } else if(ans == 5) { // foreign-aid
	people[curPlayer].money += 2;
	if(people[curPlayer].money > 10)
	  people[curPlayer].money = 10;
      } else if(ans == 6) { //coup *need to limit this action for only if >=$7
	if(people[curPlayer].money >= 7) {
	  people[curPlayer].money -= 7;
	  printf("coup:\n");
	  int response[numPlayer];
	  for(counter = 0; counter < numPlayer; counter++) {
	    if(counter != curPlayer)
	      printf("%d. player %s   ", counter, people[counter].name);
	  }
	  printf("\n");
	  fgets(input, sizeof(input), stdin);
	  if(input[0] == '9')
	    exit(0);
	  int tAns = input[0] - 48;
	  printf("you staged a coup...\nwaiting for player %s to reveal an influence...\n", people[tAns].name);

	  printf("\n\nplayer %s, player %s staged a coup on you\nyou must reveal an influence:\n", people[tAns].name, people[curPlayer].name);
	  int tempN = 0;
	  if(people[tAns].i1[0] != 'D') {
	    printf("%d. %s   ", tempN, people[tAns].i1);
	    tempN++;
	  }
	  if(people[tAns].i2[0] != 'D') {
	    printf("%d. %s   ", tempN, people[tAns].i2);
	    tempN += 2;
	  }
	  printf("\n");
	  fgets(input, sizeof(input), stdin);
	  if(input[0] == '9')
	    exit(0);
	  int dead = input[0] - 48;
	  char rep[100] = "DEAD ";
	  if(tempN == 3) {
	    if(dead % 2 == 0) {
	      strcat(rep, people[tAns].i1);
	      strcpy(people[tAns].i1, rep);
	    }
	    else {
	      strcat(rep, people[tAns].i2);
	      strcpy(people[tAns].i2, rep);
	    }
	  } else if(tempN == 2) {
	    strcat(rep, people[tAns].i2);
	    strcpy(people[tAns].i2, rep);  
	  } else if(tempN == 1) {
	    strcat(rep, people[tAns].i1);
	    strcpy(people[tAns].i1, rep);
	  }
	  else {
	    ;
	  }
	}
	else
	  ;
      } else {
	;
      }
      if(ans < 4) {
	for(counter = 1; counter < numPlayer ; counter++) {
	  int temp = counter + curPlayer;
	  printf("player %s: player %s attempted to %s\n", people[temp].name, people[curPlayer].name, turnActions[ans]);
	  printf("0. allow   1. challenge   2. ...\n");
	  fgets(input, sizeof(input), stdin);
	  if(input[0] == '9')
	    exit(0);
	  int tAns = input[0] - 48;
	  if(tAns == 0) {
	    ;
	  } else if(tAns == 1) {
	    printf("this one is gonna be annoying :(\n");
	  } else if(tAns == 2) {
	    ;
	  } else {
	    ;
	  }
	}
      }
    
    
      printf("\n\n\n");
      for(counter = 0; counter < ocounter; counter += 2)
	printf("action %d: player %s, %d\n", 1 + counter/2, people[orders[counter]].name, orders[counter+1]);
      printf("player info:\n");
      for(counter = 0; counter < numPlayer; counter++) {
	if(counter == curPlayer)
	  printf("%d: player %s, $%d, %s, %s\n", counter, people[counter].name,
		 people[counter].money, people[counter].i1, people[counter].i2);
	else {
	  printf("%d: player %s, $%d, ", counter, people[counter].name,
		 people[counter].money);
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
      int deadPlayers = 0;
      for(counter = 0; counter < numPlayer; counter++) {
	if(people[counter].i1[0] == 'D' && people[counter].i2[0] == 'D')
	  deadPlayers++;
      }
      if(deadPlayers + 1 == numPlayer) {
	printf("game done\n");
	endGame = 0;
      }


      ocounter++;
      curPlayer++;
      if(curPlayer == numPlayer)
	curPlayer = 0;

    }
  }
}
