#include "./printf.h"
#include "../uart/uart.h"

#define MAX_PRINT_SIZE 256


//Reference: https://www.geeksforgeeks.org/c-program-to-print-all-digits-of-a-given-number/
void printInteger(int num, char *buffer, int *buffer_index) {
    char temp_buffer[MAX_PRINT_SIZE];
    int temp_index = MAX_PRINT_SIZE - 1;

    //Here we are copying the integer, for example 100 into the another array
    //Using the % operator divide for 10 and convert them into char
    //When we divide 100 % 10 bit by bit, the result will be something lile '0','0','1'. Hence it is reversed,
    do {
        //For example the integer is 100
        //Store the first element to the buffer, at temp_buffer[255], hence temp_buffer[255] = 0
        temp_buffer[temp_index] = (num % 10) + '0';

        //Decrement one index
        temp_index--;

        //Keep doing this and we will get temp_buffer[254] = 0, temp_buffer[253] = 1. And we just need to loop through the temp_buffer from 0 to max index, we will get the correct number

        // Update num to num/10 to extract next last digit
        num /= 10;
    } while (num != 0);

    //Store each digit from the temp_buffer to the main buffer
    for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
        buffer[*buffer_index] = temp_buffer[i];
        (*buffer_index)++;
    }
}


//Printing hexadecimal is similar to printInteger, but with the use of many character from 0 to 9..A to F
void printHexadecimal(int num, char *buffer, int *buffer_index) {
    char hex_digits[] = "0123456789ABCDEF";
    char temp_buffer[MAX_PRINT_SIZE];
    int temp_index = MAX_PRINT_SIZE - 1;

    //Repeat the same printing of each digit like printing the integer
    do {
        temp_buffer[temp_index] = hex_digits[num % 16];
        temp_index--;
        num /= 16;
    } while (num != 0);

    for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
        buffer[*buffer_index] = temp_buffer[i];
        (*buffer_index)++;
    }
}

//Printing a floating points values, we have to print the integer part of it first, then we can print the floating point values
void printFloat(double num, char *buffer, int *buffer_index) {

    //Get the integer part by converting a float to the int
    int intPart = (int) num;

    //Get the floating point parts/ fractional parts
    double fracPart = num - intPart;

    //First we need to store all the integer part of the float
    printInteger(intPart, buffer, buffer_index);

    /**
     * TODO: Handle fractional parts
    */
    //And we add a dot to indicate moving to the floating point values
    buffer[*buffer_index] = '.';

    //Move the index to the first fractional/floating values
    (*buffer_index)++;

    //store 6 floating point values to the buffer, the process is similar to storing integer
    for (int i = 0; i < 6; i++) {
        fracPart *= 10;
        int digit = (int) fracPart;
        buffer[*buffer_index] = digit + '0';
        (*buffer_index)++;
        fracPart -= digit;
    }
}

void printf(char *string, ...) {

    //argument list
    va_list ap;
    //initialize the list
    va_start(ap, string);

    char buffer[MAX_PRINT_SIZE];
    int buffer_index = 0;

    //Loop through the user string
    while (*string != '\0') {

        //If we meet a format specifier
        if (*string == '%') {
            //move to the next character to check for the d,s, or x or f
            string++;

            //d
            if (*string == 'd') {
                //Get the integer from the list
                int x = va_arg(ap, int);

                //Print the integer
                printInteger(x, buffer, &buffer_index);

                //Move to next character in the string
                string++;
            } 

            //c
            else if (*string == 'c') {
                
                //Get the character from the list
                char c = (char) va_arg(ap, int);

                //Put them into the buffer array
                buffer[buffer_index] = c;
                //Move to next index ready for another allocation
                buffer_index++;

                
                string++;
            } 
            //s
            else if (*string == 's') {

                //Get the string from the list
                char *str = va_arg(ap, char *);

                //Iterate through the string
                while (*str != '\0') {

                    //Then we store each element of the string to the buffer
                    buffer[buffer_index] = *str;
                    buffer_index++;
                    str++;
                }

                //Move to next character in the string
                string++;
            } 
            
            //x
            else if (*string == 'x') {

                int x = va_arg(ap, int);
                printHexadecimal(x, buffer, &buffer_index);

                //Move to next character in the string
                string++;
            } 
            //f
            else if (*string == 'f') {
                
                double num = va_arg(ap, double);
                printFloat(num, buffer, &buffer_index);

                //Move to next character in the string
                string++;
            }

        } 
        
        //If we dont see any %d, %s, %c, %f, %x, we just keep putting the new character to the buffer
        else {
            buffer[buffer_index] = *string;
            buffer_index++;
            
            //Move to next character in the string
            string++;
        }

        if (buffer_index == MAX_PRINT_SIZE - 1)
            break;
    }

    
    va_end(ap);

    
    uart_puts(buffer);

    
    uart_puts("MYOS>");
}
