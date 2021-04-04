#include "lumps.h"
#include "wad.h"
#include "mapcolors.h"

#include <cstring>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: ./doomsvg </path/to/wad> <mapname>\n");
    exit(1);
  }

  if (std::strlen(argv[2]) > 8) {
    printf("Valid map names are less than 8 characters\n");
    exit(1);
  }

  // Load a wadfile
  WadReader wadfile(argv[1]);

  std::vector<vertex_t> vertexes = wadfile.ReadVertexes(argv[2]);
  std::vector<linedef_t> lines = wadfile.ReadLinedefs(argv[2]);
  std::vector<sector_t> sectors = wadfile.ReadSectors(argv[2]);
  std::vector<subsector_t> ssectors = wadfile.ReadSubsectors(argv[2]);
  std::vector<seg_t> segs = wadfile.ReadSegs(argv[2]);
  std::vector<sidedef_t> sidedefs = wadfile.ReadSidedefs(argv[2]);
  std::vector<thing_t> things = wadfile.ReadThings(argv[2]);

  int min_x = vertexes[0].x;
  int min_y = vertexes[0].y;

  int max_x = vertexes[0].x;
  int max_y = vertexes[0].y;

  for (int i = 0; i < vertexes.size(); i++) {
    int x = vertexes[i].x;
    int y = vertexes[i].y;

    if (x < min_x) {
      min_x = x;
    }
    if (x > max_x) {
      max_x = x;
    }
    if (y < min_y) {
      min_y = y;
    }
    if (y > max_y) {
      max_y = y;
    }
  }

  std::vector<color_t> playpal = wadfile.ReadPlaypal();

  int x_offset = 0 - min_x;
  int y_offset = 0 - min_y;

  int resolution_x = max_x + x_offset;
  int resolution_y = max_y + y_offset;
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>");
  printf("<svg height=\"%d\" width=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">",
    resolution_y,
    resolution_x
  );
  color_t background = playpal[MAPCOLOR_BACK];
  printf("<rect width=\"100%%\" height=\"100%%\" stroke-width=\"0\" fill=\"rgb(%d,%d,%d)\"/>",
    background.r,
    background.g,
    background.b
  );

  for(int i = 0; i < lines.size(); i++) {
    vertex_t *vert1 = &vertexes[lines[i].start_vertex];
    vertex_t *vert2 = &vertexes[lines[i].end_vertex];

    color_t color;
    color.r = 255;
    color.g = 0;
    color.b = 0;

    switch(lines[i].kind) {
    case 26: case 32: case 99: case 133:
      color = playpal[MAPCOLOR_BDOR];
      break;
    case 27: case 34: case 136: case 137:
      color = playpal[MAPCOLOR_YDOR];
      break;
    case 28: case 33: case 134: case 135:
      color = playpal[MAPCOLOR_RDOR];
      break;
    case 11: case 52: case 197: case 51: case 124: case 198:
      color = playpal[MAPCOLOR_EXIT];
      break;
    case 39: case 97: case 125: case 126:
      color = playpal[MAPCOLOR_TELE];
    }

    if(lines[i].left_sidedef == 0xFFFF) {
      sector_t *sector = &sectors[sidedefs[lines[i].right_sidedef].sector_num];
      if (sector->kind == 9 || (sector->kind & LINEDEF_SECRET)) {
        color = playpal[MAPCOLOR_SECR];
      } else {
        color = playpal[MAPCOLOR_WALL];

        if (sector->ceiling_height == sector->floor_height) {
          color = playpal[MAPCOLOR_CLSD];
        }
      }
    } else {
      sector_t *s1 = &sectors[sidedefs[lines[i].left_sidedef].sector_num];
      sector_t *s2 = &sectors[sidedefs[lines[i].right_sidedef].sector_num];

      if (s1->ceiling_height != s2->ceiling_height) {
        color = playpal[MAPCOLOR_CCHG];
      }

      if (s1->floor_height != s2->floor_height) {
        color = playpal[MAPCOLOR_FCHG];
      }

      if (s1->kind == 9 || s2->kind == 9 || (s1->kind & LINEDEF_SECRET) || (s2->kind & LINEDEF_SECRET)) {
        color = playpal[MAPCOLOR_SECR];
      }

      if ((!(lines[i].flags & LINEDEF_SECRET)) && (
        (s1->floor_height == s1->ceiling_height) ||
        (s2->floor_height == s2->ceiling_height)
      )) {
        color = playpal[MAPCOLOR_CLSD];
      }

      if (s1->floor_height == s2->floor_height && s1->ceiling_height == s2->ceiling_height) {
        color = playpal[MAPCOLOR_FLAT];
      }
    }

    if (lines[i].flags & LINEDEF_SECRET) {
      color = playpal[MAPCOLOR_WALL];
    }

    printf("<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(%u,%u,%u);stroke-width:1.5;vector-effect:non-scaling-stroke;\" />",
      vert1->x + x_offset,
      resolution_y - (vert1->y + y_offset),
      vert2->x + x_offset,
      resolution_y - (vert2->y + y_offset),
      color.r,
      color.g,
      color.b
    );
  }

  bool is_multiplayer = false;
  for (int i = 0; i < things.size(); i++) {
    if (things[i].kind > 1 && things[i].kind <= 4) {
      is_multiplayer = true;
    }
  }

  for(int i = 0; i < things.size(); i++) {
    // Player starts + deathmatch start
    if ((things[i].kind >= 1 && things[i].kind <=4)) {
      // Draw player
      color_t color = playpal[MAPCOLOR_SNGL];
      if (is_multiplayer) {
        switch(things[i].kind) {
        case 1:
          // Player 1
          color = playpal[MAPCOLOR_PLYR0];
          break;
        case 2:
          // Player 2
          color = playpal[MAPCOLOR_PLYR1];
          break;
        case 3:
          // Player 3
          color = playpal[MAPCOLOR_PLYR2];
          break;
        case 4:
          // Player 4
          color = playpal[MAPCOLOR_PLYR3];
          break;
        }
      }

      printf("<g width=\"32\" height=\"32\" transform=\"translate(%d, %d)\" style=\"stroke:rgb(%d, %d, %d);\">",
        things[i].x + x_offset, resolution_y - (things[i].y + y_offset),
        color.r,
        color.g,
        color.b
      );

      int angle = 270 - things[i].angle;
      printf("<line x1=\"0\" y1=\"-12\" x2=\"0\" y2=\"16\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("<line x1=\"0\" y1=\"16\" x2=\"4\" y2=\"8\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("<line x1=\"0\" y1=\"16\" x2=\"-4\" y2=\"8\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("<line x1=\"0\" y1=\"-12\" x2=\"4\" y2=\"-16\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("<line x1=\"0\" y1=\"-12\" x2=\"-4\" y2=\"-16\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("<line x1=\"0\" y1=\"-8\" x2=\"4\" y2=\"-12\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("<line x1=\"0\" y1=\"-8\" x2=\"-4\" y2=\"-12\" transform=\"rotate(%d)\" vector-effect=\"non-scaling-stroke\"/>", angle);
      printf("</g>");
    }
  }
  printf("</svg>\n");
}
