# include "game.h"

int remaining_players = MAX; //global var for tracking for winners
int take_action(struct client client);
void send_message(int client_socket, int message);

void play_game(int client_socket){
  /*
   * client method
   */
  char buffer[BUFFER_SIZE];
  int message;

  // main server should prompt client to move, which the prompts the client
  // client waits on main server to respond before moving client-side
  // while(read(from_client, buffer, sizeof(buffer))){
  //   message = int(buffer);
  //
  //   // HANDLE ANY EXCEPTIONS HERE
  //
  //   send_message(buffer);
  //   read(client_socket, buffer, sizeof(buffer));
  //   printf("[client %d] received from Main Server: [%s]\n", getpid(), buffer);
  // }

  // read(from_client, buffer, sizeof(buffer));
  printf("[client %d] received from Main Server: [%s]\n", getpid(), buffer);

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[client %d] received: [%s]\n", getpid(), buffer);
    /*
     * GAME HERE
     */

    strcpy(buffer, "yeet recieved");
    //process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop

  // read(from_client, buffer, sizeof(buffer));
  // printf("[client %d] received: [%s]\n", getpid(), buffer);
}

void master_game(struct client clients[]){
  /*
   * Loops through the clients, asking them to ask the client to make moves.
   * Does other coup decisions (such as challenging, etc.)
   */
   // play_game(clients);
}

void send_message(int client_socket, int message){
  /*
   * client perspective:
   * Based on the number/code that was sent by the main server, send the corresponding message to the client
   * Codes:
   * 0:
   * 1:
   * 2:
   */

}

int take_action(struct client client){
  /*
   * Main server perspective:
   * Has the client prompt the player to take an action.
   * Returns an int containing an action the player chose.
   */
   return 0;
}
