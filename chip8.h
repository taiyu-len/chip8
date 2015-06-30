/* chip8.h */
#pragma once

enum
{
  RAMSIZE   = 4096,
  VSIZE     = 16,
  STACKSIZE = 48,
  GFXWIDTH  = 64,
  GFXHEIGHT = 32
};


struct chip8
{
  //Memory
  unsigned char memory[ RAMSIZE ];

  //Registers
  unsigned char  V[ VSIZE ];
  unsigned short I, pc;

  //Stack
  unsigned short stack[ STACKSIZE ],
                 sp;
  //Timers:
  unsigned char delay_timer,
                sound_timer;

  //Input : each bit represents a key
  unsigned short key;

  //Waiting for input if true
  unsigned char  waiting;

  //Graphics : Each bit represents a pixel
  unsigned char graphics[ GFXWIDTH * GFXHEIGHT / 8 ];
  unsigned char x, y, n, clear;
};

//Initilize a chip8 structure. returns 0
int chip8_init   (struct chip8 *chip8);

//Load a rom into memory. retusn 0 on success, 1 on failure
int chip8_loadrom(struct chip8 *chip8, const char *filename);

