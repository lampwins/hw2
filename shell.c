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

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// --------------------------------------------
// Implementation for each prototype below
// --------------------------------------------
// void parse( char* line, command_t* p_cmd );
// int execute( command_t* p_cmd );
// int find_fullpath( char* fullpath, command_t* p_cmd );
// int is_builtin( command_t* p_cmd );
// int do_builtin( command_t* p_cmd );
// void cleanup( command_t* p_cmd );

int find_fullpath( char* fullpath, command_t* p_cmd ){
	
	struct stat buffer;
	int partCount, i, start_pos, exists, startConcatPos, j;
	
	char* path_env_variable;
	path_env_variable = getenv( "PATH" );
	printf("PATH = %s\n", path_env_variable );
	
	partCount = number_of_needle_in_hey_stack(path_env_variable, ':') + 1;
	
	start_pos = 0;
	for(i = 0; i < partCount; i++){
		char part[pos_of_next_char(path_env_variable, start_pos, ':') - start_pos];
		
		copy_string_part(path_env_variable, part, start_pos, pos_of_next_char(path_env_variable, start_pos, ':') - 1);
		
		char fullPath[string_length(part) + string_length(p_cmd->name)];
		
		copy_string_part(part, fullPath, 0, string_length(part));
		
		startConcatPos = string_length(part);
		
		for(j = 0; j < string_length(p_cmd->name); j++){
			fullPath[startConcatPos + j] = p_cmd->name[j];
		}
		
		
		exists = stat(fullPath, &buffer); 
		
		
		if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) )  {
        	// Directory exists
        	return 0;
		} else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
          // File exists
          
        	free(p_cmd->name);
        	p_cmd->name = malloc(string_length(fullPath));
        	copy_string_part(fullPath, p_cmd->name, 0, string_length(fullPath));
          
        	return 1;
		} else {
			return 0;
		}
	}
	
	
	return 0;
}

int number_of_needle_in_hey_stack(char* str, char needle){
	int i = 0, counter = 0;
	while(str[i] !=  '\0'){
		if(str[i] == needle){
			counter++;
		}
		i++;
	}
			
	return counter;
}

int is_builtin( command_t* p_cmd ){
	int i, j;
	
	for(i = 0; i < p_cmd->argc; i++){
		for(j = 0; j < string_length(p_cmd->argv[i]); j++){
			if(p_cmd->argv[i][j] != p_cmd->name[j]){
					return 0;
			}
		}
	}
	
	return 1;
}

void parse( char* line, command_t* p_cmd ){

	//declare vars
	int i, partCount, start_pos, pos_of_next_space, length_of_part;
	
	//remove the line break at the end of the line
	line[string_length(line) - 2] = '\0';

	//get the number of of spaces and add one, this is the number of parts
	partCount += number_of_needle_in_hey_stack(line, ' ') + 1;
	
	//allocate space for all the parts (+1 for null)
	p_cmd->argv = (char **) malloc(sizeof(char *) * (partCount + 1));

	//set argc to the number of parts
	p_cmd->argc = partCount;
	
	//loop through all parts
	start_pos = 0;
	for(i = 0; i < partCount; i++){
		
		//get the pos of the next space
		pos_of_next_space = pos_of_next_char(line, start_pos, ' ');
		
		//calculate the length of the current part
		length_of_part = pos_of_next_space - start_pos;
		
		//allocate space for this part and copy the part in
		p_cmd->argv[i] = malloc(sizeof(char *) * (length_of_part));
		copy_string_part(line, p_cmd->argv[i], start_pos, pos_of_next_space);
		
		//check if first part
		if(i == 0){
			// allocate and copy first part to cmd name
			p_cmd->name = malloc(sizeof(char *) * (length_of_part));
			copy_string_part(p_cmd->argv[i], p_cmd->name, 0, length_of_part);
		}
		
		//calculate new starting pos
		start_pos = pos_of_next_space + 1;
	}
	
	//null terminate the argv array
	p_cmd->argv[partCount] = NULL;
	
}

int pos_of_next_char(char* str, int start_pos, char needle){
	int i = start_pos;
	while(str[i] != '\0'){
		if(str[i] == needle){
			if(str[i + 1] == '\0'){
				str[i] = '\0';
			}
			return i;
		}
		i++;
	}
	
	return i;
}

void copy_string_part(char* src, char* dest, int start, int stop){
	
	int i = 0;
	while((dest[i] = src[start])){
		if(start++ == stop)
			break;
		i++;
	}
	dest[i] = '\0';
}

int string_length(char* str){
	
	int i = 0;
	
	while(str[i] != '\0')
		i++;
		
	return ++i;
}


int execute( command_t* p_cmd ){

	int fnd = FALSE;
	char* fullpath;
	int status;

	fnd = find_fullpath( fullpath, p_cmd );
		
	if ( fnd ) {
		if ( fork() == 0 ) {
			execv( fullpath, p_cmd->argv );
		}
		wait( &status );
	} else {
		// display to user cmd not found
	}
	
	return 1;
}