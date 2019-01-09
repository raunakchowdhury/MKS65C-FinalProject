# include "game.h"

void play_game(int client_socket){
  /*
   * subserver method
   */
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    /*
     * GAME HERE
     */

    strcpy(buffer, "yeet recieved");
    //process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
}
