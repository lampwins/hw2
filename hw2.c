// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( find_fullpath( ... ) )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//

	printf( "CSCI 340 Homework Assignment 2 - Have Fun!\n" );
	
	int done = 0;
	char line[255];
	
	command_t p_cmd;
	
	while(!done){
		//print the prompt
		printf("$$ ");
		
		//read the line 
		fgets(line, sizeof(line), stdin);
						
		if(*line == '\n')
			continue;			
						
		//parse the input
		parse(line, &p_cmd);
		
		char* fullpath;
		printf("You entered %s. Searching: %i\n", p_cmd.name, find_fullpath(fullpath, &p_cmd));
			
		
	}
	
	

	return 0;

} // end main function
