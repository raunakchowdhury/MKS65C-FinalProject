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
//'move history'
//'data'
//'what prompt'
int main() {
  char pause = 1;
  char orders[1000]; //calloc? temp
  char curPlayer = 'a';
  int nPlayers = 5;
  char playerMax = curPlayer + nPlayers;
  int ocounter = 0;
  char input[100];
  char line;
  char endGame = 1;
  char pMoney[nPlayers];
  char infl1[nPlayers];
  char infl2[nPlayers];
  char deck[15];
  random(deck);
  draw
    random from deck infl1
 
  while(endGame) {
    while(pause) {
      pause = 0;
    }
    printf("player %c answer response\n", curPlayer);
    fgets(input, sizeof(input), stdin);
    line = input[0];
    orders[ocounter] = curPlayer;
    ocounter++;
    orders[ocounter] = line;
    ocounter++;
    curPlayer++;
    if(curPlayer == playerMax)
      curPlayer = 'a';
    printf("\n\n\n\n\n\n\n\n\n\n");
    int c = 0;
    for(c; c < ocounter; c += 2)
      printf("player %c, %c\n", orders[c], orders[c+1]);
    
  }
}
