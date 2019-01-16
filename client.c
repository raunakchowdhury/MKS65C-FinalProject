#include "networking.h"
#include "coupGame.h"

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
    if (strcmp("Make a choice!", buffer) == 0) {

      printf("%s- ", BLU);
      fgets(buffer, sizeof(buffer), stdin);
      printf("%s", NRM);
      *strchr(buffer, '\n') = 0;
      if(strcmp("exit", buffer) == 0) {
	write(server_socket, buffer, sizeof(buffer));
	printf("game ended because you left\n");
	exit(0);
      }
      write(server_socket, buffer, sizeof(buffer));
    }
    else if(strcmp("-----GAME END-----\n", buffer) == 0) {
      printf("%s", buffer);
      exit(0);
    }
    else {
      printf("%s", buffer);
    }
    readResult = read(server_socket, buffer, sizeof(buffer));
  }
  // if buffer needs to be calloc'd, just rm here
}
