#include "networking.h"
#include "coupGame.h"
int respond(int server_socket);

int respond(int server_socket){
  /*
   * Lets the client know if they should be responding or not
   */
   char buffer[BUFFER_SIZE];
   read(server_socket, buffer, sizeof(buffer));
   //printf("\nRecieved: [%s]\n", buffer);

   // send junk to facilitate next read
   // write(server_socket, "ye", sizeof("ye"));
   if (strcmp(buffer, "y") == 0){
     return 1;
   }
   // server will respond with "n" if it does not want the client to respond
   return 0;
}


int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  // char arrays for determining valid input
  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  int readResult = read(server_socket, buffer, sizeof(buffer));
  while (readResult) {
    if(strcmp("exit", buffer) == 0) {
      exit(0);
    }
    if (strcmp("Make a choice!", buffer) == 0) {

      printf("%s- ", BLU);
      fgets(buffer, sizeof(buffer), stdin);
      printf("%s", NRM);
      *strchr(buffer, '\n') = 0;
      write(server_socket, buffer, sizeof(buffer));
    }
    else{
      printf("%s", buffer);
    }
    readResult = read(server_socket, buffer, sizeof(buffer));
  }
  // if buffer needs to be calloc'd, just rm here
}
