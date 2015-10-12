#include <stdio.h>
#include <stdint.h>
#include "file_system.h"

#include "joesDebugTools.h"


int main( void )
{
	initFakeFS();

	uint32_t cur_dir, num_files;
	num_files = get_total_num_files();	

	cur_dir = 0;
	
	//add_dir( cur_dir, "new" );
	//add_dir( cur_dir, "fml" );
//asciiDump( dirs, header.num_dir * sizeof( struct FS_Directory ) );
printf("\n------------------------------------------\n");

	rem_dir_leaf( 6 );
//asciiDump( dirs, header.num_dir * sizeof( struct FS_Directory ) );
	//print_dir( cur_dir );
	

	printf("Children: %d\n", get_num_children(cur_dir) );

}
