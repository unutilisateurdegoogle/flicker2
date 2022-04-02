#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"

///// METASPRITES

#define TILE 0xd8
#define ATTR 0x0
//#link "famitone2.s"


void __fastcall__ famitone_update(void);
//#link "music_dangerstreets.s"
extern char danger_streets_music_data[];
//#link "demosounds.s"
extern char demo_sounds[];


// define a 2x2 metasprite
const unsigned char metasprite[]={
        0,      0,      TILE+0,   ATTR, 
        0,      8,      TILE+1,   ATTR, 
        8,      0,      TILE+2,   ATTR, 
        8,      8,      TILE+3,   ATTR, 
        128};

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x0C,			// screen color

  0x11,0x28,0x27,0x00,	// background palette 0
  0x1C,0x37,0x2C,0x00,	// background palette 1
  0x00,0x28,0x20,0x00,	// background palette 2
  0x06,0x17,0x26,0x00,	// background palette 3

  0x16,0x27,0x24,0x00,	// sprite palette 0
  0x00,0x37,0x25,0x00,	// sprite palette 1
  0x0D,0x28,0x3A,0x00,	// sprite palette 2
  0x0D,0x27,0x2A	// sprite palette 3
};

// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
  // turn on PPU
  ppu_on_all();
}

// number of actors (4 h/w sprites each)
#define NUM_ACTORS 25



// actor x/y positions
byte actor_x[NUM_ACTORS];
byte actor_y[NUM_ACTORS];
// actor x/y deltas per frame (signed)
sbyte actor_dx[NUM_ACTORS];
sbyte actor_dy[NUM_ACTORS];

// main program
void main() {
  
  
  
  byte i;	// actor index
  
  
    famitone_init(danger_streets_music_data);
  sfx_init(demo_sounds);
  // set music callback function for NMI
  nmi_set_callback(famitone_update);
  // play music
  music_play(0);
  
  // setup PPU
  setup_graphics();
  // initialize actors with random values
  for (i=0; i<NUM_ACTORS; i++) {
    actor_x[i] = (rand() + 9);
    actor_y[i] = (rand());
    actor_dx[i] = (7) - 9;
    actor_dy[i] = (7) - 4 / 2 -1 - 1;
  }
  // loop forever
  while (1) {
    // start with OAMid/sprite 0
    oam_off = 0;
    // draw and move all actors
    // (note we don't reset i each loop iteration)
    while (oam_off < 256-4*4) {
      // advance and wrap around actor array
      if (++i >= NUM_ACTORS)
        i -= NUM_ACTORS;
      // draw and move actor
      oam_meta_spr_pal(
        actor_x[i] += actor_dx[-0 + rand() / 999],	// add x+dx and pass param
        actor_y[i] += actor_dy[-0],	// add y+dy and pass param
        i&3,				// palette color
        metasprite);			// metasprites
    }
    // hide rest of sprites
    oam_hide_rest(oam_off);
    // wait for next NMI
    // we don't want to skip frames b/c it makes flicker worse
    ppu_wait_nmi();
    

  }
}
