# include "game.h"

void play_game(int client_socket, int from_parent, int to_parent){
  /*
   * subserver method
   */
  char buffer[BUFFER_SIZE];

  // main server should prompt subserver to move, which the prompts the client
  // subserver waits on main server to respond before moving client-side
  // while(read(from_parent, buffer, sizeof(buffer))){
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
   int i;
   char buffer[BUFFER_SIZE];

   strcpy(buffer, "Hello world!");

   for (i = 0; i < MAX; i++){
     printf("[Main server]: to_child: %d \n", subservers[i].to_child);
     write(subservers[i].to_child, buffer, sizeof(buffer));
   }
   printf("[Main server]: processes done\n");
   while(1){}
}
