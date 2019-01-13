#include "networking.h"
#include "game.h"

int player_num;

void process(char *s);
void subserver(int from_client, int from_parent, int to_parent);
int is_valid_input(char buffer[], char ** possible_choices);

// handle the SIGINT singla by deleting the well_known_pipe(WKP)
static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Closing pipes...\n");
    if(unlink("to_client") != -1){
      printf("Pipe to_client deleted.\n" );
    }
    if(unlink("to_parent") != -1){
      printf("Pipe to_parent deleted.\n" );
    }
    exit(0);
  }
}

int main() {

  int listen_socket;
  int yes_count = 0; // tally up yesses and compare at end
  int i;
  struct client player;
  struct client clients[MAX]; // array of players
  char msg[BUFFER_SIZE]; // holds msgs
  char buffer[BUFFER_SIZE];
  char* possible_choices[4] = {"y", "Y", "n", "N"}; //used for force_valid_input
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
    write(client_socket, "y", sizeof("y"));

    // read in junk from client to facilitate next write
    // read(client_socket, buffer, sizeof(buffer));
    // write(client_socket, "1", sizeof("1"));
    // actual read
    read(client_socket, buffer, sizeof(buffer));

    // Add him/her to the array of current players, and announce that he/she has joined the game
    // NOTE: ADD A SIGNAL HANDLER TO CLIENT WHEN HE/SHE EXITS
    strcpy(player.name, buffer);
    player.client_socket = client_socket;
    clients[player_num] = player;
    player_num++;

    strcpy(msg, player.name);
    strcat(msg, " has joined the lobby!");
    printf("%s\n", msg);
    announce(clients, player_num, msg);

    // If there is more than one player in the game, ask all if they want to start
    // printf("Requesting assent from players\n" );
    if (player_num > 1){
      // assemble message for next code section
      strcpy(msg, "There are currently ");
      sprintf(string_int, "%d", player_num);
      strcat(msg, string_int);
      strcat(msg, " players in the game. Would you like to proceed with the game (y/n)?");
      yes_count = 0;

      // Loop through the players and ask them if they want to begin
      for (i = 0; i < player_num; i++){
        // printf("Sending message to %s...\n", clients[i].name);
        write(clients[i].client_socket, msg, sizeof(msg));
        // Give client permission to respond
        write(clients[i].client_socket, "y", sizeof("y"));
        // read(clients[i].client_socket, buffer, sizeof(buffer));
        // write(clients[i].client_socket, "1", sizeof("1"));
        //printf("Sent message!\n");
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
      strcat(msg, " no, the server will continue waiting.");
      announce(clients, player_num, msg);
      printf("%s\n", msg);
    }
  }

  strcpy(msg, "\n\nStarting game!\n" );
  // Notify all players
  announce(clients, player_num, msg);
  printf("\n\nStarting game!\n");

  //mmaster_game(clients); //does all the game control stuff
}

void announce(struct client clients[], int num_players, char* msg){
  /*
   * Announces msg to all the players. No further action is needed from the players.
   */
   int i;
   char buffer[BUFFER_SIZE];
   strcpy(buffer, msg);
   for (i = 0; i < num_players; i++){
     write(clients[i].client_socket, buffer, sizeof(buffer));
     write(clients[i].client_socket, "n", sizeof("n"));
   }
}

void subserver(int client_socket, int from_parent, int to_parent) {

  // play_game(client_socket, from_parent, to_parent);
  printf("Player %d has left the game.\n", player_num );
  close(client_socket);
  exit(0);
}

void process(char * s) {
  /*
  This would be the in-game process transmitted to the server?
  */
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }

}
