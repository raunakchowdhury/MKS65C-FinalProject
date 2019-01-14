#include "networking.h"

int respond(int server_socket);
void force_valid_input(int server_socket, char buffer[], char * previous_instructions, char ** possible_choices);
int is_valid_input(char buffer[], char ** possible_choices);

int respond(int server_socket){
  /*
   * Lets the client know if they should be responding or not
   */
   char buffer[BUFFER_SIZE];
   printf("\nReading..\n" );
   read(server_socket, buffer, sizeof(buffer));
   printf("\nRecieved: [%s]\n", buffer);

   // send junk to facilitate next read
   // write(server_socket, "ye", sizeof("ye"));
   if (strcmp(buffer, "y") == 0){
     return 1;
   }
   // server will respond with "n" if it does not want the client to respond
   return 0;
}

void force_valid_input(int server_socket, char* buffer, char * previous_instructions, char ** possible_choices){
  /*
   * Checks if player's input is valid against the possible_choices.
   * If it is, changes the buffer to show the input.
   * If not, it repeats instructions until the player chooses something valid.
   */
   printf("enter response: ");
   fgets(buffer, sizeof(buffer), stdin);
   *strchr(buffer, '\n') = 0;

   while(!is_valid_input(buffer, possible_choices)){
     printf("Invalid input! Please try again!\n");
     printf("%s\n", previous_instructions);
     printf("enter response: ");
     fgets(buffer, sizeof(buffer), stdin);
     *strchr(buffer, '\n') = 0;
   }
}

int is_valid_input(char buffer[], char ** possible_choices){
  /*
   * Helper for force_valid_input.
   * Checks buffer against possible_choices.
   * Returns 1 if buffer is a valid input, 0 otherwise.
   */

   int i;
   while(!possible_choices[i]){
     // only return if it's true
     if (!strcmp(buffer, possible_choices[i])){
       return 1;
     }
     i++;
   }
   return 0;
}


int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  // char arrays for determining valid input
  char* yes_no[4] = {"y", "Y", "n", "N"};
  char* make_decision[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
  char* instructions;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  while (read(server_socket, buffer, sizeof(buffer))) {
    //printf("Reading buffer...\n");
    printf("%s\n", buffer);
    instructions = buffer;

    // if a response is needed from the client
    if (respond(server_socket)){
      // printf("reading buffer to determine response set...\n");

      // determine which set of integers is allowed
      // read(server_socket, buffer, sizeof(buffer));

      printf("enter response: ");
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      // if any input is valid
      // if(!strcmp(buffer, "1")){
      //   printf("enter response: ");
      //   fgets(buffer, sizeof(buffer), stdin);
      //   *strchr(buffer, '\n') = 0;
      // }
      // else if(!strcmp(buffer, "2"))
      //   force_valid_input(server_socket, buffer, instructions, yes_no);
      // else if(!strcmp(buffer, "3"))
      //   force_valid_input(server_socket, buffer, instructions, make_decision);
      write(server_socket, buffer, sizeof(buffer));
    }
  }
  // if buffer needs to be calloc'd, just rm here
}
