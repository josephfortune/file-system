#include <stdio.h>
#include <stdint.h>
#include "file_system.h"


int main( void )
{
	initFakeFS();

	uint32_t cur_dir, num_files;
	num_files = get_total_num_files();	

	cur_dir = 2;

	print_dir( cur_dir );
	//printf("Offset: %d\n", get_file_offset(5) );

}
