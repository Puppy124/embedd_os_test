
#ifndef FRAMEBF_H
#define FRAMEBF_H

/*------------------------------------------------------------------------------------
Defines
-------------------------------------------------------------------------------------*/
//New Tags for Screen Display
#define MBOX_TAG_SETPHYWH           (0x48003)
#define MBOX_TAG_SETVIRTWH          (0x48004)
#define MBOX_TAG_SETVIRTOFF         (0x48009)
#define MBOX_TAG_SETDEPTH           (0x48005)
#define MBOX_TAG_SETPXLORDR         (0x48006)
#define MBOX_TAG_GETFB              (0x40001)
#define MBOX_TAG_GETPITCH           (0x40008)

/*------------------------------------------------------------------------------------
Typedef
-------------------------------------------------------------------------------------*/
typedef enum 
{
   NO_FILL,
   APPLY_FILL  
} Fill_option;

/*------------------------------------------------------------------------------------
Function declaration
-------------------------------------------------------------------------------------*/
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, Fill_option fill);
void drawLine(int x1, int y1,int x2,int y2, unsigned int attr);

#endif