#include <string>
#include <vector>

typedef enum WadType {
  IWAD = 0,
  PWAD = 1,
} wadtype_t;

typedef struct wadheader {
  wadtype_t kind;
  uint32_t directory_count;
  uint32_t directory_offset;
} wadheader_t;

typedef struct directory_entry {
  uint32_t lump_offset;
  uint32_t lump_size;
  char lump_name[8];
} directory_entry_t;

class WadReader {
public:
  WadReader(std::string path);
  ~WadReader();

private:
  std::vector<char> filedata;

  // Functions to help with initialization
  wadheader_t ReadHeader();
  std::vector<directory_entry_t> ReadDirectory();

  wadheader_t wadheader;
  std::vector<directory_entry_t> directory;
};
