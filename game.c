# include "game.h"

int remaining_players = MAX; //global var for tracking for winners
int take_action(struct subserver subserver);
void send_message(int client_socket, int message);

void play_game(int client_socket, int from_parent, int to_parent){
  /*
   * subserver method
   */
  char buffer[BUFFER_SIZE];
  int message;

  // main server should prompt subserver to move, which the prompts the client
  // subserver waits on main server to respond before moving client-side
  // while(read(from_parent, buffer, sizeof(buffer))){
  //   message = int(buffer);

  //   HANDLE ANY EXCEPTIONS HERE

  //   send_message(buffer);
  //   read(client_socket, buffer, sizeof(buffer));
  //   printf("[subserver %d] received from Main Server: [%s]\n", getpid(), buffer);
  // }

  read(from_parent, buffer, sizeof(buffer));
  printf("[subserver %d] received from Main Server: [%s]\n", getpid(), buffer);

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    /*
     * GAME HERE
     */

    strcpy(buffer, "yeet recieved");
    //process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop

  // read(from_parent, buffer, sizeof(buffer));
  // printf("[subserver %d] received: [%s]\n", getpid(), buffer);
}

void master_game(struct subserver subservers[]){
  /*
   * Loops through the subservers, asking them to ask the client to make moves.
   * Does other coup decisions (such as challenging, etc.)
   */
   int i, j;
   int action_taken; // holds the action taken by the player
   char buffer[BUFFER_SIZE];

   strcpy(buffer, "Hello world!");

   // do shuffle stuff
   for (i = 0; i < MAX; i++){
     printf("[Main server]: to_child: %d \n", subservers[i].to_child);
     write(subservers[i].to_child, buffer, sizeof(buffer));
   }

   while(remaining_players > 1){
     for (i = 0; i < MAX; i++){
       //ask curr player to take a turn if he/she is not NULL
       if (!subservers[i].to_child){ // has no influences or whatever (INPUT HERE)

         //if this is a counterable action, ask each player if they wish to challenge that decision

         // if (counterable_action)
         for (j = 0; j < MAX; j++){
           // ask player by writing to the subserver, who recieves it and writes to client

           // if a challenge is issued, check cards and find a peace (find peace should kill clients if needed)
           // find_peace(subservers[i], subservers[j])
           // if find_peace results in no action, continue asking players to challenge; else, break
         }
         //end the turn
       }
     }
   }

   printf("[Main server]: processes done\n");
}

void send_message(int client_socket, int message){
  /*
   * Subserver perspective:
   * Based on the number/code that was sent by the main server, send the corresponding message to the client
   * Codes:
   * 0:
   * 1:
   * 2:
   */

}

int take_action(struct subserver subserver){
  /*
   * Main server perspective:
   * Has the subserver prompt the player to take an action.
   * Returns an int containing an action the player chose.
   */
   return 0;
}
