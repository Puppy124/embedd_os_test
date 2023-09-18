#include "custom.h"
#include "../uart/uart.h"

//Function to copy a specified string to a certain string
/**
 * 
 * @param char *destination: a string to be copied to
 * @param const char *source: the string that the char *destination string will have the same value to
 * @param int num: starting index
*/
char* strncpy_custom(char* destination, const char* source, int num) {
    char* dest = destination;
    const char* src = source;

    //Copy each character starting from the specified index,=
    while (num > 0 && *src != '\0') {
        *dest = *src;
        //Move to next element
        dest++;
        src++;

        //Calcualte how many characters left needs to be copied
        num--;
    }

    //Add null terminator at the end of the destination string
    while (num > 0) {
        *dest = '\0';
        dest++;
        num--;
    }

    return destination;
}


//Function to return the length of the string, or total character in the array
int strlen_custom(const char* str) {
    const char* ptr = str;
    
    while (*ptr != '\0') {
        ptr++;
    }
    
    return (int)(ptr - str);
}


//Function to compare string, if 2 strings is similar, we return 1, otherwise return 0
int str_cmp(const char *str1, const char *str2){
    // Quit if the either string is null
    if (*str1 == '\0' || *str2 == '\0')
    {
        return 0;
    }

    //length is not equal
    if (strlen_custom(str1) != strlen_custom(str2))
    {
        return 0;
    }

    //Check for each character
    for (int i = 0; (i < str1[i]) != '\0' && str2[i] != '\0'; i++)
    {
        if (str1[i] != str2[i])
        {
            return 0;
        }
    }

    return 1;
}

//Function to split string based on the whitespace
/**
 * @param input: this is the array contains user input
 * @param words: this is the 2d array used to contain user input that has been splited by whitespace
 * @param numWords: this is the number of words that the user has typed in
 * @return void
*/
//Reference function: https://www.w3resource.com/c-programming-exercises/string/c-string-exercise-31.php
void splitString(const char* input, char words[MAX_WORDS][MAX_WORD_LENGTH], int* numWords) {

    //Word counter
    int wordCount = 0;

    //Starting index to begin copy
    int wordStart = -1;

    //Loop through each character of the user command array
    for (int i = 0; input[i] != '\0'; i++) {

        //If the element is the character
        if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n') {

            //Get the index, because we are counting how many whitespace, then we will split the string 
            //according to the whitespace
            if (wordStart == -1) {
                wordStart = i;
            }

        } 
        
        //If we meet a whitespace
        else {
            if (wordStart != -1) {

                //Calculate the length of the word before this whitespace
                int wordLength = i - wordStart;

                //Limit the amount of character to copy
                if (wordLength > MAX_WORD_LENGTH - 1) {
                    wordLength = MAX_WORD_LENGTH - 1;
                }

                //Copy the user command into the 2d array
                strncpy_custom(words[wordCount], &input[wordStart], wordLength);

                //Put the null terminator at the end
                words[wordCount][wordLength] = '\0';

                //increment to a new counter
                wordCount++;

                //Restart the index
                wordStart = -1;

                //If we ran out of space for the 2d array, we stop
                if (wordCount >= MAX_WORDS) {
                    break;
                }
            }
        }
    }

    //Add the last word of the user command array into the 2d array container
    if (wordStart != -1) {
        int wordLength = strlen_custom(&input[wordStart]);
        if (wordLength > MAX_WORD_LENGTH - 1) {
            wordLength = MAX_WORD_LENGTH - 1;
        }
        //Copy the string to the last element of the 2d array
        strncpy_custom(words[wordCount], &input[wordStart], wordLength);
        //Add the null terminator
        words[wordCount][wordLength] = '\0';
        wordCount++;
    }

    //Update the number of words pointer
    *numWords = wordCount;
}


//function convert integer to character
char intToChar(int num) {
    switch (num){

        case 0:
        return '0';

        case 1:
        return '1';

        case 2:
        return '2';

        case 3:
        return '3';

        case 4:
        return '4';

        case 5:
        return '5';

        case 6:
        return '6';

        case 7:
        return '7';

        case 8:
        return '8';

        case 9:
        return '9';
        
        default:
        return '\0';

    }
}


//function convert character to integer
int charToInt(char c) {
    switch (c){

        case '0':
        return 0;

        case '1':
        return 1;

        case '2':
        return 2;

        case '3':
        return 3;

        case '4':
        return 4;

        case '5':
        return 5;

        case '6':
        return 6;

        case '7':
        return 7;

        case '8':
        return 8;

        case '9':
        return 9;
        
        default:
        return '\0';

    }
}


// display the information of the help command
void displayHelpCommandUsage(){
    uart_puts("\n\nCommand Name: help");
    uart_puts("\nUsage: Show brief information of all commands\n");
    uart_puts("Example: MyOS> help\n");
}

//display the information of the showinfo command
void displayShowInfoCommandUsage(){
    uart_puts("\n\nCommand Name: showinfo");
    uart_puts("\nUsage: Show board revision and board MAC address in correct format/ meaningful information\n");
    uart_puts("Example: MyOS> showinfo\n");
}


//display the information of the clear command
void displayClearCommandUsage(){
    uart_puts("\n\nCommand Name: clear");
    uart_puts("\nUsage: Clear screen (in our terminal it will scroll down to current position of the cursor)\n");
    uart_puts("Examples: MyOS> clear");
}

//display the information of the setcolor command
void displaySetColorCommandUsage(){
    uart_puts("\n\nCommand Name: setcolor\n");
    uart_puts("\nSyntax: setcolor -t/-b <text color> -t/-b <background color>\n");
    uart_puts("\nUsage: Set text color, and/or background color of the console to one \nof the following colors: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
    uart_puts("\nExamples: MyOS> setcolor -t yellow \n\tMyOS> setcolor -b yellow -t white\n");
}


int setTextColor(char *textColor){
    if(str_cmp(textColor,"black") == 1){
        uart_puts("\x1b[30m");
    }

    else if(str_cmp(textColor,"red") == 1){
        uart_puts("\x1b[31m");
    }

    else if(str_cmp(textColor,"green") == 1){
        uart_puts("\x1b[32m");
    }

    else if(str_cmp(textColor,"yellow") == 1){
        uart_puts("\x1b[33m");
    }
    
    else if(str_cmp(textColor,"blue") == 1){
        uart_puts("\x1b[34m");
    }

    else if(str_cmp(textColor,"purple") == 1){
        uart_puts("\x1b[35m");
    }

    else if(str_cmp(textColor,"cyan") == 1){
        uart_puts("\x1b[36m");
    }

    else if(str_cmp(textColor,"white") == 1){
        uart_puts("\x1b[37m");
    }
    else {
        uart_puts("Unknown color\n");
        return 0; //Incorrect case return 0
    }

    //Matching color case return 1 as success
    return 1;
}

int setBackgroundColor(char *backgroundColor){
    if(str_cmp(backgroundColor,"black")== 1){
        uart_puts("\x1b[40m");
    }

    else if(str_cmp(backgroundColor,"red")== 1){
        uart_puts("\x1b[41m");
    }

    else if(str_cmp(backgroundColor,"green")== 1){
        uart_puts("\x1b[42m");
    }

    else if(str_cmp(backgroundColor,"yellow")== 1){
        uart_puts("\x1b[43m");
    }
    
    else if(str_cmp(backgroundColor,"blue")== 1){
        uart_puts("\x1b[44m");
    }

    else if(str_cmp(backgroundColor,"purple")== 1){
        uart_puts("\x1b[45m");
    }

    else if(str_cmp(backgroundColor,"cyan")== 1){
        uart_puts("\x1b[46m");
    }

    else if(str_cmp(backgroundColor,"white")== 1){
        uart_puts("\x1b[47m");
    }
    else {
        uart_puts("Unknown color\n");
        return 0; //Incorrect case return 0
    }

    return 1;//Matching color case return 1 as success
}

// Function to print out the hello message
void printWelcome_Message(){
    uart_puts("### ###  ### ###  ### ###  #### ##  ## ##        ##    ## ##    ## ##  ");
    uart_puts("\n");
    uart_puts(" ##  ##   ##  ##   ##  ##  # ## ##  ##  ##     # ##    #   ##  ##   ##");
    uart_puts("\n");
    uart_puts(" ##       ##       ##        ##         ##    ## ##   ##   ##  ##   ## ");
    uart_puts("\n");
    uart_puts(" ## ##    ## ##    ## ##     ##        ##    ##  ##    ## ###  ##   ## ");
    uart_puts("\n");
    uart_puts(" ##       ##       ##        ##       ##     ### ###       ##  ##   ##");
    uart_puts("\n");
    uart_puts(" ##  ##   ##  ##   ##  ##    ##      #   ##      ##   ##   ##  ##   ##");
    uart_puts("\n");
    uart_puts("### ###  ### ###  ### ###   ####    ######       ##    ## ##    ## ##");
    uart_puts("\n");
    uart_puts("                                                                     ");
    uart_puts("\n");
    uart_puts("### ##     ##     ### ##   ### ###            ## ##    ## ## ");
    uart_puts("\n");
    uart_puts("##  ##     ##     ##  ##   ##  ##           ##   ##  ##   ##");
    uart_puts("\n");
    uart_puts("##  ##   ## ##    ##  ##   ##               ##   ##  ####   ");
    uart_puts("\n");
    uart_puts("## ##    ##  ##   ## ##    ## ##            ##   ##   ##### ");
    uart_puts("\n");
    uart_puts("##  ##   ## ###   ## ##    ##               ##   ##      ###");
    uart_puts("\n");
    uart_puts("##  ##   ##  ##   ##  ##   ##  ##           ##   ##  ##   ##");
    uart_puts("\n");
    uart_puts("### ##   ###  ##  #### ##  ### ###            ## ##    ## ##");
    uart_puts("\n");

    uart_puts("\nDeveloped by VO DAI DUONG - S3821186\n");

    uart_puts("\n\nMYOS>");
}

//Function to return the point of similarity between 2 strings
double getStringSimilarity(const char *str1, const char *str2) {
    int len1 = strlen_custom(str1);
    int len2 = strlen_custom(str2);

    //Get the smaller length
    int commonLength = len1 > len2 ? len2 : len1;

    
    int matchingChars = 0;

    //Loop through each character
    for (int i = 0; i < commonLength; i++) {
        if (str1[i] == str2[i]) {
            matchingChars++;
        }
    }

    return (double) matchingChars / commonLength * 100.0;
}


//Copy whole string
void copyString(char *destination, const char *source) {
    // Iterate through the source array and copy each character
    int i = 0;
    while(1){

        //Reaching null terminator means the end
        if(source[i] == '\0'){
            //We put null terminator for the string to be copied to
            destination[i] = '\0';
            break;
        }
        else {
            destination[i] = source[i];
            i++;
        }
    }
}