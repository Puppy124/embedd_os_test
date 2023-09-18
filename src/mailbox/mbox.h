// -----------------------------------mbox.h -------------------------------------

#include "../kernel/gpio.h"


//Library for part 3, due to unlimited argument
#include "../../gcclib/stddef.h"
#include "../../gcclib/stdint.h"
#include "../../gcclib/stdarg.h"

/* a properly aligned buffer */
extern volatile unsigned int mBuf[36];
#define ADDR(X) (unsigned int)((unsigned long) X)

/* Registers */
#define VIDEOCORE_MBOX (MMIO_BASE + 0x0000B880)
#define MBOX0_READ ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x00))
#define MBOX0_PEEK ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x10))
#define MBOX0_SENDER ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x14))
#define MBOX0_STATUS ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x18))
#define MBOX0_CONFIG ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x1C))
#define MBOX1_WRITE ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x20))
#define MBOX1_PEEK ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x30))
#define MBOX1_SENDER ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x34))
#define MBOX1_STATUS ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x38))
#define MBOX1_CONFIG ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x3C))


//Request/Response code in Buffer content
#define MBOX_RESPONSE	0x80000000
#define MBOX_REQUEST 	0

//Status Value (from Status Register)
#define MBOX_FULL		0x80000000
#define MBOX_EMPTY		0x40000000


/* channels */
#define MBOX_CH_POWER   0	//Power management
#define MBOX_CH_FB      1	//Frame buffer
#define MBOX_CH_VUART   2	//Virtual UART
#define MBOX_CH_VCHIQ   3	//VCHIQ
#define MBOX_CH_LEDS    4	//LEDs
#define MBOX_CH_BTNS    5	//Buttons
#define MBOX_CH_TOUCH   6	//Touch screen
#define MBOX_CH_PROP    8	//Property tags (ARM -> VC)


/* tags */
#define MBOX_TAG_GETSERIAL      0x00010004	//Get board serial
#define MBOX_TAG_GETMODEL       0x00010001	//Get board serial
#define MBOX_TAG_SETCLKRATE     0x00038002
#define MBOX_TAG_LAST           0

/*set physical width tag*/
#define MBOX_TAG_SETPHYWH       0x00048003 //set physical width and height
#define MBOX_TAG_SETVOLTAGE     0x00038003 //set voltage
#define MBOX_TAG_SETTURBO       0x00038009 // set turbo
#define MBOX_TAG_SETVBUFWH      0x00048004 // set virtual buffer width and height
#define MBOX_TAG_SETPXORDER     0x00048006 // set pixel order

/* Function Prototypes */
int mbox_call(unsigned int buffer_addr, unsigned char channel);


void mbox_buffer_setup(unsigned int buffer_addr, unsigned int tag_identifier, unsigned int **res_data, unsigned int res_length, unsigned int req_length, ...);
