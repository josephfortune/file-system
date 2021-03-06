#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "file_system.h"


/********************************************************
 *	initFakeFS
 *
 * Creates a fake file system in memory for the purpose 
 * of testing and developing
 ********************************************************/
void initFakeFS( void )
{
	int nDirs = 9;
	int nFiles = 4;

	header.num_dir = nDirs;
	header.next_id = 20;
	dirs = malloc( sizeof( struct FS_Directory ) * nDirs );
	files = malloc( sizeof( struct FS_File ) * nFiles );
	
	// root
	strncpy( dirs[0].name, "root", MAX_NAME_SIZE );
	dirs[0].num_files = 0;
	dirs[0].num_children = 2;
	dirs[0].id = 0;

	// img
	strncpy( dirs[1].name, "img", MAX_NAME_SIZE );
	dirs[1].num_files = 0;
	dirs[1].num_children = 3;
	dirs[1].id = 1;

	// docs
	strncpy( dirs[2].name, "docs", MAX_NAME_SIZE );
	dirs[2].num_files = 1;
	dirs[2].num_children = 3;
	dirs[2].id = 2;

	// 2009
	strncpy( dirs[3].name, "2009", MAX_NAME_SIZE );
	dirs[3].num_files = 0;
	dirs[3].num_children = 0;
	dirs[3].id = 3;

	// 2010
	strncpy( dirs[4].name, "2010", MAX_NAME_SIZE );
	dirs[4].num_files = 0;
	dirs[4].num_children = 0;
	dirs[4].id = 4;

	// 2011
	strncpy( dirs[5].name, "2011", MAX_NAME_SIZE );
	dirs[5].num_files = 2;
	dirs[5].num_children = 0;
	dirs[5].id = 5;

	// Resume
	strncpy( dirs[6].name, "resume", MAX_NAME_SIZE );
	dirs[6].num_files = 0;
	dirs[6].num_children = 0;
	dirs[6].id = 6;

	// work
	strncpy( dirs[7].name, "work", MAX_NAME_SIZE );
	dirs[7].num_files = 0;
	dirs[7].num_children = 0;
	dirs[7].id = 7;

	// life
	strncpy( dirs[8].name, "life", MAX_NAME_SIZE );
	dirs[8].num_files = 1;
	dirs[8].num_children = 0;
	dirs[8].id = 8;

	
	// manifest.txt
	strncpy( files[0].name, "manifest.txt", MAX_NAME_SIZE );
	files[0].size = 200;

	// 1.jpg
	strncpy( files[1].name, "1.jpg", MAX_NAME_SIZE );
	files[1].size = 200;

	// 2.jpg
	strncpy( files[2].name, "2.jpg", MAX_NAME_SIZE );
	files[2].size = 210;

	// diary.txt
	strncpy( files[3].name, "diary.txt", MAX_NAME_SIZE );
	files[3].size = 100;

	
	
}




/********************************************************
 *	get_index
 *
 * Takes the a directory's ID and returns the directory's
 * in-memory buffer index, or ERROR if not found
 ********************************************************/
uint32_t get_index( uint32_t id )
{
	uint32_t i;
	for ( i = 0; i < header.num_dir; i++ )
	{
		if ( id == dirs[i].id ) return i; 
	}
	return ERROR;
}




/********************************************************
 *	get_parent
 *
 * Takes the a directory's ID and returns the parent
 * directory's ID, or ERROR if not found
 ********************************************************/
uint32_t get_parent( uint32_t id )
{
	uint32_t i, index, dir_offset;
	index = get_index( id );
	if ( index == ERROR ) return ERROR; // If ID not found
	
	for ( i = 0, dir_offset = 0; i < header.num_dir; i++ )
	{
		dir_offset += dirs[i].num_children;
		if ( dir_offset >= index ) return dirs[i].id;		
	}
	return ERROR;
}




/********************************************************
 *	get_dir_offset
 *
 * Takes the a directory's ID and returns the directory's
 * child area offset in the in-memory buffer, or ERROR
 * if ID not found
 ********************************************************/
uint32_t get_dir_offset( uint32_t id )
{
	uint32_t dir_offset, i;
	dir_offset = 0; // Used to keep track of how many directories come before the desired one in the tree

	for ( i = 0; i < header.num_dir; i++ )
	{
		if ( id == dirs[i].id ) // If dir found
		{
			dir_offset++; // Align index into requested directory
			return dir_offset; 
		}
		dir_offset += dirs[i].num_children;
	}
	return ERROR;
}
 



/********************************************************
 *	get_total_num_files
 *
 * Returns the total number of files in the file system.
 * This function will slow down as the file system grows.
 * Minimize its usage by doing book-keeping in memory.
 ********************************************************/
uint32_t get_total_num_files( void )
{
	int num_files, i;
	num_files = 0;
	
	for ( i = 0; i < header.num_dir; i++ )
		num_files += dirs[i].num_files;
	return num_files;
}




/********************************************************
 *	get_file_offset
 *
 * Takes the a directory's ID and returns the directory's
 * in-memory file buffer index, or ERROR if ID not found.
 ********************************************************/
uint32_t get_file_offset( uint32_t id )
{
	uint32_t file_offset, num_files, i;
	file_offset = 0; // Used to keep track of how many files come before the desired one in the tree
	num_files = get_total_num_files(); // ***************************************************** REPLACE WITH CLASS VAR INSTEAD OF USING
									// SLOW get_total_num_files FUNCTION EVERYTIME!!!!

	for ( i = 0; i < header.num_dir; i++ ) // Get num files
	{
		if ( id == dirs[i].id ) // If dir found
		{
			//file_offset++; // Align index into requested directory
			return file_offset; 
		}
		file_offset += dirs[i].num_files;
	}
	return ERROR;
}




/********************************************************
 *	get_num_children
 *
 * Takes the a directory's ID and returns the directory's
 * number of child directories.
 ********************************************************/
inline uint32_t get_num_children( uint32_t id )
{
	return dirs[get_index(id)].num_children;
}




/********************************************************
 *	get_num_files
 *
 * Takes the a directory's ID and returns the directory's
 * file count.
 ********************************************************/
inline uint32_t get_num_files( uint32_t id )
{
	return dirs[get_index(id)].num_files;
}




/********************************************************
 *	get_children
 *
 * Takes the a directory's ID and returns a pointer to
 * an allocated array of FS_Directory structs,
 * representing the files within that directory. 
 * (Don't forget to free the memory)
 ********************************************************/
struct FS_Directory* get_children( uint32_t id )
{
	uint32_t num_children, dir_offset;
	struct FS_Directory *children;

	num_children = dirs[get_index(id)].num_children;
	dir_offset = get_dir_offset( id );

	children = (struct FS_Directory*)malloc( num_children * sizeof( struct FS_Directory) );
	memcpy( children, &dirs[dir_offset], num_children * sizeof( struct FS_Directory ) );

	return children; 
}




/********************************************************
 *	get_files
 *
 * Takes the a directory's ID and returns a pointer to
 * an allocated array of FS_File structs representing the
 * files within that directory. 
 * (Don't forget to free the memory)
 ********************************************************/
struct FS_File* get_files( uint32_t id )
{
	uint32_t num_files, file_offset;
	struct FS_File *dir_files;

	num_files = get_num_files( id );
	file_offset = get_file_offset( id );

	dir_files = (struct FS_File*)malloc( num_files * sizeof( struct FS_File) );
	memcpy( dir_files, &files[file_offset], num_files * sizeof( struct FS_File ) );

	return dir_files; 
}




/********************************************************
 *	print_dir
 *
 * Takes the a directory's ID and prints the child
 * directories and files within.
 ********************************************************/
void print_dir( uint32_t id )
{
	uint32_t i, num_children, num_files;
	struct FS_Directory *children;
	struct FS_File *dir_files;

	num_children = get_num_children( id );
	if ( num_children )
	{
		children = get_children( id );
		for ( i = 0; i < num_children; i++ )
			printf("%s\n", children[i].name );
		free( children );
	}
	

	num_files = get_num_files( id );
	if ( num_files )
	{
		dir_files = get_files( id );
		for ( i = 0; i < num_files; i++ )
			printf("%s\n", dir_files[i].name );
		free( dir_files );
	}
		
}



/********************************************************
 *	add_dir
 *
 * Takes the a directory's ID and adds a child directory
 * by copying the entire original directory buffer to a 
 * new buffer with a gap at the beginning of the parent's
 * child space, and then builds the new directory in that
 * gap. The original buffer is freed and replaced with 
 * the new buffer.
 ********************************************************/
void add_dir( uint32_t parent_id, uint8_t* name )
{
	int struct_size, dir_offset, dirs_buffer_len, first_half_len, scnd_half_len; 

	struct_size = sizeof( struct FS_Directory );
	dirs_buffer_len = struct_size * header.num_dir;	// Original dir buffer size
	dir_offset = get_dir_offset( parent_id );

	// Buffer size split into halves, where the split is where the new dir is to go
	first_half_len = dir_offset * struct_size ;
	scnd_half_len = dirs_buffer_len - first_half_len;
	
	// Allocate space for one exact copy + 1 additional dir
	struct FS_Directory *new_dirs = malloc( dirs_buffer_len + struct_size * 2 ); 
	// WHY DO WE NEED SPACE FOR 2 STRUCT SIZES?????????????????????????????????????????????????????????????????


	// Copy first half of buffer, then second half leaving space for new dir
	memcpy( new_dirs, dirs, first_half_len );
	memcpy( new_dirs + dir_offset + 1, dirs + dir_offset, scnd_half_len + struct_size );

	// Build new dir
	strncpy( new_dirs[dir_offset].name, name, MAX_NAME_SIZE );
	new_dirs[dir_offset].num_files = 0;
	new_dirs[dir_offset].num_children = 0;
	new_dirs[dir_offset].id = next_id();

	// Update header and parent
	header.num_dir++;
	new_dirs[get_index( parent_id )].num_children++;	

	// Free old dirs buffer with new updated one
	free( dirs );
	dirs = new_dirs;	
}


// THIS NEEDS TO BE COMPLETELY REDONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int rem_dir_leaf( uint32_t id )
{
	printf("Deleteing leaf id: %d\n", id );
	int dir_offset, len_after_dir;

	// If dir does not exist
	if ( !( dir_offset = get_dir_offset( id ) ) ) return ERROR;

	// Update header and parent
	header.num_dir--;
	dirs[get_index( get_parent( id ) )].num_children--;

	len_after_dir = ( header.num_dir - dir_offset - 1 ) * sizeof( struct FS_Directory );

	// Collapse directory buffer on deleted space
	memcpy( dirs + dir_offset, dirs + dir_offset + 1, len_after_dir );

	return id;
}

int rem_dir( uint32_t id )
{
	printf("Called rem_dir( id: %d )\n", id );	
	int i, children;
	struct FS_Directory* dir_list;

	children = get_num_children( id );

	// If there are no more children, delete directory
	if ( children == 0 )
	{
		printf("No children in id: %d\n", id );		
		rem_dir_leaf( id );
		return id;
	}

	// Call function recursively over all the child dirs
	dir_list = get_children( id );
	for ( i = 0; i < children; i++ ) 
		rem_dir( dir_list[i].id );
	free( dir_list );

	// Upon return, all children are deleted, now delete this
	printf("Returned for id: %d, and removing this\n", id );
	rem_dir_leaf( id );

// TODO handle errors
	
}

/********************************************************
 *	next_id
 *
 * Gets and increments the next available id from the 
 * header.
 ********************************************************/
uint32_t next_id( void )
{
	uint32_t new_id;

	new_id = header.next_id;
	header.next_id++;
	// TODO make this to store next id update_header();
	
	return new_id;
	
}

//TODO make function to collapse physical file size if dirs are removed, or just write a clean file with every update

// TODO make function than can retrieve an id by name (obviously restricted to within the current directory)

