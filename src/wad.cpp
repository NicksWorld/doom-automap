#include "wad.h"
#include "lumps.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

WadReader::WadReader(std::string path) {
  std::ifstream wadfile(path.c_str(), std::ios::binary | std::ios::ate);

  std::streamsize size = wadfile.tellg();
  wadfile.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);

  wadfile.read(buffer.data(), size);

  filedata = buffer;

  // Parse important info for loading data
  this->wadheader = this->ReadHeader();
  this->directory = this->ReadDirectory();
}

WadReader::~WadReader() {}

wadheader_t WadReader::ReadHeader() {
  wadheader_t header;

  char wadtype[4];
  std::memcpy(&wadtype, filedata.data(), sizeof(char[4]));
  if (std::strncmp(wadtype, "IWAD", 4) == 0) {
    header.kind = WadType::IWAD;
  } else if (std::strncmp(wadtype, "PWAD", 4) == 0) {
    header.kind = WadType::PWAD;
  } else {
    fprintf(stderr, "Invalid wad type! Aborting!\n");
    exit(1);
  }

  std::memcpy(&header.directory_count, filedata.data() + 4, sizeof(uint32_t));
  std::memcpy(&header.directory_offset, filedata.data() + 8, sizeof(uint32_t));

  return header;
}

std::vector<directory_entry_t> WadReader::ReadDirectory() {
  directory_entry_t* list_ptr =
    (directory_entry_t*) (filedata.data() + wadheader.directory_offset);

  std::vector<directory_entry_t> dir(list_ptr, list_ptr + wadheader.directory_count);
  return dir;
}

int WadReader::LocateMapIndex(char mapname[8]) {
  for(int i = 0; i < directory.size(); i++) {
    if (std::strncmp(mapname, directory[i].lump_name, 8) == 0) {
      return i;
    }
  }
  return -1;
}

std::vector<vertex_t> WadReader::ReadVertexes(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<vertex_t>();
  }

  directory_entry_t vertex_entry = directory[map_index + MapLumps::VERTEXES];
  vertex_t* vertex_ptr = (vertex_t*) (filedata.data() + vertex_entry.lump_offset);

  std::vector<vertex_t> vertexes(vertex_ptr, vertex_ptr + (vertex_entry.lump_size / sizeof(vertex_t)));
  return vertexes;
}

std::vector<linedef_t> WadReader::ReadLinedefs(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<linedef_t>();
  }

  directory_entry_t linedef_entry = directory[map_index + MapLumps::LINEDEFS];
  linedef_t* linedef_ptr = (linedef_t*) (filedata.data() + linedef_entry.lump_offset);

  std::vector<linedef_t> linedefs(linedef_ptr, linedef_ptr + (linedef_entry.lump_size / sizeof(linedef_t)));
  return linedefs;
}

std::vector<sector_t> WadReader::ReadSectors(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<sector_t>();
  }

  directory_entry_t sector_entry = directory[map_index + MapLumps::SECTORS];
  sector_t* sector_ptr = (sector_t*) (filedata.data() + sector_entry.lump_offset);

  std::vector<sector_t> sectors(sector_ptr, sector_ptr + (sector_entry.lump_size / sizeof(sector_t)));
  return sectors;
}

std::vector<subsector_t> WadReader::ReadSubsectors(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<subsector_t>();
  }

  directory_entry_t ssector_entry = directory[map_index + MapLumps::SSECTORS];
  subsector_t* ssector_ptr = (subsector_t*) (filedata.data() + ssector_entry.lump_offset);

  std::vector<subsector_t> ssectors(ssector_ptr, ssector_ptr + (ssector_entry.lump_size / sizeof(subsector_t)));
  return ssectors;
}

std::vector<seg_t> WadReader::ReadSegs(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<seg_t>();
  }

  directory_entry_t seg_entry = directory[map_index + MapLumps::SEGS];
  seg_t* seg_ptr = (seg_t*) (filedata.data() + seg_entry.lump_offset);

  std::vector<seg_t> segs(seg_ptr, seg_ptr + (seg_entry.lump_size / sizeof(seg_t)));
  return segs;
}

std::vector<sidedef_t> WadReader::ReadSidedefs(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<sidedef_t>();
  }

  directory_entry_t sidedef_entry = directory[map_index + MapLumps::SIDEDEFS];
  sidedef_t* sidedef_ptr = (sidedef_t*) (filedata.data() + sidedef_entry.lump_offset);

  std::vector<sidedef_t> sidedefs(sidedef_ptr, sidedef_ptr + (sidedef_entry.lump_size / sizeof(sidedef_t)));
  return sidedefs;
}

std::vector<thing_t> WadReader::ReadThings(char mapname[8]) {
  int map_index = LocateMapIndex(mapname);
  if(map_index == -1) {
    return std::vector<thing_t>();
  }

  directory_entry_t things_entry = directory[map_index + MapLumps::THINGS];
  thing_t* thing_ptr = (thing_t*) (filedata.data() + things_entry.lump_offset);

  std::vector<thing_t> things(thing_ptr, thing_ptr + (things_entry.lump_size / sizeof(thing_t)));
  return things;
}

std::vector<color_t> WadReader::ReadPlaypal() {
  char playpal[8] = "PLAYPAL";

  for(int i = 0; i < directory.size(); i++) {
    if (std::strncmp(playpal, directory[i].lump_name, 8) == 0) {
      color_t* color_ptr = (color_t*) (filedata.data() + directory[i].lump_offset);
      std::vector<color_t> colors(color_ptr, color_ptr + (14 * 256));
      return colors;
    }
  }
  return std::vector<color_t>();
}
