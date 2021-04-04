#ifndef LUMP_H
#define LUMP_H

#include <cstdint>
#include <stdlib.h>
#include <sys/types.h>

enum MapLumps {
  THINGS = 1,
  LINEDEFS,
  SIDEDEFS,
  VERTEXES,
  SEGS,
  SSECTORS,
  NODES,
  SECTORS,
  REJECT,
  BLOCKMAP,
};

typedef struct Vertex {
  int16_t x;
  int16_t y;
} vertex_t;

// TODO: Find a better format for storing the flags (and verify them)
#define LINEDEF_BLOCK_ALL 1
#define LINEDEF_BLOCK_MONSTERS 1 << 1
#define LINEDEF_TWO_SIDED 1 << 2
#define LINEDEF_UPPER_TEXTURE_UNPEGGED 1 << 3
#define LINEDEF_LOWER_TEXTURE_UNPEGGED 1 << 4
#define LINEDEF_SECRET 1 << 5
#define LINEDEF_BLOCK_SOUND 1 << 6
#define LINEDEF_AUTOMAP_HIDDEN 1 << 7
#define LINEDEF_AUTOMAP_ALWAYS_SHOWN 1 << 8

typedef struct Linedef {
  uint16_t start_vertex;
  uint16_t end_vertex;

  uint16_t flags;
  uint16_t kind;

  uint16_t sector_tag;

  uint16_t right_sidedef;
  uint16_t left_sidedef;
} linedef_t;

typedef struct Sector {
  int16_t floor_height;
  int16_t ceiling_height;
  char floor_name[8];
  char ceiling_name[8];
  int16_t lightlev;
  int16_t kind;
  int16_t tag;
} sector_t;

typedef struct Subsector {
  uint16_t seq_count;
  uint16_t first_seg;
} subsector_t;

typedef struct Seg {
  uint16_t start_vertex;
  uint16_t end_vertex;
  uint16_t angle;
  uint16_t linedef_num;
  uint16_t direction;
  uint16_t offset;
} seg_t;

typedef struct Sidedef {
  int16_t x_offset;
  int16_t y_offset;
  char upper_texture[8];
  char lower_texture[8];
  char middle_texture[8];
  int16_t sector_num;
} sidedef_t;

typedef struct Thing {
  int16_t x;
  int16_t y;
  int16_t angle;
  int16_t kind;
  int16_t flags;
} thing_t;

typedef struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color_t;

#endif // LUMP_H
