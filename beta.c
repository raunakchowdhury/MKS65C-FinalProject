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
  printf("%.100s- ", BLU);
  fgets(input, sizeof(input), stdin);
  printf("%.100s", NRM);
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

void print(int client) {
  printf("%d\n%s", client, line);
}

void printInfo(int cur) {
  char info[charMax] = "";
  strcat(info, "-----player info-----\n");
  for(counter = 0; counter < numPlayer; counter++) {
    if(counter == cur) {
      sprintf(tString, "%.100splayer %.100s: $%d%.100s, %.100s, %.100s\n", GRN, people[counter].name, people[counter].wealth, NRM, people[counter].i1, people[counter].i2);
      strcat(info, tString);
    }
    else {
      sprintf(tString, "player %.100s: $%d, ", people[counter].name, people[counter].wealth);
      strcat(info, tString);
      if(people[counter].revealed == 1) {
	strcat(info, people[counter].i1);
	strcat(info, "%.100s, ");
      }
      else
	strcat(info, "unknown, ");
      if(people[counter].revealed == 2) {
	strcat(info, people[counter].i2);
	strcat(info, "\n");
      }
      else 
	strcat(info, "unknown\n");
    }
  }
  strcat(info, "\n");
  strcpy(line, info);
}

void printHistory() {
  //redisgn with char array... or file where every pub action is written
  printf("history:\n");
  for(counter = 0; counter < ocounter; counter += 2)
    printf("action %d: player %.100s,  %.100s\n", 1 + counter/2,
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
    people[counter].wealth = 7;
    people[counter].revealed = 0;
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

void tax(int cur) {
  printf("you attempted to tax\nwaiting for challenges\n");
  block(cur, 9, 0);
  people[cur].wealth += 3;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
}

void steal(int cur) {
  printf("steal:\n");
  int answers[7];
  int ansC = 0;
  for(counter = 0; counter < numPlayer; counter++) {
    if(people[counter].revealed == 3 || counter == cur || people[counter].wealth == 0) { //logic flaw if everyone 0 wealth
      ;
    } else {
      printf("%d. player %.100s   ", ansC, people[counter].name);
      answers[ansC] = counter;
      ansC++;
    }
  }
  printf("\n");
  getInput();
  check(ansC);
  int tAns = atoi(input);
  tAns = answers[tAns];
  printf("you attempted to steal from player %.100s...\nwaiting for blocks/challenges...\n", people[tAns].name);
  block(cur, tAns, 1);
  people[cur].wealth += 2;
  people[tAns].wealth -= 2;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
  if(people[tAns].wealth < 0)
    people[tAns].wealth = 0;
}

void assassinate(int cur) {
  people[cur].wealth -= 3;
  printf("assassinate:\n");

  int answers[7];
  int ansC = 0;
  for(counter = 0; counter < numPlayer; counter++) {
    if(people[counter].revealed == 3 || counter == cur) {
      ;
    } else {
      printf("%d. player %.100s   ", ansC, people[counter].name);
      answers[ansC] = counter;
      ansC++;
    }
  }
  printf("\n");
  getInput();
  check(ansC);
  int tAns = atoi(input);
  tAns = answers[tAns];
  printf("you attempted to assassinate player %.100s...\nwaiting for player %.100s to reveal an influence...\n", people[tAns].name, people[tAns].name);
  block(cur, tAns, 2);
}

void exchange(int cur) {
  printf("you attempted to exchange...\nwaiting for blocks/challenges...\n");
  block(cur, 9, 3);
  int options[4];
  int tempN = 0;
  int size = 4;
  int replace = 0;
  options[2] = findLoc(cur + 10);
  if(people[cur].revealed == 1) {
    tempN += 1;
    replace = 3;
  }
  options[3] = findLoc(cur + 20);
  if(people[cur].revealed == 2) {
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
  

  printf("\nplayer %.100s, choose a role to keep:\n", people[cur].name);
  printf("0. %.100s   1. %.100s   ", cards[options[0] / 3], cards[options[1] / 3]);
  switch(tempN) {
  case 1:
    printf("2. %.100s\n", cards[options[3] / 3]);
    size = 3;
    break;
  case 2:
    printf("2. %.100s\n", cards[options[2] / 3]);
    size = 3;
    break;
  default:
    printf("2. %.100s   3. %.100s\n", cards[options[2] / 3], cards[options[3] / 3]);
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
    printf("0. %.100s   1. %.100s   2. %.100s\n", cards[options[0] / 3], cards[options[1] / 3], cards[options[2] / 3]);
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
}

void income(int cur) {
  people[cur].wealth++;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
}

void foreignAid(int cur) {
  printf("you attempted to draw foreign-aid...\nwaiting for blocks...\n");
  block(cur, 9, 5);
  //check retirm value to make things happen
  // *** also check what block changes
  people[cur].wealth += 2;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
}

void coup(int cur) {
  people[cur].wealth -= 7;
  printf("coup:\n");
  for(counter = 0; counter < numPlayer; counter++) {
    if(counter < cur)
      printf("%d. player %.100s   ", counter, people[counter].name);
    else if(counter == cur)
      ;
    else
      printf("%d. player %.100s   ", counter - 1, people[counter].name);
  }
  printf("\n");
  getInput(numPlayer - 1);
  int tAns = atoi(input);
  printf("you staged a coup...\nwaiting for player %.100s to reveal an influence...\n", people[tAns].name);
  if(tAns >= cur)
    tAns++;
  //reveal(int cur)  tAns
  printf("\n\nplayer %.100s, player %.100s staged a coup on you\nyou must reveal an influence:\n", people[tAns].name, people[cur].name);
  reveal(tAns);
 
}


void reveal(int cur) {
  int answers[2];
  int size = 1;

  //printing choices avaiable
  if(people[cur].revealed == 1) {
    printf("0. %.100s\n", people[cur].i2);
    answers[0] = 2;
  } else if(people[cur].revealed == 2) {
    printf("0. %.100s\n", people[cur].i1);
    answers[0] = 1;
  } else {
    printf("0. %.100s   1. %.100s\n", people[cur].i1, people[cur].i2);
    answers[0] = 1;
    answers[1] = 2;
    size = 2;
  }
  getInput(size);
  int ans = atoi(input);
  char dead[charMax] = "DEAD ";
  ans = answers[ans];
  if(ans == 1) {
    strcat(dead, people[cur].i1);
    strcpy(people[cur].i1, dead);
    people[cur].revealed += ans;
  }
  if(ans == 2) {
    strcat(dead, people[cur].i2);
    strcpy(people[cur].i2, dead);
    people[cur].revealed += ans;
  }
}

// 0 for challenge failed, 1 for challenger succeeded
int challenge(int challenger, int challenged, int card) {
  if(strcmp(people[challenged].i1, cards[card]) == 0 ||
     strcmp(people[challenged].i2, cards[card]) == 0) { //failed challege
    printf("\nplayer %.100s, you failed to challenge player %.100s\nyou must reveal an influence: \n", people[challenger].name, people[challenged].name);
    reveal(challenger);
    return 0;
  } else { //successful challenge
    printf("\nplayer %.100s, player %.100s successfully challenged you\nyou must reveal an influence:\n", people[challenged].name, people[challenger].name);
    reveal(challenged);
    return 1;
  }
}

//change when to return 1, aka exit early
// 0 no blocks?
//change so when action isnt blocked, effect takes place!!!
int block(int cur, int def, int ans) {
  int c;
  for(c = 1; c < numPlayer; c++) {
    int result;
    int temp = (c + cur) % numPlayer;
    if(people[temp].revealed != 3) {
      if(temp == def)
	printf("\nplayer %.100s: player %.100s attempted to %.100s\n", people[temp].name, people[cur].name, actions[ans]);
      else if(ans == 1) //steal
	printf("\nplayer %.100s: player %.100s attempted to %.100s from %.100s\n", people[temp].name, people[cur].name, turnActions[ans], people[def].name);
      else if(ans == 2) //assinate
	printf("\nplayer %.100s: player %.100s attempted to %.100s %.100s\n", people[temp].name, people[cur].name, turnActions[ans], people[def].name);
      else
	printf("\nplayer %.100s: player %.100s attempted to %.100s\n", people[temp].name, people[curPlayer].name, turnActions[ans]);
      if(ans != 5)
	printf("0. allow   1. challenge   ");
      int size = 2;
      switch(ans) {
	//tax, exchange, is default
	//income, coup not blocked
      case 1: //steal
	if(temp == def) {
	  printf("2. block with %.100s   3. block with %.100s\n", cards[1], cards[3]);
	  size = 4;
	} else {
	  printf("\n");
	}
	break;
      case 2: //assassinate
	if(temp == def) {
	  printf("2. block with %.100s\n", cards[4]);
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
	printf("player %.100s: you must reveal an influence\n", people[temp].name);
	reveal(def);
	return 1;
      } else if(ans == 2 && temp == def && tAns == 1) {
	result = challenge(temp, cur, ans);
	if(result == 0) { //cur did have assassin
	  printf("player %.100s: you must reveal an influence\n", people[temp].name);
	  reveal(temp);
	}
	return 1;
      } else if(ans == 2 && tAns == 2) { //block with contessa, temp == def
	printf("you attempted to block with %.100s\nwaitng for challenges\n\n", cards[4]);
	for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	  int tTemp = (tCounter + temp) % numPlayer;
	  if(people[tTemp].revealed != 3) {
	    printf("player %.100s, player %.100s attempted to block with %.100s\n", people[tTemp].name, people[temp].name, cards[4]);
	    printf("0. allow   1. challenge\n");
	    getInput();
	    check(2);
	    int tTAns = atoi(input);
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
	}
      } else if(ans == 5 && tAns == 1) { //foreign-aid
	printf("you attempted to block with %.100s\nwaiting for challenges\n\n", cards[0]);
	for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	  int tTemp = (tCounter + temp) % numPlayer;
	  if(people[tTemp].revealed != 3) {
	    printf("player %.100s, player %.100s attempted to block with duke\n", people[tTemp].name, people[temp].name);
	    printf("0. allow   1. challenge\n");
	    getInput();
	    check(2);
	    int tTAns = atoi(input);
	    if(tTAns == 1) { //challenge
	      result = challenge(tTemp, temp, 0);
	      
	      //need to break for loop;
	      //need to undo action of challenged
	      break; 
	    }
	  }
	}
	return 1;
      } else if(ans == 1 && (tAns == 2 || tAns == 3)) { //implicit temp == def
	int reCard;
	if(tAns == 2)
	  reCard = 1;
	if(tAns == 3)
	  reCard = 3;
      
	printf("you attempted to block with %.100s\nwaiting for challenges\n\n", cards[reCard]);
	for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	  int tTemp = (tCounter + temp) % numPlayer;
	  if(people[temp].revealed != 3) {
	    printf("player %.100s, player %.100s attempted to block with %.100s\n", people[tTemp].name, people[temp].name, cards[reCard]);
	    printf("0. allow   1. challenge\n");
	    getInput();
	    check(2);
	    int tTAns = atoi(input);
	    if(tTAns == 1) { //challenge
	      result = challenge(tTemp, temp, 0);
	      
	      //need to break for loop;
	      //need to undo action of challenged
	      break;
	    }
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
}

int chooseAction() {
  printf("your turn!\n%d: player %.100s\n0. tax   1. steal   2. assassinate   3. exchange   4. income   5. foreign-aid   6. coup\nenter the action you wish to do:\n", curPlayer, people[curPlayer].name);
  getInput();
  check(7);
  
  int ans = atoi(input);
  while(((ans == 0 || ans == 4 || ans == 5) && people[curPlayer].wealth == 10) ||
	(ans == 2 && people[curPlayer].wealth < 3) ||
	(ans == 6 && people[curPlayer].wealth < 7)) {
    if(ans == 0 || ans == 4 || ans == 5)
      printf("you can no longer gain wealth, choose another action\n");
    if(ans == 2)
      printf("you cannot assassinate, choose another action\n");
    if(ans == 6)
      printf("you cannot coup, choose another action\n");
    getInput();
    check(7);
    ans = atoi(input);
  }
  return ans;
}

void actionEffect(int ans) {
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
    income(curPlayer);
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

void turn() {
  //needed for printing chat hist?
  orders[ocounter] = curPlayer;
  ocounter++;
 
  int ans = chooseAction();
  orders[ocounter] = ans;
  //switches???
  actionEffect(ans);
}

void gameEnd() {
  int deadPlayers = 0;
  int winner;
  for(counter = 0; counter < numPlayer; counter++) {
    if(people[counter].revealed == 3)
      deadPlayers++;
    else
      winner = counter;
  }
  if(deadPlayers + 1 == numPlayer) { //only one man standing
    printf("player %.100s won!!!\n", people[winner].name);
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
    print(9);
    //skip dead players
    if(people[curPlayer].revealed == 3) {
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
