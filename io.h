/* io.h: stores IO stuff */
#pragma once
#include "chip8.h"

int io_init (
  struct chip8 *chip8,
  const  char *title
);

int io (
  struct chip8 *chip8
);

