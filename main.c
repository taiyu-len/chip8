/* main.c */
#include <stdio.h>

#include "chip8.h"
#include "exec.h"
#include "io.h"

int main(int argc, char **argv)
{
  if(argc == 1) goto USAGE;

  //Load chip
  struct chip8      chip8;
  chip8_init      (&chip8);
  if(chip8_loadrom(&chip8, argv[1])) goto USAGE;

  //Initilize IO
  io_init(&chip8, argv[1]);

  //Run main loop
  for(;;)
  {
    exec(&chip8);
    io  (&chip8);
  }
  return 0;
USAGE:
  printf("Usage: %s Romname\n",argv[0]);
  return 1;
}

