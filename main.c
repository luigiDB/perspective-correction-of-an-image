/*
 * compile with 
 * gcc -g -o main main.c
 */
 
//#include "circles.h"
//#include "transformation.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CORNER 8
#define TEST_MODE 46
#define NORMAL_MODE 64

void help(){
	char* options_msg = "Usage: \n\
		-i \"<filename>\"........: input file name\n\
		-c \"<corner>\"..........: position of the 4 corners separated by commas\n\
		-t ....................: test mode\n\
		-o \"<filename>\"........: output file name\n\
		-h ....................: show this help\n";
        
	fprintf(stderr,"%s", options_msg);		
}

int main(int argc, char** argv){
	char opt;
	char* input_file = NULL;
	char* output_file = NULL;
	int corners[N_CORNER];
	char* corners_string = NULL;
	char* corners_string_support = NULL;	//needed because strtok() destroy original string
	int mode = NORMAL_MODE;
	int i;
	
	
	//10 argumets are mandatory
	if(argc < 3){
		fprintf(stderr,"Too few argumets.\n");
		help();
		exit(0);
	}
	
	
	while((opt = getopt(argc, argv, "i:c:to:h")) != -1){
		switch(opt){
			case 'i':
				input_file = strdup(optarg);
				break;
			case 'c':
				corners_string = strdup(optarg);
				corners_string_support = strtok(corners_string,",");
				
				i = 0;
				while (corners_string_support != NULL)
				{
					corners[i] = atoi(corners_string_support);
					corners_string_support = strtok (NULL, " ,.-");
					i++;
				}
				if(i != N_CORNER){
					exit(0);
				}
				break;
			case 't':
				mode = TEST_MODE;
				break;
			case 'o':
				output_file = strdup(optarg);
				break;
			case 'h':
				help();
				break;
			default:
				help();
				exit(0);
		}
	}
	
	if(input_file == NULL){	
		fprintf(stderr,"Needed input file\n");
		exit(0);
	}
	
	if(output_file == NULL){	
		output_file = (char*)malloc(sizeof("output.jpg")+1);
		strcpy(param.output_file, "output.txt\0");
	}
	
	//convert coordinates from percentage to absolute
	// usare la get_positions_circles
	//che deve essere modificata per leggere un array di interi
	
	if( mode == NORMAL_MODE ){
		//call transformation
		//ret = transformation(input_file, output_file, corners);
	} else {
		//call circles
		//ret = circles(input_file, corners);
	}
	
	free(output_file);
	
	return 0;
}
