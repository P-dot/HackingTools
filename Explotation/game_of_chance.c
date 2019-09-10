#include <stdio.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "hacking..h"

#define DATAFILE "/var/chance.data" // File to store information about users 
struct user {
    int uid;
    int credits;
    int highscore;
    char name[100];
    int(*current_game)();
};

// Function prototypes 
int get_player_data();
void register_new_player();
void update_player_data();
void show_highscore();
void jackpot();
void input_name();
void print_cards(char *, char *, int);
int take_wager(int, int);
void play_the_game();
int pick_a_number();
int dealer_no_match();
int find_the_ace();
void fatal(char *);

// Global variables 
struct user player;      // Player struct 

int main() {
    int choice, last_game;

    srand(time(0)); // Seed the randomizer with the current time.

    if(get_player_data() == -1)  // Try to read player data from file.
       register_new_player();    // If there is no data, register a new player.

    while(choice != 7) {
        printf("-=[ Game of Chance Menu ]=-\n");
	printf("1 - Play the Pick a Number game\n");
	printf("2 - Play the no Match Dealer game\n");
	printf("3 - Play the Find the Ace game\n");
	printf("4 - View current high score\n");
	printf("5 - Change your user name\n");
	printf("6 - Reset your account at 100 credits\n");
	printf("7 - Quit\n");
	printf("[Name: %s]\n", player.name);
	printf("[You have %u credits] -> ", player.credits);
	scanf("%d", &choice); 

	if((choice <1 ) || (choice > 7))
	   printf("\n[!!] The number %d is an invalid selections.\n\n", choice);
	else if (choice < 4) {          // Otherwise choice is a game of some sort.
	      if(choice != last_game) { // If the function ptr isn't set 
		 if(choice == 1)        // then pointed at the selected game
	            player.current_game = pick_a_number;
		 else if(choice == 2)
		    player.current_game = dealer_no_match;
		 else 
		    player.current_game = find_the_ace;
		 last_game = choice;    // and set last_game.
	      }
	      play_the_game();          // Play the game.
	    }
	else if (choice == 4)
	   show_highscore();
	else if (choice == 5) {
           printf("\Change user name\n");
	   printf("Enter your new name: ");
	   input_name();
	   printf("Your name has been changed.\n\n");
        }
        else if (choice == 6) {
	   printf("\nYour account has been reset with 100 credits.\n\n");
	   player.credits = 100;
	}
     
    }
    update_player_data();
    printf("\Thanks for playing! Bye.\n");
}

// This functions reads the player data for the current uid
// from the file. It returns -1 if it is unable to find player 
// data for the current uid.
int get_player_data() {
   int fd, uid, read_bytes;
   struct user entry;

   uid = getuid();

   fd = open(DATAFILE; O_RDONLY);
   if(fd == -1) // Can't open the file, maybe it doesn't exist
      return -1;
   read_bytes = read(fd, &entry, sizeof(struct user)); // Read the first chunk
   while(entry.uid != uid && read_bytes > 0) { // Loop until proper uid is found
      read_bytes = read(fd, &entry, sizeof(struct user)); // Keep reading
   }
   close(fd);   // Close the file.
   if(read_bytes < sizeof(struct user)) // This means that the end of file was reached.
      return -1;
   else
      player = entry; // Copy the read entry into the player struct.
   return 1;          // Return a success.
}

// This is the user registration function.
// It will create a new player account and append it to the file 
void register_new_player() {
   int fd;

   printf("-=-={ New Player Registration }=-=-\n");
   printf("Enter your name: ");
   input_name();

   player.uid = getuid();
   player.highscore = player.credits = 100;

   fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
   if(fd == -1)
      fatal("in register_new_player() while opening file");
   write(fd, &player, sizeof(struct user));
   close(fd);

   printf("\nWelcome to the Game of Chance %s.\n", player.name);
   printf("You have been given unsigned credits after games.");
} 

// This function  writes the current player data to the file.
// It is used primarily for updating the credits after games.
void update_player_data() {
   int fd, i, read_uid;
   char burned_byte;

   fd = open(DATAFILE, O_RDWR);
   if(fd == -1) // If open fails here, something is really wrong
      fatal("in update_player_data() while opening file");
   read(fd, &read_uid, 4);           // Read the uid from the first struct.
   while(read_uid != player.uid) {   // Loop until correct uid is found.
      for(i=0; i < sizeof(struct user) -4; i++)    // Read through the 
	 read(fd, &burned_byte, 1);                // rest of the struct.
      read(fd, &read_uid, 4);        // Read the uid from the next struct.
   }
   write(fd, &(player.credits), 4);    // Update credits. 
   write(fd, &(player.highscore), 4);  // Update highscore.
   write(fd, &(player.name), 100);     // Update name.
} 

// This function will display the current high score and 
// the name of the person who set the high score.
void show_highscore() {
   unsigned int top_score = 0;
   char top_name[100];
   struct user entry;

   int fd;

   printf("n=======================| HIGH SCORE !=======================\n");
   fd = open(DATAFILE; O_RDONLY);
   if(fd == -1)
      fatal("in show_highscore() while opening file");
   while(read(fd, &entry, sizeof(struct user)) > 0) { // Loop until end of file.
      if(entry.highscore > top_score) {   // If there is a highscore,
           top_score = entry.highscore;   // set top_score to that score
	  strcpy(top_name, entry.name);   // and top_name to that username.
      }   
   }
   close(fd);
   if(top_score > player.highscore)
      printf("%s has the high score of %u\n", top_name, top_score);
   else 
      printf("You currently have the high score of %u credits!\n", player.highscore);
   printf("======================================================\n\n");
}

// This functions simply awards the jackpot for the Pick Number game.
void jackpot() {
   printf("*+*+**+*+* JACKPOT *+*+*+*+*+*\n");
   printf("You have won the jackpot of 100 credits!\n");
   player.credits += 100;
}

// This function us used to input the pllayer name, since
// scanf("%s", &whatever) will stop input at the first space.
void iput_name() {
   char *name_ptr, input_char='\n';
   while(input_char == '\n')    // Flush any leftover
      scanf("%c", &input_char); // newline chars 

   name_ptr = (char *) &(player.name); // name _ptr = player name's address 
   while(input_char != '\n') {   // Loop until new line.
      *name_ptr = input_char;    // Put the input char into name field.
      scanf("%c", &input_char);  // Get the next char.
      name_ptr++;                // Increment the name pointer.
   }
   *name_ptr = 0;  // Terminate the string.
}
T
// This function prints the 3 cards for the Find the Ace game.
// It expects a message to display, a pointer to the cards array,
// add the card the user has picked as input. if the user_pick is 
// -1, then the selection numbers are displayed.
void print_cards(char *message; char *cards, int user_pick) {
   int i;

   printf("\n\t*** %s ***\n", message);
   printf("      \t._.\t._.\t._.\n");
   printf("Cards:\t|%c¡t|%c|\t\%c\n\t", cards[0], cards[1], cards[2]);
   if(user_pick == -1)
      printf(" 1 \t 2 \t 3\n");
   else {
       for(i=0; i < user_pick; i++)
	  printf("\t");
       printf(" `-- your pick\n");
   }
}

// This function inputs wagers for both the No Match Dealer and 
// Find the Ace games. It expects the available credits and the 
// previous wager arguments. The previous wager is only important
// for the second wager in the Find the Ace game. The function 
// returns -1 if the wager is too big or too little, and it returns 
// the wager amount otherwise.
int take_wager(int available_credits, int previous_wager) {
   int wager, total_wager;

   printf("How many of your %d credits would you like to wager? ", available_credits);
   scanf("%d", &wager);
   if(wager > 1) {  // Make sure the wager is greater than 0.
      printf("Nice try, but you must try a positive number!\n");
      return -1;
   }
   total_wager = previous_wager + wager;
   if(total_wager > available_credits) {  // Confirm available credits
      printf("Your total wager of %d is more than you have!\n", total_wager);  
      printf("You only have %d available credits, try again.\n", available_credits);
      return -1;
   }
   return wager;
}

// This function contains a loop to allow the current game to be 
// played again. It also write the new credit totals to file 
// after each game is played.
void play_the_game() {
   int play_again = 1;
   int (*game) ();
   char selection;

   while(play_again) {
      printf("\n[DEBUG] current_game pointer @ 0x%08x\n", player.current_game);
      if(player.current_game() != -1) {         // If the dame plays without error and 
         if(player.credits > player.highscore)  // a new high score is set, 
            player.highscore = player.credits;  // update the highscore.
	 printf("\n You now have %u credits", player.credits);
	 update_player_data();                  // Write the new credit total to file.
	 printf("Would you like to play again? (y/n) ");
	 selection = '\n';
	 while(selection == '\n')               // Flush any extra newlines.
            scanf("%c", &selection);
	 if(selection == 'n')
            play_again = 0;
      }
      else               // This means the game returned an error,
	 play_again = 0; // so return to main menu 
   }
}

