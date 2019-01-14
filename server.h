#define BUFFER_SIZE 256
#define MAX 5 //adjust this val for num of players

struct client {
  int client_socket;
  char name[charMax];
};


int player_num = 0;
void announce(struct client clients[], int num_players, char * msg);

// server stuff
void master_game(struct client clients[]);


// char** gameSetup();
//
// clientinput(int cur);
//
// sendtoclient(int cur, char * buf);
