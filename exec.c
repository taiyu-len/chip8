/* exec.c */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "exec.h"


void exec(struct chip8 *chip8)
{
  //Get current opcode
  unsigned short opcode
    = chip8->memory[chip8->pc] << 8
    | chip8->memory[chip8->pc + 1];

  //Load important values
  unsigned int X   = ((opcode >> 8) & 0x000F),
               Y   = ((opcode >> 4) & 0x000F),
               N   = (opcode & 0x000F),
               NN  = (opcode & 0x00FF),
               NNN = (opcode & 0x0FFF);
#define VX  chip8->V[((opcode >> 8) & 0x000F)]
#define VY  chip8->V[((opcode >> 4) & 0x000F)]
#define V0  chip8->V[0x0]
#define VF  chip8->V[0xF]
#define LOG(x) if(0)puts(x)

  if(1){
    static int counter = 0;
    if(!counter--)
    {
      printf("━━━┯━━━━━━┯━━━┯━━┯━━┯━━┯━━┯━━┯━━┯━━┯━━┓\n"
             "PC │OPCODE│ I │SP│V0│V1│V2│VF│DT│ST├──┚\n");
      counter = 20;
    }
    printf(  "%3x│ %4x │%3x│%2x│%2x│%2x│%2x│%2x│%2i│%2i│\n",
        chip8->pc,
        opcode,
        chip8->I,
        chip8->sp,
        V0,
        chip8->V[1],
        chip8->V[2],
        VF,
        chip8->delay_timer,
        chip8->sound_timer);
  }


  //Increment program counter
  chip8->pc += 2;

  //Get N from NXXX opcode
  switch(opcode >> 12)
  {
    case 0x0:LOG("00E0: Clear screen, 00EE: Return from subroutine");
      {
        //00E0: Clears the screen
        if((opcode & 0x00FF) == 0x00E0)
        {
          memset(chip8->graphics, 0, sizeof(chip8->graphics));
          chip8->clear = 1;
        }
        //00EE: Retuns from subroutine
        else if((opcode & 0x00FF) == 0x00EE)
        {
          --chip8->sp;
          chip8->pc = chip8->stack[chip8->sp];
        }
        break;
      }
    case 0x1:LOG("1NNN: Jumps to address NNN");
      {
        chip8->pc = NNN;
        break;
      }
    case 0x2:LOG("2NNN: Call subroutine at NNN");
      {
        chip8->stack[chip8->sp] = chip8->pc;
        ++chip8->sp;
        chip8->pc = NNN;
        break;
      }
    case 0x3:LOG("3XNN: Skips the next instruction if VX == NN");
      {
        chip8->pc += (VX == NN)*2;
        break;
      }
    case 0x4:LOG("4XNN: Skips the next instruction if VX != NN");
      {
        chip8->pc += (VX != NN)*2;
        break;
      }
    case 0x5:LOG("5XY0: Skips The next instruction if VX == VY");
      {
        chip8->pc += (VX == VY)*2;
        break;
      }
    case 0x6:LOG("6XNN: Sets V[X] = NN");
      {
        VX = NN;
        break;
      }
    case 0x7:LOG("7XNN: Adds NN to V[X]");
      {
        VX += NN;
        break;
      }
    case 0x8:LOG("8XYN: Bitwise manipulations");
      {
        switch(opcode & 0x000F)
        {
          case 0x0:LOG("8XY0: Sets VX to the value of VY");
            VX = VY;
            break;
          case 0x1:LOG("8XY1: Sets VX to VX | VY");
            VX |= VY;
            break;
          case 0x2:LOG("8XY2: Sets VX to VX & VY");
            VX &= VY;
            break;
          case 0x3:LOG("8XY3: Sets VX to VX ^ VY");
            VX ^= VY;
            break;
          case 0x4:LOG("8XY4: Adds VY to VX. VF is set to 1 when there is a carry 0 when there isnt");
            VF = (VX + VY > 255);
            VX += VY;
            break;
          case 0x5:LOG("8XY5: Vy is subtracted from VX. VF is set to 0 when theres a borrow, 1 when there isnt");
            VF = (VX - VY < 0);
            VX -= VY;
            break;
          case 0x6:LOG("8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift");
            VF = VX & 1;
            VX >>= 1;
            break;
          case 0x7:LOG("8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.");
            VF = (VY - VX < 0);
            VX = VY - VX;
            break;
          case 0xE:LOG("8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift");
            VF = (VX >> 7);
            VX <<= 1;
            break;
          default: goto invalid;
        }
        break;
      }
    case 0x9:LOG("9XY0: Skips the next instruction if VX != VY");
      {
        chip8->pc += (VX != VY)*2;
        break;
      }
    case 0xa:LOG("ANNN: Sets I to address NNN");
      {
        chip8->I = NNN;
        break;
      }
    case 0xb:LOG("BNNN: Jumps to address NNN + V0");
      {
        chip8->pc = NNN + V0;
        break;
      }
    case 0xc:LOG("CXNN: Sets VX to a random number masked by NN");
      {
        VX = rand() & NN;
        break;
      }
    case 0xd:LOG("DXYN: Draws a sprite at cooridinate (VX,VY) with Height N");
             //from memory address I to I+N. uses XOR to draw pixels.
             //If pixels are cleared. set VF to 1. otherwise 0
      {
        VF = 0;
        for(unsigned int row = 0; row < N; ++row)
        {
          //Get sprite value
          unsigned char
            sprite = chip8->memory[chip8->I + row],
          //Get written values
            write[2] = {
              (unsigned char)(sprite >> (VX % 8)),     //===10101
              (unsigned char)(sprite << (8 - (VX % 8)))//        010=====
            };
          unsigned int idx = VX/8 + (VY+row)*GFXWIDTH / 8;
          //Check for collisions, that is any bit that will be cleared
          VF |= (chip8->graphics[idx] & write[0]
                 || chip8->graphics[idx+1] & write[1]);
          //XOR values onto screen
          chip8->graphics[idx] ^= write[0];
          chip8->graphics[idx+1] ^= write[1];
        }
        //Remember drawing coordinates for drawing the output
        chip8->x = VX;
        chip8->y = VY;
        chip8->n = N;
        break;
      }
    case 0xe:LOG("EX9E, EXA1: keypresses");
      {
        if(VX > 15) goto invalid;
        if(NN == 0x9E) //Skip if key VX is pressed
        {
          if(chip8->key & (1 << VX)) chip8->pc += 2;
        }
        else if(NN == 0xA1) //Skip if key VX is not pressed
        {
          if(!(chip8->key & (1 << VX))) chip8->pc += 2;
        }
        else goto invalid;
        break;
      }
    case 0xf:LOG("FX07, FX0A, FX15, FX18, FX1E, FX29, FX33, FX55, FX65");
      switch(NN)
      {
        case 0x07:LOG("FX07: Set VX to the value of te delay timer");
          VX = chip8->delay_timer;
          break;
        case 0x0A:LOG("FX0A: A keypress is awaited, and then stored in VX");
          chip8->waiting = 1;
          break;
        case 0x15:LOG("FX15: Sets the delay timer to VX");
          chip8->delay_timer = VX;
          break;
        case 0x18:LOG("FX18: Sets the sound timer to VX");
          chip8->sound_timer = VX;
          break;
        case 0x1E:LOG("FX1E: Adds VX to I. set VF to 1 if overflow");
          VF = (chip8->I + VX > 0xFFF);
          chip8->I += VX;
          break;
        case 0x29://FX29: Sets I to the location of the spirte for the
                  //character in VX. chatarets 0-F are represented by a 4x5 font
          //Every character is 5 bytes
          chip8->I = VX * 5;
          break;
        case 0x33://get decimal value from VX aand store 100s in I, 10s in I+1
                  //and 1s in I+2
          chip8->memory[chip8->I]  = VX / 100;
          chip8->memory[chip8->I+1]= (VX % 100) / 10;
          chip8->memory[chip8->I+2]= (VX % 10);
          break;
        case 0x55:LOG("FX55: Stores V0 to VX  in memory starting at address I");
          for(size_t i = 0; i <= X; ++i)
            chip8->memory[chip8->I+i] = chip8->V[i];
          break;
        case 0x65:LOG("FX65: Fills V0 to VX in memory starting at address I");
          for(size_t i = 0; i <= X; ++i)
            chip8->V[i] = chip8->memory[chip8->I+i];
          break;
        default: goto invalid;
      }
  }

  done:
  //Decrement timers, or reset to 60 if 0
  chip8->delay_timer -= chip8->delay_timer ? 1 : -60;
  chip8->sound_timer -= chip8->sound_timer ? 1 : -60;
  return;

invalid:
  printf("Invalid opcode: %4x\n",opcode);
  goto done;
}


