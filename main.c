#include "circles.h"
#include "transformation.h"

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
	CvMat *src;
	CvPoint2D32f *pixel_corners;
	
	
	//3 argumets are mandatory
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
					
					if((corners[i] < 0) || (corners[i] > 100)){
						fprintf(stderr,"%i is not a valid argument.\n", corners[i]);
						exit(0);
					}
					
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
		strcpy(param.output_file, "output.jpg\0");
	}
	
	
	//evaluate pixel coordinates
	src = cvLoadImageM( input, 1);
	if(src==NULL){
		fprintf(stderr,"error loading image\n");
		exit(0);
	}
	pixel_corners = get_positions_circles(corners,src); //////////////////////il primo argomento è il vettore di int
	free(src);
	
	
	if( mode == NORMAL_MODE ){
		//call transformation
		ret = transformation(input_file, output_file, pixel_corners);
		if(ret != 0){
			fprintf(stderr,"Some error\n");
			exit(0);
		}
	} else {
		//call circles
		//ret = circles(input_file, pixel_corners);
		/*if(ret != 0){
			fprintf(stderr,"Some error\n");
			exit(0);
		}*/
	}
	
	free(output_file);
	
	return 0;
}
