#include "networking.h"
#include "coupGame.h"
#include "server.h"

// handle the SIGINT singla by deleting the well_known_pipe(WKP)
static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Closing game!\n");
    exit(0);
  }
}

int main() {

  player_num = 0;
  int listen_socket;
  int yes_count = 0; // tally up yesses and compare at end
  int i;
  struct client player;
  char msg[BUFFER_SIZE]; // holds msgs
  char buffer[BUFFER_SIZE];
  char string_int[2]; //holds the stringified player_num when sending messages

  listen_socket = server_setup();

  signal(SIGINT, sighandler);

  while (player_num < MAX) {

    // setup client-server connections
    printf("Awaiting connections...\n");
    int client_socket = server_connect(listen_socket);

    // Allow the client to set his/her name
    strcpy(buffer, "Type your name!");
    write(client_socket, buffer, sizeof(buffer));

    // Give client permission to respond
    write(client_socket, "Make a choice!", sizeof("Make a choice!"));

    // actual read
    read(client_socket, buffer, sizeof(buffer));

    // Add him/her to the array of current players, and announce that he/she has joined the game
    // NOTE: ADD A SIGNAL HANDLER TO CLIENT WHEN HE/SHE EXITS
    strcpy(player.name, buffer);
    player.client_socket = client_socket;
    clients[player_num] = player;
    player_num++;

    strcpy(msg, player.name);
    strcat(msg, " has joined the lobby!\n");
    printf("%s\n", msg);
    announce(msg);

    // If there is more than one player in the game, ask all if they want to start
    // printf("Requesting assent from players\n" );
    if (player_num > 1){
      // assemble message for next code section
      strcpy(msg, "There are currently ");
      sprintf(string_int, "%d", player_num);
      strcat(msg, string_int);
      strcat(msg, " players in the game. Would you like to proceed with the game (y/n)?\n");
      yes_count = 0;

      // Loop through the players and ask them if they want to begin
      for (i = 0; i < player_num; i++){
        // printf("Sending message to %s...\n", clients[i].name);
        write(clients[i].client_socket, msg, sizeof(msg));
        // Give client permission to respond
        write(clients[i].client_socket, "Make a choice!", sizeof("Make a choice!"));
        read(clients[i].client_socket, buffer, sizeof(buffer));

        // tally up yesses
        if(!strcmp(buffer, "y") || !strcmp(buffer, "Y")){
          yes_count++;
        }
      }
      // break out of the while loop if all players say yes
      if (yes_count == player_num){
        break;
      }
      // notify all players that not everyone voted yes
      strcpy(msg, "At a vote of ");
      sprintf(string_int, "%d", yes_count);
      strcat(msg, string_int);
      strcat(msg, " yes to ");
      sprintf(string_int, "%d", player_num - yes_count);
      strcat(msg, string_int);
      strcat(msg, " no, the server will continue waiting.\n");
      announce(msg);
      printf("%s\n", msg);
    }
  }
  //copy the names of clients into players for game integration purposes
  for (i = 0; i < player_num; i++){
    strcpy(names[i], clients[i].name);
  }
  strcpy(msg, "\n\nStarting game!\n" );
  // Notify all players
  announce(msg);

  //sendtoclient(1, msg);


  //
  //
//runGame(player_num, names);
  preSetup();
  addPlayers(player_num, names);
  //playersJoin();
  setup();
  runGame();

  /* printf("\n\nStarting game!\n"); */

  /* strcpy(buffer, "\nTesting function!"); */
  // sendtoclient(0);
  // clientinput(0);
}

void gameSetup() {
  /*
   * Sets up the game
   * If all current players agree on palying, the game starts
   */
}



void announce(char msg[]){
  /*
   * Announces msg to all the players. No further action is needed from the players.
   */
   int i;
   char buffer[BUFFER_SIZE];
   strcpy(buffer, msg);
   for (i = 0; i < player_num; i++){
     write(clients[i].client_socket, buffer, sizeof(buffer));
     // write(clients[i].client_socket, "n", sizeof("n"));
   }
}

char * clientinput(int cur){
  /*
   * Asks the designated client for a response.
   */

   // printf(line, "%.100s%d- ", BLU);
   //
   // fgets(input, sizeof(input), stdin);
   // printf(line,"%.100s", NRM);

   // input[strlen(input) - 1] = 0; //removes newLine

  char buffer[BUFFER_SIZE];
  printf("\n\nasking %d to write!\n", cur);
  strcpy(buffer, "Make a choice!");
  struct client player = clients[cur];
  write(player.client_socket, buffer, sizeof(buffer));
  // printf("Filler sent!\n" );
  // write(player.client_socket, "y", sizeof("y"));
  // printf("Perm sent!\n" );
  read(player.client_socket, client_answer, sizeof(client_answer));
  return client_answer;
}


void sendtoclient(int cur, char line[]) {
  /*
   * Send buf to client.
   */
   // you want it to announce to all
   char buf[BUFFER_SIZE];
   //strcpy(line, "erljkhgukghjkdbjkbdfjkbjkdfjkbjkb");
   // strcpy(buf, "#@!");
   // strcat(buf, line);
   strcpy(buf, line);
   printf("%s", buf);
   if (cur == 9){
     announce(buf);
   }
   else{
     write(clients[cur].client_socket, buf, sizeof(buf));
   }
}
