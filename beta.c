#include "beta.h"
//#include ..
//random number from 0-14

//ur current main -> playersJoin() return char[0-4][]
//map 0-4 -> client_socket
//getInput(2)
//read(input, map[2]);



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

//char * getInput(int cur)

//printTo(int cur)
// -printf replacement, printInfo replacement
void getInput(int cur) {
  //buf = clientInput(cur);
  //strcpy(input, buf);

  sprintf(line, "%.100s%d- ", BLU, cur);
  print(cur);
  fgets(input, sizeof(input), stdin);
  sprintf(line,"%.100s", NRM);
  print(cur);
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
  //sentToClient(cur, line);
  printf("%d<<<%s", client, line);
}


//int 9 ->print to every
//int 0-4 print()
//printBuff
//write(map[4], printBuff)
void printInfo(int cur) {
  char info[charMax] = "";
  char tString[charMax] = "";
  strcat(info, "-----player info-----\n");
  for(counter = 0; counter < numPlayer; counter++) {
    if(counter == cur) {
      sprintf(tString, "%.100s%.100s: $%d%.100s, %.100s, %.100s\n", GRN, people[counter].name, people[counter].wealth, NRM, people[counter].i1, people[counter].i2);
      strcat(info, tString);
    }
    else {
      sprintf(tString, "%.100s: $%d, ", people[counter].name, people[counter].wealth);
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

void check(int cur, int size) {
  int correct = 1;
  for(counter = 0; counter < size; counter++) {
    if (strcmp(input, accepted[counter]) == 0)
      correct = 0;
  }
  while(correct == 1) {

    sprintf(line, "your input does not match with the options provided\nre-enter\n");
    print(cur);
    getInput(cur);
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
    /* printf("player %d, please enter your name:\n", counter); */
    /* getInput(); */
    /* strcpy(people[counter].name, BLU); */
    /* strcat(people[counter].name, input); */
    /* strcat(people[counter].name, NRM); */
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
  sprintf(line, "%.100s attempted to tax\nwaiting for challenges\n", people[cur].name);
  print(9);
  block(cur, 9, 0);
  people[cur].wealth += 3;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
}

void steal(int cur) {
  sprintf(line, "steal:\n");
  print(cur);
  int answers[7];
  int ansC = 0;
  for(counter = 0; counter < numPlayer; counter++) {
    if(people[counter].revealed == 3 || counter == cur || people[counter].wealth == 0) { //logic flaw if everyone 0 wealth
      ;
    } else {
      sprintf(line, "%d. %.100s   ", ansC, people[counter].name);
      print(cur);
      answers[ansC] = counter;
      ansC++;
    }
  }
  sprintf(line, "\n");
  print(cur);
  getInput(cur);
  check(cur, ansC);
  int tAns = atoi(input);
  tAns = answers[tAns];
  sprintf(line, "%.100s attempted to steal from %.100s...\nwaiting for blocks/challenges...\n", people[cur].name, people[tAns].name);
  print(9);
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
  sprintf(line, "assassinate:\n");
  print(cur);
  int answers[7];
  int ansC = 0;
  for(counter = 0; counter < numPlayer; counter++) {
    if(people[counter].revealed == 3 || counter == cur) {
      ;
    } else {
      sprintf(line, "%d. %.100s   ", ansC, people[counter].name);
      print(cur);
      answers[ansC] = counter;
      ansC++;
    }
  }
  sprintf(line, "\n");
  print(cur);
  getInput(cur);
  check(cur, ansC);
  int tAns = atoi(input);
  tAns = answers[tAns];
  sprintf(line, "%.100s attempted to assassinate %.100s...\nwaiting for player %.100s to reveal an influence...\n", people[cur].name, people[tAns].name, people[tAns].name);
  print(9);
  block(cur, tAns, 2);
}

void exchange(int cur) {
  sprintf(line, "%.100s attempted to exchange...\nwaiting for blocks/challenges...\n", people[cur].name);
  print(9);
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
  

  sprintf(line, "\nchoose a role to keep:\n");
  print(cur);
  sprintf(line, "0. %.100s   1. %.100s   ", cards[options[0] / 3], cards[options[1] / 3]);
  print(cur);
  switch(tempN) {
  case 1:
    sprintf(line, "2. %.100s\n", cards[options[3] / 3]);
    print(cur);
    size = 3;
    break;
  case 2:
    sprintf(line, "2. %.100s\n", cards[options[2] / 3]);
    print(cur);
    size = 3;
    break;
  default:
    sprintf(line, "2. %.100s   3. %.100s\n", cards[options[2] / 3], cards[options[3] / 3]);
    print(cur);
  }
  getInput(cur);
  check(cur, size);
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
    sprintf(line, "choose another role to keep:\n");
    print(cur);
    sprintf(line, "0. %.100s   1. %.100s   2. %.100s\n", cards[options[0] / 3], cards[options[1] / 3], cards[options[2] / 3]);
    print(cur);
    getInput(cur);
    check(cur, 3);
    int tAns = atoi(input);
    if(strcmp(people[cur].i2, cards[options[tAns] / 3]) == 0) {
      sprintf(line, "nothing changed\n");
      print(cur);
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
  sprintf(line, "%.100s attempted to draw foreign-aid...\nwaiting for blocks...\n", people[cur].name);
  print(9);
  block(cur, 9, 5);
  //check retirm value to make things happen
  // *** also check what block changes
  people[cur].wealth += 2;
  if(people[cur].wealth > 10)
    people[cur].wealth = 10;
}

void coup(int cur) {
  people[cur].wealth -= 7;
  sprintf(line, "coup:\n");
  print(cur);
  for(counter = 0; counter < numPlayer; counter++) {
    if(counter < cur) {
      sprintf(line, "%d. %.100s   ", counter, people[counter].name);
      print(cur);
    } else if(counter == cur)
      ;
    else {
      sprintf(line, "%d. %.100s   ", counter - 1, people[counter].name);
      print(cur);
    }
  }
  sprintf(line, "\n");
  print(cur);
  getInput(cur);
  check(cur, numPlayer - 1);
  int tAns = atoi(input);
  sprintf(line, "%.100s staged a coup...\nwaiting for %.100s to reveal an influence...\n", people[cur].name, people[tAns].name);
  print(9);
  if(tAns >= cur)
    tAns++;
  //reveal(int cur)  tAns
  sprintf(line, "\n%.100s staged a coup on %.100s\n%.100s must reveal an influence:\n", people[cur].name, people[tAns].name, people[tAns].name);
  print(9);
  reveal(tAns);
 
}


void reveal(int cur) {
  int answers[2];
  int size = 1;

  //printing choices avaiable
  if(people[cur].revealed == 1) {
    sprintf(line, "0. %.100s\n", people[cur].i2);
    print(cur);
    answers[0] = 2;
  } else if(people[cur].revealed == 2) {
    sprintf(line, "0. %.100s\n", people[cur].i1);
    print(cur);
    answers[0] = 1;
  } else {
    sprintf(line, "0. %.100s   1. %.100s\n", people[cur].i1, people[cur].i2);
    print(cur);
    answers[0] = 1;
    answers[1] = 2;
    size = 2;
  }
  getInput(cur);
  check(cur, size);
  int ans = atoi(input);
  char dead[charMax] = "DEAD ";
  ans = answers[ans];
  if(ans == 1) {
    strcat(dead, people[cur].i1);
    strcpy(people[cur].i1, dead);
    people[cur].revealed += ans;
    sprintf(line, "%.100s revealed %.100s", people[cur].name, people[cur].i1);
    print(9);
  }
  if(ans == 2) {
    strcat(dead, people[cur].i2);
    strcpy(people[cur].i2, dead);
    people[cur].revealed += ans;
    sprintf(line, "%.100s revealed %.100s", people[cur].name, people[cur].i1);
    print(9);
  }
}

// 0 for challenge failed, 1 for challenger succeeded
int challenge(int challenger, int challenged, int card) {
  if(strcmp(people[challenged].i1, cards[card]) == 0 ||
     strcmp(people[challenged].i2, cards[card]) == 0) { //failed challege
    sprintf(line, "\n%.100s failed to challenge %.100s\n%.100s must reveal an influence: \n", people[challenger].name, people[challenged].name, people[challenger].name);
    print(9);
    reveal(challenger);
    return 0;
  } else { //successful challenge
    sprintf(line, "\n%.100s successfully challenged %.100s\n%.100s must reveal an influence:\n", people[challenger].name, people[challenged].name, people[challenged].name);
    print(9);
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
      if(temp == def) {
	sprintf(line, "\n%.100s: %.100s attempted to %.100s\n", people[temp].name, people[cur].name, actions[ans]);
	print(temp);
      } else if(ans == 1) {//steal
	sprintf(line, "\n%.100s: %.100s attempted to %.100s from %.100s\n", people[temp].name, people[cur].name, turnActions[ans], people[def].name);
	print(temp);
      } else if(ans == 2) {//assinate
	sprintf(line, "\n%.100s: %.100s attempted to %.100s %.100s\n", people[temp].name, people[cur].name, turnActions[ans], people[def].name);
	print(temp);
      } else { //general
	sprintf(line, "\%.100s: %.100s attempted to %.100s\n", people[temp].name, people[curPlayer].name, turnActions[ans]);
	print(temp);
      }
      if(ans != 5) {
	sprintf(line, "0. allow   1. challenge   ");
	print(temp);
      }
      int size = 2;
      switch(ans) {
	//tax, exchange, is default
	//income, coup not blocked
      case 1: //steal
	if(temp == def) {
	  sprintf(line, "2. block with %.100s   3. block with %.100s\n", cards[1], cards[3]);
	  print(temp);
	  size = 4;
	} else {
	  sprintf(line, "\n");
	  print(temp);
	}
	break;
      case 2: //assassinate
	if(temp == def) {
	  sprintf(line, "2. block with %.100s\n", cards[4]);
	  print(temp);
	  size = 3;
	} else {
	  sprintf(line, "\n");
	  print(temp);
	}
	break;
      case 5: //foreign-aid
	sprintf(line, "0. allow   1. block with duke\n");
	print(temp);
	size = 2;
	break;
      default:
	sprintf(line, "\n");
	print(temp);
      }
      getInput(temp);
      check(temp, size);
      int tCounter;
      int tAns = atoi(input);
      if(ans == 2 && temp == def && tAns == 0) { //assassination happens
	sprintf(line, "%.100s must reveal an influence\n", people[temp].name);
	print(9);
	reveal(def);
	return 1;
      } else if(ans == 2 && temp == def && tAns == 1) {
	result = challenge(temp, cur, ans);
	if(result == 0) { //cur did have assassin
	  sprintf(line, "%.100s must reveal an influence\n", people[temp].name);
	  print(9);
	  reveal(temp);
	}
	return 1;
      } else if(ans == 2 && tAns == 2) { //block with contessa, temp == def
	sprintf(line, "%.100s attempted to block with %.100s\nwaitng for challenges\n\n", people[temp].name, cards[4]);
	print(9);
	for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	  int tTemp = (tCounter + temp) % numPlayer;
	  if(people[tTemp].revealed != 3) {
	    sprintf(line, "%.100s: %.100s attempted to block with %.100s\n", people[tTemp].name, people[temp].name, cards[4]);
	    print(tTemp);
	    sprintf(line, "0. allow   1. challenge\n");
	    print(tTemp);
	    getInput(tTemp);
	    check(tTemp, 2);
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
	sprintf(line, "%.100s attempted to block with %.100s\nwaiting for challenges\n\n", people[temp].name, cards[0]);
	print(9);
	for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	  int tTemp = (tCounter + temp) % numPlayer;
	  if(people[tTemp].revealed != 3) {
	    sprintf(line, "%.100s: %.100s attempted to block with duke\n", people[tTemp].name, people[temp].name);
	    print(tTemp);
	    sprintf(line, "0. allow   1. challenge\n");
	    print(tTemp);
	    getInput(tTemp);
	    check(tTemp, 2);
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
      
	sprintf(line, "%.100s attempted to block with %.100s\nwaiting for challenges\n\n", people[temp].name, cards[reCard]);
	print(9);
	for(tCounter = 1; tCounter < numPlayer ; tCounter++) {
	  int tTemp = (tCounter + temp) % numPlayer;
	  if(people[temp].revealed != 3) {
	    sprintf(line, "%.100s: %.100s attempted to block with %.100s\n", people[tTemp].name, people[temp].name, cards[reCard]);
	    print(tTemp);
	    sprintf(line, "0. allow   1. challenge\n");
	    print(tTemp);
	    getInput(tTemp);
	    check(tTemp, 2);
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
  sprintf(line, "%.100s\'s turn!\n", people[curPlayer].name);
  print(9);
  sprintf(line, "0. tax   1. steal   2. assassinate   3. exchange   4. income   5. foreign-aid   6. coup\nenter the action you wish to do:\n");
  print(curPlayer);
  getInput(curPlayer);
  check(curPlayer, 7);
  
  int ans = atoi(input);
  while(((ans == 0 || ans == 4 || ans == 5) && people[curPlayer].wealth == 10) ||
	(ans == 2 && people[curPlayer].wealth < 3) ||
	(ans == 6 && people[curPlayer].wealth < 7)) {
    if(ans == 0 || ans == 4 || ans == 5) {
      sprintf(line, "you can no longer gain wealth, choose another action\n");
      print(curPlayer);
    }
    if(ans == 2) {
      sprintf(line, "you cannot assassinate, choose another action\n");
      print(curPlayer);
    }
    if(ans == 6) {
      sprintf(line, "you cannot coup, choose another action\n");
      print(curPlayer);
    }
    getInput(curPlayer);
    check(curPlayer, 7);
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

  int ans = chooseAction();
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
    sprintf(line, "player %.100s won!!!\n-----GAME END-----\n", people[winner].name);
    print(9);
    endGame = 0;
  }
}

int playersJoin() {
  printf("-----COUP-----\n");
  printf("enter \'exit\' if you ever wish to close the game\n\n\n");
  printf("hello, player 0 enter the number of players: (max 5)\n");
  getInput(0);
  check(0, 6);
  numPlayer = atoi(input); 
  people = calloc(numPlayer, sizeof(struct player));
  for(counter = 0; counter < numPlayer; counter++) {
    printf("player %d, please enter your name:\n", counter);
    getInput(counter);
    strcpy(people[counter].name, BLU);
    strcat(people[counter].name, input);
    strcat(people[counter].name, NRM);
  }
}

int runGame() {
  /* for(counter = 0; counter < numPlayer; counter++) { */
  /*   //populates name, wealth */
  /*   struct player cur = people[counter]; */
  /*   strcpy(people[counter].name, names[counter]); */
  /* } */
  setup();
  curPlayer = myrand() % numPlayer;
  //otherFunction
  while(endGame) {
    //play action
    //challenge
    //block action
    //undo action
    sprintf(line, "\n\n\n");
    print(9);
    int c;
    //for(c = 0; c < numPlayers; c++) {
      printInfo(curPlayer);
      //printInfo(c);
      //print(c);
      print(9);
      //}
    //skip dead players
    if(people[curPlayer].revealed == 3) {
      curPlayer++;
      curPlayer = curPlayer % numPlayer;
    } else {
      turn();
      sprintf(line, "\n\n\n");
      print(9);
      //printHistory();
      curPlayer++;
      curPlayer = curPlayer % numPlayer;
    }
    sprintf(line, "\n\n\n");
    print(9);
    gameEnd();
  }
}

//limit inputs to only acceptable answers
int main() {
  playersJoin();
  runGame();
}
