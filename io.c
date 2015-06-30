/* io.c: trying out a new formating method. its kinda nice */
#include "io.h"
#include "SDL2/SDL.h"

SDL_Window   *window;
SDL_Renderer *renderer;
SDL_Texture  *texture;

enum
{
  SCALE = 10,
  FREQ  = 90
};

/* Forward Declarations */

int EventFilter (
  void* userdata,
  SDL_Event* event
);

int io_input (
  struct chip8 *chip8
);
int io_output (
  struct chip8 *chip8
);


/* Function Definitions */

int io_init (
  struct chip8 *chip,
  const char   *name
) {
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO);
  atexit(SDL_Quit);
  //Create window
  SDL_CreateWindowAndRenderer (
    GFXWIDTH * SCALE,
    GFXHEIGHT* SCALE,
    0, &window, &renderer
  );
  //Initilize Event handling
  SDL_SetEventFilter(EventFilter, chip);

  char title[128];
  sprintf(title,"CHIP8: %s",name);
  SDL_SetWindowTitle(window, title);
  return 0;
}

int io (
  struct chip8 *chip8
) {
  io_input (chip8);
  io_output(chip8);
  //Pause
  SDL_Delay(1000/FREQ);
  return 0;
}

int io_input (
  struct chip8 *chip8
) {
  SDL_Event event;

//GET INPUT
retry_input:
  if(chip8->waiting)
    SDL_WaitEvent(&event);
  else if(!SDL_PollEvent(&event))
    goto done_input;
  //Complete all input
  do
  {
    //123C
    //456D
    //789E
    //A0BF
    if (event.type == SDL_KEYDOWN)
      switch(event.key.keysym.sym)
      {
        case SDLK_COMMA    : chip8->key |= 1 << 0; break;
        case SDLK_7        : chip8->key |= 1 << 1; break;
        case SDLK_8        : chip8->key |= 1 << 2; break;
        case SDLK_9        : chip8->key |= 1 << 3; break;
        case SDLK_u        : chip8->key |= 1 << 4; break;
        case SDLK_i        : chip8->key |= 1 << 5; break;
        case SDLK_o        : chip8->key |= 1 << 6; break;
        case SDLK_j        : chip8->key |= 1 << 7; break;
        case SDLK_k        : chip8->key |= 1 << 8; break;
        case SDLK_l        : chip8->key |= 1 << 9; break;
        case SDLK_m        : chip8->key |= 1 << 10; break;
        case SDLK_PERIOD   : chip8->key |= 1 << 11; break;
        case SDLK_0        : chip8->key |= 1 << 12; break;
        case SDLK_p        : chip8->key |= 1 << 13; break;
        case SDLK_SEMICOLON: chip8->key |= 1 << 14; break;
        case SDLK_SLASH    : chip8->key |= 1 << 15; break;
        case SDLK_q: exit(0);
        case SDLK_w: chip8->waiting = 1; goto done_input;
      }
    else if (event.type == SDL_KEYUP)
      switch(event.key.keysym.sym)
      {
        case SDLK_COMMA    : chip8->key &= ~(1 << 0); break;
        case SDLK_7        : chip8->key &= ~(1 << 1); break;
        case SDLK_8        : chip8->key &= ~(1 << 2); break;
        case SDLK_9        : chip8->key &= ~(1 << 3); break;
        case SDLK_u        : chip8->key &= ~(1 << 4); break;
        case SDLK_i        : chip8->key &= ~(1 << 5); break;
        case SDLK_o        : chip8->key &= ~(1 << 6); break;
        case SDLK_j        : chip8->key &= ~(1 << 7); break;
        case SDLK_k        : chip8->key &= ~(1 << 8); break;
        case SDLK_l        : chip8->key &= ~(1 << 9); break;
        case SDLK_m        : chip8->key &= ~(1 << 10); break;
        case SDLK_PERIOD   : chip8->key &= ~(1 << 11); break;
        case SDLK_0        : chip8->key &= ~(1 << 12); break;
        case SDLK_p        : chip8->key &= ~(1 << 13); break;
        case SDLK_SEMICOLON: chip8->key &= ~(1 << 14); break;
        case SDLK_SLASH    : chip8->key &= ~(1 << 15); break;
        case SDLK_q: exit(0);
        default: goto retry_input;
      }
    else if (event.type == SDL_QUIT)
    {
      exit(0);
    }
  } while(SDL_PollEvent(&event));
  chip8->waiting = 0;
done_input:
  return 0;
}

int io_output (
  struct chip8 *chip8
) {
  if(chip8->n)
    for(int y = chip8->y; y < chip8->y + chip8->n; ++y)
    {
      int x = chip8->x;
      SDL_Rect rect = { x*SCALE, y*SCALE, SCALE  , SCALE };
      Uint8 color;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color+50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color+50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color + 50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color + 50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color + 50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color + 50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color + 50, 0);
      SDL_RenderFillRect    (renderer, &rect);
      rect.x += SCALE;
      x++;
      color = 200 * !!(chip8->graphics[x/8 + y*GFXWIDTH/8] & (1 << (7-x%8)));
      SDL_SetRenderDrawColor(renderer, color, color+25, color + 50, 0);
      SDL_RenderFillRect    (renderer, &rect);
    }
  else if(chip8->clear)
  {
    SDL_SetRenderDrawColor(renderer,0 , 25, 50, 0);
    SDL_RenderClear(renderer);
  }
  chip8->n = chip8->clear = 0;
  //Update screen
  SDL_RenderPresent(renderer);
}

int EventFilter (
  void* userdata,
  SDL_Event* event
) {
  struct chip8 *chip8 = (struct chip8*)userdata;
  if (
    event->type == SDL_KEYDOWN
    || event->type == SDL_KEYUP
  ) switch (event->key.keysym.sym)
    {
      case SDLK_7:case SDLK_8:    case SDLK_9: case SDLK_0:
      case SDLK_u:case SDLK_i:    case SDLK_o: case SDLK_p:
      case SDLK_j:case SDLK_k:    case SDLK_l: case SDLK_SEMICOLON:
      case SDLK_m:case SDLK_COMMA:case SDLK_PERIOD: case SDLK_SLASH:
      //   Quit       //pause
      case SDLK_q: case SDLK_w:
        return 1;
      default:
        return 0;
    }
  else if(event->type == SDL_QUIT)
    return 1;
  return 0;
}
