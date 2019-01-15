#define BUFFER_SIZE 256
#define MAX 5 //adjust this val for num of players

struct client {
  int client_socket;
  char name[BUFFER_SIZE];
};

//holds all the info for server integration
char names[MAX][BUFFER_SIZE];
struct client clients[MAX]; // array of players
char client_answer[BUFFER_SIZE];


int player_num;
void announce(char * msg);

// server stuff
void master_game(struct client clients[]);

void gameSetup();

char * clientinput(int cur);

void sendtoclient(int cur, char line[]);
