// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"
#include "../kernel/gpio.h"
#include "../uart/uart.h"

/* Mailbox Data Buffer (each element is 32-bit)*/
/*
 * The keyword attribute allows you to specify special attributes
 *
 * The aligned(N) attribute aligns the current data item on an address
 * which is a multiple of N, by inserting padding bytes before the data item
 *
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
 *
 *
 * We must ensure that our our buffer is located at a 16 byte aligned address,
 * so only the high 28 bits contain the address
 * (last 4 bits is ZERO due to 16 byte alignment)
 *
*/

volatile unsigned int  __attribute__((aligned(16))) mBuf[36];


/**
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
	//Receiving message is buffer_addr & channel number
	uint32_t res;

	// Make sure that the message is from the right channel
	do {
		// Make sure there is mail to receive
		do{
			asm volatile("nop");
		}while(*MBOX0_STATUS & MBOX_EMPTY);

		// Get the message
		res = *MBOX0_READ;
	} while ( (res & 0xF) != channel);

	return res;
}


/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
	//Sending message is buffer_addr & channel number

	// Make sure you can send mail
	do{
		asm volatile("nop");
	}while(*MBOX1_STATUS & MBOX_FULL);

	// send the message
	*MBOX1_WRITE = msg;
}



/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
	//Check Buffer Address
	//uart_puts("\nBuffer Address: ");
	// uart_hex(buffer_addr);
	// uart_sendc('\n');
	
	//Prepare Data (address of Message Buffer)
	unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
	mailbox_send(msg, channel);

	/* now wait for the response */
	/* is it a response to our message (same address)? */
	if (msg == mailbox_read(channel)) {
		/* is it a valid successful response (Response Code) ? */
		if (mBuf[1] == MBOX_RESPONSE)
			//uart_puts("Got successful response \n");
	
		return (mBuf[1] == MBOX_RESPONSE);
	}

    return 0;
}



//MAILBOX SETUP FUNCTION
void mbox_buffer_setup(unsigned int buffer_addr, unsigned int tag_identifier,unsigned int **res_data, unsigned int res_length,unsigned int req_length, ...) {


	//Based on 5 tags: we will handle them accordingly
	//Check the tag_identifiers

	int numberOfResponseValues = 0;

	/**
	 * TODO: Handling the set physical width and height
	*/
	if(tag_identifier == MBOX_TAG_SETPHYWH){
		uart_puts("\nUser wants to setup physical width and height\n");

		numberOfResponseValues = 2;

		// Set up the request message
    	mBuf[0] = 8 * 4;       // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    	mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    	mBuf[2] = tag_identifier;   // TAG Identifier: set the physical screen width or height
    	mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
    	mBuf[4] = 0;            // REQUEST CODE = 0
		//Now we need to get the values of width and height from the va_arg list
		va_list ap;
		va_start(ap,numberOfResponseValues);
		//Get the width first then height
		int width = 0;
		int height = 0;

		//Based on the number of values in the response, we will have to loop through accordingly, with set physical height/width tag, we only have
		//2 values in response
		for(int i = 0 ; i < 2 ; i++){
			if(i == 0){
				width = va_arg(ap,int);
			}

			else if(i == 1){
				height = va_arg(ap,int);
			}
		}
    	mBuf[5] = width;            // u32 width in pixels
		mBuf[6] = height;			// u32 height in pixels
		mBuf[7] = MBOX_TAG_LAST; //END tag

		//Add the value to the response data
		*res_data[0] = height;
		*res_data[1] = width;

		//Return to avoid impacting other code down below
		return;
	}

	/**
	 * TODO: Handling the setting voltage
	*/
	else if(tag_identifier == MBOX_TAG_SETVOLTAGE){
		// Unique voltage IDs:
		// 0x000000000: reserved
		// 0x000000001: Core
		// 0x000000002: SDRAM_C
		// 0x000000003: SDRAM_P
		// 0x000000004: SDRAM_I
		uart_puts("\nUser wants to setup the voltage\n");
		mBuf[0] = 8 * 4;       // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    	mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    	mBuf[2] = tag_identifier;   // TAG Identifier: set the physical screen width or height
    	mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
    	mBuf[4] = 0;            // REQUEST CODE = 0

		//Now we need to get the values of width and height from the va_arg list
		va_list ap;
		va_start(ap,numberOfResponseValues);
		//Get the width first then height
		int voltageId = 0;
		int voltageValue = 0;

		//Based on the number of values in the response, we will have to loop through accordingly, with set physical height/width tag, we only have
		//2 values in response
		for(int i = 0 ; i < 2 ; i++){
			if(i == 0){
				voltageId = va_arg(ap,int);
			}

			else if(i == 1){
				voltageValue = va_arg(ap,int);
			}
		}
    	mBuf[5] = voltageId;  // Voltage id
		mBuf[6] = voltageValue; //value
		mBuf[7] = MBOX_TAG_LAST; //END tag

		//Add the value to the response data
		*res_data[0] = voltageId;
		*res_data[1] = voltageValue;

		//Return to avoid impacting other code down below
		return;
	}

	/**
	 * TODO: Handling the setting turbo
	*/
	else if(tag_identifier == MBOX_TAG_SETTURBO){
		uart_puts("\nUser wants to setup the turbo\n");
		mBuf[0] = 8 * 4;       // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    	mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    	mBuf[2] = tag_identifier;   // TAG Identifier: set the physical screen width or height
    	mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
    	mBuf[4] = 0;            // REQUEST CODE = 0

		//Get the value for level
		va_list ap;
		va_start(ap,numberOfResponseValues);
		int level = va_arg(ap,int);

    	mBuf[5] = 0;            // u32 id Set the turbo state for index id. id should be zero
		mBuf[6] = level;            // u32 level level will be zero for non-turbo and one for turbo.
		mBuf[7] = MBOX_TAG_LAST; //END tag

		//Add the value to the response data
		*res_data[0] = 0;
		*res_data[1] = level;

		//Return to avoid impacting other code down below
		return;
	}

	/**
	 * TODO: Handling the setting virtual buffer width/height
	*/
	else if(tag_identifier == MBOX_TAG_SETVBUFWH){
		uart_puts("\nUser wants to setup the virtual buffer\n");
		// Set up the request message
    	mBuf[0] = 8 * 4;       // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    	mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    	mBuf[2] = tag_identifier;   // TAG Identifier: set the physical screen width or height
    	mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
    	mBuf[4] = 0;            // REQUEST CODE = 0
		//Now we need to get the values of width and height from the va_arg list
		va_list ap;
		va_start(ap,numberOfResponseValues);
		//Get the width first then height
		int width = 0;
		int height = 0;

		//Based on the number of values in the response, we will have to loop through accordingly, with set physical height/width tag, we only have
		//2 values in response
		for(int i = 0 ; i < 2 ; i++){
			if(i == 0){
				width = va_arg(ap,int);
			}

			else if(i == 1){
				height = va_arg(ap,int);
			}
		}
    	mBuf[5] = width;            // u32 width in pixels
		mBuf[6] = height;			// u32 height in pixels
		mBuf[7] = MBOX_TAG_LAST; //END tag

		//Add the value to the response data
		*res_data[0] = width;
		*res_data[1] = height;

		//Return to avoid impacting other code down below
		return;
	}

	/**
	 * TODO: Handling the set pixel order
	*/
	else if(tag_identifier == MBOX_TAG_SETPXORDER){
		uart_puts("\nUser wants to setup the pixel order\n");
		mBuf[0] = 8 * 4;       // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    	mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    	mBuf[2] = tag_identifier;   // TAG Identifier: set the physical screen width or height
    	mBuf[3] = 4;            // Value buffer size in bytes (max of request and response lengths)
    	mBuf[4] = 0;            // REQUEST CODE = 0

		//Get the value for level
		va_list ap;
		va_start(ap,numberOfResponseValues);
		int value = va_arg(ap,int);
    	mBuf[5] = value;            // u32: state (as above)
		mBuf[6] = MBOX_TAG_LAST;        
	
		//Add the value to the response data
		*res_data[0] = value;

		//Return to avoid impacting other code down below
		return;
	}




	


}

