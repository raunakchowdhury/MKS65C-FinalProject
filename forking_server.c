#include "networking.h"
#include "game.h"

int player_num;

void process(char *s);
void subserver(int from_client);

// handle the SIGINT singla by deleting the well_known_pipe(WKP)
static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Closing pipes...\n");
    if(unlink("to_child") != -1){
      printf("Pipe to_child deleted.\n" );
    }
    if(unlink("to_parent") != -1){
      printf("Pipe to_parent deleted.\n" );
    }
    exit(0);
  }
}

int main() {

  int listen_socket;
  int f;
  int to_child;
  int from_child;
  listen_socket = server_setup();

  signal(SIGINT, sighandler);

  while (1) {
    // //unlink pipes for cross-server communication
    // unlink("to_parent");
    // unlink("to_child");
    // printf("pipes deleted!\n");

    //recreate pipes for further connections
    if (!mkfifo("to_child", 0644)){
      printf("Pipe to_child initialized!\n");
    }
    if (!mkfifo("to_parent", 0644)){
      printf("Pipe to_parent initialized!\n");
    }
    int client_socket = server_connect(listen_socket);
    player_num++;
    printf("Player %d joined the game.\n", player_num);

    f = fork();
    if (f == 0){
      // establish connection on this side as well
      // printf("Opening ends...\n" );
      int to_parent = open("to_parent", O_WRONLY);
      int from_parent = open("to_child", O_RDONLY);
      // printf("ends opened!\n" );
      subserver(client_socket);
    }
    else{
      // printf("opening ends..\n" );
      // open pipes to child
      from_child = open("to_parent", O_RDONLY);
      to_child = open("to_child", O_WRONLY);
      // printf("ends opened!\n" );
      close(client_socket);
    }
    //once finished adding players, move on to the actual game
    // game_server(): does all the game control stuff
  }
}

void subserver(int client_socket) {

  play_game(client_socket);
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
