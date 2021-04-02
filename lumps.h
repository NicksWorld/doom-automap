#include <cstdint>
#include <stdlib.h>
#include <sys/types.h>

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
