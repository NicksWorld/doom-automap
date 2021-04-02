#include "wad.h"

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
  if (std::strcmp(wadtype, "IWAD") == 0) {
    header.kind = WadType::IWAD;
  } else if (std::strcmp(wadtype, "PWAD") == 0) {
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
  char* directory_list = filedata.data() + wadheader.directory_offset;

  std::vector<directory_entry_t> dir(wadheader.directory_count);

  for(int i = 0; i < wadheader.directory_count; i++) {
    directory_entry_t entry;

    std::memcpy(&entry, directory_list + (i * 16), sizeof(directory_entry_t));

    printf("%s\n", entry.lump_name);

    dir[i] = entry;
  }
  return dir;
}
