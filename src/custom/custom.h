#include "../kernel/gpio.h"

//This one is for containing the user input only
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 1000

#define AVAILABLE_COMMANDS 4 //available commands here used for autocomplete feature


//This one is for storing the user input into the 2D array
#define MAX_COMMAND_LENGTH 100 //length of
#define MAX_COMMANDS 10 //number of command will be stored

//Function prototypes
/*STRING FUNCTION-------------*/
char* strncpy_custom(char* destination, const char* source, int num);//copy by a certain length
int strlen_custom(const char* str);

//Copy whole string
void copyString(char *destination, const char *source);

double getStringSimilarity(const char *str1, const char *str2);


//Split the string into multiple element, separated by whitespace
void splitString(const char* input, char words[MAX_WORDS][MAX_WORD_LENGTH], int* numWords);
int str_cmp(const char *str1, const char *str2);
/*---------------------------*/

//convert integer to character
char intToChar(int num);


//Convert character to integer
int charToInt(char c);

// display the information of the help command
void displayHelpCommandUsage();

//display the information of the showinfo command
void displayShowInfoCommandUsage();


//display the information of the clear command
void displayClearCommandUsage();

//display the information of the setcolor command
void displaySetColorCommandUsage();



//Coloring commands
int setTextColor(char *textColor);

int setBackgroundColor(char *backgroundColor);


//Screen printing function
void printWelcome_Message();




//Function for the command history feature

