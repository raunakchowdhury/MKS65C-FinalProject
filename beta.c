#include "beta.h"

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

//getInput(int cur)

//printTo(int cur)
// -printf replacement, printInfo replacement
void getInput() {
  printf("%s- ", BLU);
  fgets(input, sizeof(input), stdin);
  printf("%s", NRM);
  input[strlen(input) - 1] = 0; //removes newLine
  if(strcmp(input, "exit") == 0)
    exit(0);
}

int findLoc(int value) {
  for(counter = 0; counter < 15; counter++) {
    if(court[counter] == value)
      return counter;
  }
  return 99;
}

//sendOutput(char b[charMax])

void printInfo(int cur) {
  printf("-----player info-----\n");
  for(counter = 0; counter < numPlayer; counter++) {
    if(counter == cur)
    printf("%splayer %s: $%d%s, %s, %s\n", GRN, people[counter].name,
	   people[counter].wealth, NRM, people[counter].i1, people[counter].i2);
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

void check( int size) {
  int correct = 1;
  for(counter = 0; counter < size; counter++) {
    if (strcmp(input, accepted[counter]) == 0)
      correct = 0;
  }
  while(correct == 1) {
    printf("your input does not match with the options provided\nre-enter\n");
    getInput();
    for(counter = 0; counter < size; counter++) {
      if (strcmp(input, accepted[counter]) == 0)
	correct = 0;
    }
  }
}

//populates people and court
void setup() {
  for(counter = 0; counter < numPlayer; counter++) {
    //populates name, wealth
    struct player cur = people[counter];
    printf("player %d, please enter your name:\n", counter);
    getInput();
    strcpy(people[counter].name, BLU);
    strcat(people[counter].name, input);
    strcat(people[counter].name, NRM);
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

int tax(int cur) {
  printf("you attempted to tax\nwaiting for challenges\n");
  people[cur].wealth += 3;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
  block(cur, 9, 0);
}

int steal(int cur) {
  printf("steal:\n");
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
  check(numPlayer - 1);
  int tAns = atoi(input);
  printf("you attempted to steal from player %s...\nwaiting for blocks/challenges...\n", people[tAns].name);
  if(tAns >= cur)
    tAns++;
  printf("\n\nplayer %s, player %s attempted to steal from you\nyou must reveal an influence:\n", people[tAns].name, people[cur].name);
  people[cur].wealth += 2;
  people[tAns].wealth -= 2;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
  if(people[tAns].wealth < 0)
    people[tAns].wealth = 0;
  
  block(cur, tAns, 1);
}

int assassinate(int cur) {
  if(people[cur].wealth >= 3) {
    people[cur].wealth -= 3;
    printf("assassinate:\n");
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
    check(numPlayer - 1);
    int tAns = atoi(input);
    if(tAns >= cur)
      tAns++;
    printf("you attempted to assassinate player %s...\nwaiting for player %s to reveal an influence...\n", people[tAns].name, people[tAns].name);
    block(cur, tAns, 2);
  }
  else
    ; //for cannot coup too little wealth
}

int exchange(int cur) {
  int options[4];
  int tempN = 0;
  int size = 4;
  int replace = 0;
  options[2] = findLoc(cur + 10);
  if(people[cur].i1[0] == 'D') {
    tempN += 1;
    replace = 3;
  }
  options[3] = findLoc(cur + 20);
  if(people[cur].i2[0] == 'D') {
    tempN += 2;
    replace = 2;
  }
  
  randLoc = myrand();
  while(court[randLoc] != 0)
    randLoc = myrand();
  options[0] = randLoc;

  randLoc = myrand();
  while(court[randLoc] != 0 || randLoc == options[0])
    randLoc = myrand();
  options[1] = randLoc;
  

  printf("choose a role to keep:\n");
  printf("0. %s   1. %s   ", cards[options[0] / 3], cards[options[1] / 3]);
  switch(tempN) {
  case 1:
    printf("2. %s\n", cards[options[3] / 3]);
    size = 3;
    break;
  case 2:
    printf("2. %s\n", cards[options[2] / 3]);
    size = 3;
    break;
  default:
    printf("2. %s   3. %s\n", cards[options[2] / 3], cards[options[3] / 3]);
  }
  getInput();
  check(size);
  int ans = atoi(input);
  if(size == 3) {
    if(ans == 0 || ans == 1) {
      court[options[ans]] = court[options[replace]];
      court[options[replace]] = 0;
      if(replace == 2) {
	strcpy(people[cur].i1, cards[options[ans] / 3]);
      }
      if(replace == 3)
	strcpy(people[cur].i2, cards[options[ans] / 3]);
    }
    printInfo(cur);
  }
  if(size == 4) {
    if(ans == 0 || ans == 1) { //uh
      strcpy(people[cur].i1, cards[options[ans] / 3]);
      court[options[ans]] = 10 + cur;
      court[options[2]] = 0;
    }
    if(ans == 3) {
      char temp[charMax];
      strcpy(temp, people[cur].i1);
      strcpy(people[cur].i1, people[cur].i2);
      strcpy(people[cur].i2, temp);
      //swap
      court[options[3]] = 10 + cur;
      court[options[2]] = 20 + cur;
    }
    printInfo(cur);
    int t = options[ans];
    options[ans] = options[3];
    options[3] = t;
    //move done one to far right
    printf("choose another role to keep:\n");
    printf("0. %s   1. %s   2. %s\n", cards[options[0] / 3], cards[options[1] / 3], cards[options[2] / 3]);
    getInput();
    check(3);
    int tAns = atoi(input);
    if(strcmp(people[cur].i2, cards[options[tAns] / 3]) == 0) {
      printf("nothing changed\n"); 
    } else {
      strcpy(people[cur].i2, cards[options[tAns] / 3]);
      court[findLoc(20 + cur)] = 0;
      court[options[tAns]] = 20 + cur;
    }
  }
  block(cur, 9, 3);
}

int income(int cur) {
  if(people[cur].wealth == 10) {
    printf("you can no longer gain wealth, turn wasted\n");
    return 1;
  }
  people[cur].wealth++;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
  return 0;
  // if cant do... return 1 for fail and ask again?
}

int foreignAid(int cur) {
  if(people[cur].wealth == 10) {
    printf("you can no longer gain wealth, turn wasted\n");
    block(cur, 9, 5);
    return 1;
  }
  people[cur].wealth += 2;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
  block(cur, 9, 5);
  return 0;
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
    getInput(numPlayer - 1);
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
  int size = 1;
  if(tempN == 3)
    size = 2;
  getInput(size);
  int ans = atoi(input);
  char dead[charMax] = "DEAD ";
  if(tempN == 3) { //when both influences are still alive
    if(ans == 0) {
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

// 0 for challenge failed, 1 for challenger succeeded
int challenge(int challenger, int challenged, int card) {
  if(strcmp(people[challenged].i1, cards[card]) == 0 ||
     strcmp(people[challenged].i2, cards[card]) == 0) { //failed challege
    printf("\nplayer %s, you failed to challenge player %s\nyou must reveal an influence: \n", people[challenger].name, people[challenged].name);
    reveal(challenger);
    return 0;
  } else { //successful challenge
    printf("\nplayer %s, player %s successfully challenged you\nyou must reveal an influence:\n", people[challenged].name, people[challenger].name);
    reveal(challenged);
    return 1;
  }
}

//change when to return 1, aka exit early
//change so when action isnt blocked, effect takes place!!!
int block(int cur, int def, int ans) {
  int c;
  for(c = 1; c < numPlayer; c++) {
    int result;
    int temp = (c + cur) % numPlayer;
    if(temp == def)
      printf("\nplayer %s: player %s attempted to %s\n", people[temp].name, people[cur].name, actions[ans]);
    else if(ans == 1) //steal
      printf("\nplayer %s: player %s attempted to %s from %s\n", people[temp].name, people[cur].name, turnActions[ans], people[def].name);
    else if(ans == 2) //assinate
      printf("\nplayer %s: player %s attempted to %s %s\n", people[temp].name, people[cur].name, turnActions[ans], people[def].name);
    else
      printf("\nplayer %s: player %s attempted to %s\n", people[temp].name, people[curPlayer].name, turnActions[ans]);
    if(ans != 5)
      printf("0. allow   1. challenge   ");
    int size = 2;
    switch(ans) {
      //tax, exchange, is default
      //income, coup not blocked
    case 1: //steal
      if(temp == def) {
	printf("2. block with %s   3. block with %s\n", cards[1], cards[3]);
	size = 4;
      } else {
	printf("\n");
      }
      break;
    case 2: //assassinate
      if(temp == def) {
	printf("2. block with %s\n", cards[4]);
	size = 3;
      } else {
	printf("\n");
      }
      break;
    case 5: //foreign-aid
      printf("0. allow   1. block with duke\n");
      size = 2;
      break;
    default:
      printf("\n");
    }
    getInput();
    check(size);
    int tCounter;
    int tAns = atoi(input);
    if(ans == 2 && temp == def && tAns == 0) { //assassination happens
      reveal(def);
      return 1;
    } else if(ans == 2 && temp == def && tAns == 1) {
      result = challenge(temp, cur, ans);
      if(result == 0) { //cur did have assassin
	reveal(temp);
      }
      return 1;
    } else if(ans == 2 && tAns == 2) { //block with contessa, temp == def
      printf("you attempted to block with %s\nwaitng for challenges\n\n", cards[4]);
      for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	int tTemp = (tCounter + temp) % numPlayer;	  
	printf("player %s, player %s attempted to block with %s\n", people[tTemp].name, people[temp].name, cards[4]);
	printf("0. allow   1. challenge\n");
	getInput();
	check(2);
	int tTAns = atoi(input);
	if(tTAns == 0) {
	  people[curPlayer].wealth -= 2;
	}
	if(tTAns == 1) { //challenge
	  result = challenge(tTemp, temp, 4);
	  if(result == 1) { // temp didnt have contessa
	    reveal(temp);
	  }
	  //need to break for loop;
	  //need to undo action of challenged
	  break; //uhhh
	}
      }
    } else if(ans == 5 && tAns == 1) { //foreign-aid
      printf("you attempted to block with %s\nwaiting for challenges\n\n", cards[0]);
      for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	int tTemp = (tCounter + temp) % numPlayer;	  
	printf("player %s, player %s attempted to block with duke\n", people[tTemp].name, people[temp].name);
	printf("0. allow   1. challenge\n");
	getInput();
	check(2);
	int tTAns = atoi(input);
	if(tTAns == 0) {
	  people[curPlayer].wealth -= 2;
	}
	if(tTAns == 1) { //challenge
	  result = challenge(tTemp, temp, 0);
	      
	  //need to break for loop;
	  //need to undo action of challenged
	  break; 
	}
      }
      return 1;
    } else if(ans == 1 && (tAns == 2 || tAns == 3)) { //implicit temp == def
      int reCard;
      if(tAns == 2)
	reCard = 1;
      if(tAns == 3)
	reCard = 3;
      
      printf("you attempted to block with %s\nwaiting for challenges\n\n", cards[reCard]);
      for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	int tTemp = (tCounter + temp) % numPlayer;	  
	printf("player %s, player %s attempted to block with %s\n", people[tTemp].name, people[temp].name, cards[reCard]);
	printf("0. allow   1. challenge\n");
	getInput();
	check(2);
	int tTAns = atoi(input);
	if(tTAns == 0) {
	  people[curPlayer].wealth -= 2;
	}
	if(tTAns == 1) { //challenge
	  result = challenge(tTemp, temp, 0);
	      
	  //need to break for loop;
	  //need to undo action of challenged
	  break;
	}
      }
      return 1; //steal happens, other players dont need to challenge anymore?
    } else if(tAns == 1) { //challenge tax, exchange
      result = challenge(temp, cur, ans);
      //need to break for loop;
      //need to undo action of challenged
      return 1;
    } else { //tAns == 0 allow
      ;
    }
  }
}

void turn() {
  //needed for printing chat hist?
  orders[ocounter] = curPlayer;
  ocounter++;
  printf("your turn!\n%d: player %s\n0. tax   1. steal   2. assassinate   3. exchange   4. income   5. foreign-aid   6. coup\nenter the action you wish to do:\n", curPlayer, people[curPlayer].name);
  getInput();
  check(7);
  
  int ans = atoi(input);
  orders[ocounter] = ans;
  int redo;
  //switches???
  switch(ans) {
  case 0:
    tax(curPlayer);
    break;
  case 1:
    steal(curPlayer);
    break;
  case 2:
    assassinate(curPlayer);
    break;
  case 3:
    exchange(curPlayer);
    break;
  case 4:
    redo = income(curPlayer);
    break;
  case 5:
    foreignAid(curPlayer);
    break;
  case 6:
    coup(curPlayer);
    break;
  default:
    ;
  }
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

int playersJoin() {
  printf("-----COUP-----\n");
  printf("enter \'exit\' if you ever wish to close the game\n\n\n");
  printf("hello, player 1 enter the number of players: (max 5)\n");
  getInput();
  check(6);
  numPlayer = atoi(input); 
  people = calloc(numPlayer, sizeof(struct player));
}

int runGame() {
  setup();
  curPlayer = myrand() % numPlayer;
  //otherFunction
  while(endGame) {
    //play action
    //challenge
    //block action
    //undo action
    printf("\n\n\n");
    printInfo(curPlayer);
    //skip dead players
    if(people[curPlayer].i1[0] == 'D' && people[curPlayer].i2[0] == 'D') {
      curPlayer++;
      curPlayer = curPlayer % numPlayer;
    } else {
      turn();
      printf("\n\n\n");
      //printHistory();
      ocounter++;
      curPlayer++;
      curPlayer = curPlayer % numPlayer;
    }
    printf("\n\n\n");
    gameEnd();
  }
}

//limit inputs to only acceptable answers
int main() {
  playersJoin();
  runGame();
}
