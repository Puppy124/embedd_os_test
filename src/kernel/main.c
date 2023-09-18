/*------------------------------------------------------------------------------------
Includes
-------------------------------------------------------------------------------------*/
#include "../mailbox/mbox.h"
#include "../uart/uart.h"
#include "../custom/custom.h"
#include "../framebuffer/framebf.h"
#include "../printf/printf.h"
#include "../framebuffer/picture.h"
#include "../framebuffer/game.h"
#include "../framebuffer/basket.h"
#include "../framebuffer/apple.h"
#include "../framebuffer/knife.h"

/*------------------------------------------------------------------------------------
Defines
-------------------------------------------------------------------------------------*/
#define SCREEN_HEIGHT               (768u)
#define SCREEN_WIDTH                (1024u)
#define DISPLAY_PICTURE_HEIGHT      (1024u)
#define PICTURE_LIMIT               (3u)
#define BASKET_WIDTH                (200u)
#define BASKET_HEIGHT               (200u)

/*------------------------------------------------------------------------------------
Global variables
-------------------------------------------------------------------------------------*/
// Available commands
//  List of available commands
const char *availableCommands[AVAILABLE_COMMANDS] = 
{
    "help", 
    "clear", 
    "setcolor", 
    "showinfo"
};
const char increaseCommand[] = "increase";
const char decreaseCommand[] = "decrease";

int scrollPosition = 0;
int picture_count = 1; 
int start_game = 1;
volatile int key_press = 0;

/*------------------------------------------------------------------------------------
Extern declaration
-------------------------------------------------------------------------------------*/
#ifdef TASK_1
extern const unsigned long picture_1[PICTURE_SIZE];
extern const unsigned long picture_2[PICTURE_SIZE];
extern const unsigned long picture_3[PICTURE_SIZE];
#endif

extern const unsigned long game_background[BACKGROUND_SIZE];
extern const unsigned long game_background2[BACKGROUND_SIZE];
extern const unsigned long basket[BASKET_SIZE];
extern const unsigned long apple[APPLE_SIZE];
extern const unsigned long knife[KNIFE_SIZE];

/*------------------------------------------------------------------------------------
Function declaration
-------------------------------------------------------------------------------------*/
void executeClearScreenCommand();
void masterHandlerFunction(char *userCommandArray);
void resetUserCommandArray(char *commandArray);
void executeClearLineCommand();
void reset2DContainer(char commandContainer[MAX_COMMANDS][MAX_COMMAND_LENGTH]);
void activateMailBoxSetupFunction();
void drawScrollImage(unsigned long *picture_array, int scrollPosition);
void drawImage(unsigned long *picture_array, int x, int y, int targetWidth, int targetHeight, int originalWidth, int originalHeight);
void scaleAndDrawImage(unsigned long *picture_array, int screenWidth, int screenHeight);
unsigned char getUart(void);
void reset_timer(void);

int timer_flag = 0;
int timer_start = 0;
/**
 * GAME FUNCTION
*/
// void playGame();


/*------------------------------------------------------------------------------------
Global functions
-------------------------------------------------------------------------------------*/
void main()
{
    // set up serial console
    uart_init();

    //frame buffer
    framebf_init();

    // Clear the screen first
    executeClearScreenCommand();

    // Print the welcome message
    printWelcome_Message();
    
    // usercommand line value will be store inside the array
    char userCommand[MAX_COMMAND_LENGTH];
    int idx = 0;
    Game_stage current_stage;
    int basketX = 100;  // Adjust the initial X coordinate as needed
    // Calculate the Y coordinate to place the basket at the bottom of the screen
    int basketHeight = 200;  // Replace with the actual height of the basket image
    int basketY = SCREEN_HEIGHT - basketHeight + 50;
    int verticalMoveDistance = 10; // Fall by 20 pixels over the course of one second
    

// Initialize the apple's position
    int appleX = 100; // Initial X-coordinate
    int appleY = 0; // Initial Y-coordinate
    // Initialize the knife's position
    int knifeX = 200; // Initial X-coordinate (adjust as needed)
    int knifeY = 50;  // Initial Y-coordinate (adjust as needed)
    // This one used to store user input commands
    char commandStoreList[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int commandCount = 0;

    int basket_move = 0;

    drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);
    // Reset both array before use
    reset2DContainer(commandStoreList);
    resetUserCommandArray(userCommand);

    //MAIN PROGRAM
    while (1)
    {
        unsigned char c;

   

        // playGame();
        #ifdef TASK_1
        switch (picture_count)
        {
            case 1: 
                drawImage(picture_1, scrollPosition);
                break;
            case 2:
                drawImage(picture_2, scrollPosition);
                break;
            case 3:
                drawImage(picture_3, scrollPosition);
                break;
            default: 
                picture_count = 1;
                break;
        }
        
        #endif

        // set_wait_timer(1, 100000);
        c = getUart();
        // set_wait_timer(0, 100000);
        if (c == '\n') {
            key_press = 1;
            if (current_stage == WELCOME_STAGE) {
                clearFramebuffer();
                current_stage = PLAY_STAGE;
            }
            else {
                clearFramebuffer();
                current_stage = WELCOME_STAGE;
            }
            
        }
        else if (c == 'a') {
            basket_move = 1;
            if(basketX >= 0) {
                basketX -= 20;
            }
            // drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);                   
        }
        else if (c == 'd') {
            basket_move = 1;
            if(basketX <= SCREEN_WIDTH - BASKET_WIDTH) {
                basketX += 20;
            } 
            // drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);           
        }
        // set_wait_timer(1, 1000000);
        // set_wait_timer(1, 1000000);
        // appleY += verticalMoveDistance;
        // drawImage(apple, appleX, appleY, 50, 50, 50, 50);
        // set_wait_timer(0, 1000000);
        // if(appleY >= SCREEN_HEIGHT - 100) {
        //     appleY = 100;
        //     drawScrollImage(game_background2, 0);
        // }
        // set_wait_timer(0, 1000000);
        // if(key_press == 1 || start_game == 1) 
        // {
            // set_wait_timer(1, 1000000);
            switch (current_stage) {
                case WELCOME_STAGE:
                    // if(key_press) {
                    //     drawScrollImage(game_background, 0);
                    //     key_press = 0;
                    // }
                     drawScrollImage(game_background, 0);
                    uart_puts("you entered welcome stage\n");
                    break;
                case PLAY_STAGE:
                    // uart_puts("you entered play stage\n");
                    // set_wait_timer(1, 100000);
                    drawScrollImage(game_background2, 0);
                    // set_wait_timer(0, 100000);
                    // if(basket_move) {
                    //     drawScrollImage(game_background2, 0);
                    //     basket_move = 0;
                    // }
                    // set_wait_timer(1, 100000);
                    appleY += verticalMoveDistance;
                    if(appleY >= SCREEN_HEIGHT - 100) {
                        appleY = 100;
                       
                    }

                    knifeY += 10;
                    if(knifeY >= SCREEN_HEIGHT - 50) {
                        knifeY = 50;
                    }
                    
                    // if (basket_move) {
                    //     drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);
                    //     basket_move = 0;
                    // }

                    // set_wait_timer(1, 500000);
                    drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);
                    // set_wait_timer(0, 500000);

                    // set_wait_timer(1000000);
                    set_wait_timer(1, 1000000);
                    static int result = 0;
                    // if(timer_flag) {
                        result++;
                        if(result >= 10) {
                            result = 0;
                        }
                    uart_sendc(intToChar(result));uart_puts("\n");
                    
                    drawImage(apple, appleX, appleY, 50, 50, 50, 50);
                    // drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);
                    drawImage(knife, knifeX, knifeY, 150, 150, 100, 100);
                    set_wait_timer(0, 1000000);
                    // drawImage(basket, basketX, basketY, BASKET_WIDTH, BASKET_HEIGHT, BASKET_WIDTH, BASKET_HEIGHT);
                    // reset_timer;
                    // timer_flag = 0;

                    // }
                  
                    break;
                default: 
                    current_stage = WELCOME_STAGE;
            }
            // set_wait_timer(0, 1000000);
        //     key_press = 0;
        // }

        // start_game = 0;

        
        // read each char
        

        // If we get the backspace character
        // if (c == '\b')
        // {

        //     // We only allow if the user command index is larger than 0, otherwise it will delete the MYOS>
        //     if (idx == 0){
        //         continue;
        //     }

        //     // Otherwise we allow them to remove the character from the console
        //     idx -= 1;
        //     userCommand[idx] = '\0';

        //     // Go back one character
        //     uart_sendc('\b');
        //     // Make it blank
        //     uart_sendc(' ');

        //     uart_sendc('\b');
        //     continue;
        // }
        
        #ifdef TASK_1
        else if (c == 'a' && picture_count > 1) 
        {
            picture_count -= 1;    
            scrollPosition = 0;
        }

        else if (c == 'd' && picture_count < PICTURE_LIMIT)
        {
            picture_count += 1;
            scrollPosition = 0;
        }
		// Handle key presses
		else if (c == 'w' && scrollPosition > 0) {
			uart_sendc(c);
			scrollPosition -= 10; // Adjust the scroll position as needed
		}

		else if (c == 's' && scrollPosition < SCREEN_HEIGHT) {

			if ((scrollPosition + SCREEN_HEIGHT) <= DISPLAY_PICTURE_HEIGHT) {
				uart_sendc(c);
				scrollPosition += 10; // Adjust the scroll position as needed
			}
		}
        #endif

        // // Once the user type the enter, meaning \n, we will capture that command
        // else if (c == '\n' || c == '\r')
        // {

        //     // Put the null terminator at the end of the array
        //     userCommand[idx] = '\0';


            
        //     // Execute the user command
        //     // Call the master function to handle all the commands
        //     masterHandlerFunction(userCommand);
            
        //     // Clear the user command array to ready for the next command
        //     idx = 0;
        //     resetUserCommandArray(userCommand);

        //     uart_puts("\n");
        //     uart_puts("MYOS>");

        //     continue;
        // }

        // else {
		// 	// Store the user command into the array
		// 	userCommand[idx] = c;
		// 	uart_sendc(c);
		// 	// move to next element
		// 	idx++;
		// }


    }
}

void resetUserCommandArray(char *commandArray){
    int length = strlen_custom(commandArray);
    // Loop through the array and clear the value
    for (int i = 0; i < length; i++){
        commandArray[i] = '\0';
    }
}


// COMMAND NUMBER 3 - CLEAR COMMAND
void executeClearScreenCommand(){
    uart_puts("\033[2J");
    uart_puts("\033[H");
}

void executeClearLineCommand(){
    uart_puts("\033[2K\r");
}

// Take the user command, split the string based on the white space, then we will handling according to
// the first element of the string
void masterHandlerFunction(char *userCommandArray){

    /**
     * TODO: handle the array here
     */
    // Split the string based on the white character and add each command into the 2d array of character
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int numWords = 0;
    splitString(userCommandArray, words, &numWords);

    if (str_cmp(words[0], "clear") && numWords == 1){
        executeClearScreenCommand();
    }

    else if(str_cmp(words[0],"play") && numWords == 1){
        // playGame();
    }

    else{
        uart_puts("\nUnknown command name...");
    }
}

//Reset 2d array
void reset2DContainer(char commandContainer[MAX_COMMANDS][MAX_COMMAND_LENGTH]){

    for (int i = 0; i < MAX_COMMANDS; i++){
        for (int j = 0; j < MAX_COMMAND_LENGTH; j++){
            commandContainer[i][j] = '\0';
        }
    }
}


void activateMailBoxSetupFunction(){

    unsigned int *physize = 0; //pointer to response data
    //setup raspberry screen size

    uart_puts("\n----------------------------------\n");

    //Testing the set physical width
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, &physize, 8, 8, 1920, 1080);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    uart_puts("\nGot Actual Physical Width: ");
    uart_dec(physize[0]);
    uart_puts("\nGot Actual Physical Height: ");
    uart_dec(physize[1]);

    uart_puts("\n----------------------------------\n");


    //Test the set voltage
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETVOLTAGE, &physize, 8, 8, 0x000000002, 3);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    uart_puts("\nGot voltage id: ");
    uart_dec(physize[0]);
    uart_puts("\nValue of voltage: ");
    uart_dec(physize[1]);

    uart_puts("\n----------------------------------\n");

    //Test the set turbo
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETTURBO, &physize, 8, 8, 1);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    uart_puts("\nTurbo id: ");
    uart_dec(physize[0]);
    uart_puts("\nTurbo level value: ");
    uart_dec(physize[1]);

    uart_puts("\n----------------------------------\n");

    //Test the set virtual buffer width height
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETVBUFWH, &physize, 8, 8, 1280, 720);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    uart_puts("\nGot Virtual Buffer Width: ");
    uart_dec(physize[0]);
    uart_puts("\nGot Virtual Buffer Height: ");
    uart_dec(physize[1]);

    uart_puts("\n----------------------------------\n");

    //Test the set pixel order
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPXORDER, &physize, 8, 8, 1);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    uart_puts("\nGot Pixel Order value: ");
    uart_dec(physize[0]);

    uart_puts("\n----------------------------------\n");

    //Print out the MYOS> again
    uart_puts("\nMYOS>");

}

unsigned int uart_isReadByteReady(void){
	return (*AUX_MU_LSR & 0x01);
}

unsigned char getUart(void){
    unsigned char ch = 0;
    if (uart_isReadByteReady())
    	ch = uart_getc();
        // key_press = 1;
    return ch;
}

void drawImage(unsigned long *picture_array, int x, int y, int targetWidth, int targetHeight, int originalWidth, int originalHeight)
{
    int imageWidth = originalWidth;   // Replace with the actual width of your image
    int imageHeight = originalHeight;  // Replace with the actual height of your image


    // Calculate the scaling factors for width and height
    float scaleX = (float)targetWidth / imageWidth;
    float scaleY = (float)targetHeight / imageHeight;

    // Draw the resized image at the specified position (x, y)
    for (int dy = 0; dy < targetHeight; dy++)
    {
        for (int dx = 0; dx < targetWidth; dx++)
        {
            // Calculate the corresponding position in the original image
            int originalX = (int)((float)dx / scaleX);
            int originalY = (int)((float)dy / scaleY);

            unsigned long pixelColor = picture_array[originalX + originalY * imageWidth];
            if ((pixelColor != 0x00000000))
            {
                // Draw the pixel since it's not fully transparent
                drawPixelARGB32(x + dx, y + dy, pixelColor);
            }
        }

    }
}

void drawScrollImage(unsigned long *picture_array, int scrollPosition)
{
    // Clear the screen

    // Calculate the portion of the image to display based on the scroll position
    int startY = scrollPosition;
    int endY = scrollPosition + SCREEN_HEIGHT;
    // if (endY <= 1024) {
    // Draw the portion of the image within the startY and endY bounds
        for (int y = startY; y < endY; y++)
        {
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                // Get the pixel color from your image data and draw it at (x, y)
                unsigned int pixelColor = picture_array[x + y * SCREEN_WIDTH];
                drawPixelARGB32(x, y - scrollPosition, pixelColor);
            }
        }
    // }
}

void scaleAndDrawImage(unsigned long *picture_array, int screenWidth, int screenHeight)
{
    int imageWidth = 612;   // Replace with the actual width of your image
    int imageHeight = 432; // Replace with the actual height of your image

    // Calculate scaling factors for width and height
    float scaleX = (float)screenWidth / imageWidth;
    float scaleY = (float)screenHeight / imageHeight;

    // Use the smaller scaling factor to maintain aspect ratio
    float scale = scaleX < scaleY ? scaleX : scaleY;

    // Calculate the scaled image dimensions
    int scaledWidth = (int)(imageWidth * scale);
    int scaledHeight = (int)(imageHeight * scale);

    // Calculate the position to center the image on the screen
    int xOffset = (screenWidth - scaledWidth) / 2;
    int yOffset = (screenHeight - scaledHeight) / 2;

    // Draw the scaled image
    for (int y = 0; y < scaledHeight; y++)
    {
        for (int x = 0; x < scaledWidth; x++)
        {
            // Calculate the corresponding position in the original image
            int originalX = (int)(x / scale);
            int originalY = (int)(y / scale);

            // Get the pixel color from the original image data and draw it at (x, y)
            unsigned int pixelColor = picture_array[originalX + originalY * imageWidth];
            drawPixelARGB32(x + xOffset, y + yOffset, pixelColor);
        }
    }
}

void clearFramebuffer(void) 
{
    for(int y = 0; y < 768; y++) {
        for(int x = 0; x < 1024; x++) {
            drawPixelARGB32(x, y, 0xFF000000);
        }
    }
}



/* Function to wait for some msec: the program will stop there */
void wait_msec(unsigned int n)
{
    register unsigned long f, t, r, expiredTime;
    // Get the current counter frequency (Hz)
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t));
    // Calculate expire value for counter
    expiredTime = t + ((f / 1000) * n) / 1000;
    do
    {
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));
    } while (r < expiredTime);
}

void set_wait_timer(int set, unsigned int msVal)
{
    static unsigned long expiredTime = 0; // declare static to keep value
    register unsigned long r, f, t;
    if (set)
    { /* SET TIMER */
        // Get the current counter frequency (Hz)

        asm volatile("mrs %0, cntfrq_el0"
                     : "=r"(f));
        // Read the current counter
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(t));
        // Calculate expired time:
        expiredTime = t + ((f / 1000) * msVal) / 1000;

    }

    else
    { /* WAIT FOR TIMER TO EXPIRE */
        do
        {
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(r));
        } while (r < expiredTime);
        expiredTime = 0;
    }
}

void start_timer(unsigned int msVal)
{
    static unsigned long expiredTime = 0; // declare static to keep value
    register unsigned long r, f, t;

        // Get the current counter frequency (Hz)
    if (!timer_start) {
        asm volatile("mrs %0, cntfrq_el0"
                     : "=r"(f));
        // Read the current counter
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(t));
        // Calculate expired time:
        expiredTime = t + ((f / 1000) * msVal) / 1000;
        timer_start = 1;
    }

    if (!timer_flag)
    {
        asm volatile("mrs %0, cntpct_el0"
                    : "=r"(r));
    }

    if(r >= expiredTime) {
        timer_flag = 1;
        timer_start = 0;
        expiredTime = 0;
    }
}

void reset_timer(void) {
    timer_flag = 0;
    timer_start = 0;
}

int gameArray[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

/**
 * GAME LOGIC
 */
void playGame()
{
    uart_puts("\n User wants to play the game\n");

    int gameStopped = 0;

    // Begin the loop
    while (gameStopped != 10)
    {
        // set timer with 100ms
        set_wait_timer(1,1000000);
        // go doing other tasks (which must take less than 100ms)
        // ..........
        uart_sendc(intToChar(gameStopped));uart_puts("\n");
        gameStopped++;
        set_wait_timer(0,1000000);
    }
}
