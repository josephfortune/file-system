#define MAX_NAME_SIZE 32
#define ERROR -1

struct FS_Header
{
	uint8_t magic[4];
	uint32_t num_dir;
	uint32_t next_id;
}__attribute__((packed));

struct FS_Directory
{
	uint8_t name[MAX_NAME_SIZE];
	uint32_t num_files;
	uint32_t num_children;
	uint32_t id;
}__attribute__((packed));

struct FS_File
{
	uint8_t name[MAX_NAME_SIZE];
	uint32_t size;
}__attribute__((packed));

// ======================= GLOBALS =======================
struct FS_Header header;
struct FS_Directory *dirs;
struct FS_File *files;

// =======================================================


// File System Wide
void initFakeFS( void );
uint32_t get_total_num_files( void );

// Directories
uint32_t get_index( uint32_t id );
uint32_t get_parent( uint32_t id );
uint32_t get_dir_offset( uint32_t id );
inline uint32_t get_num_children( uint32_t id );
struct FS_Directory* get_children( uint32_t id );
inline uint32_t get_num_files( uint32_t id );
struct FS_File* get_files( uint32_t id );
void add_dir( uint32_t parent_id, uint8_t* name );
int rem_dir_leaf( uint32_t parent_id );
int rem_dir( uint32_t parent_id );
void print_dir( uint32_t id );
uint32_t next_id( void );

// Files
uint32_t get_file_offset( uint32_t id );







