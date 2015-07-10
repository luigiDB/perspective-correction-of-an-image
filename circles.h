/*
What does do this source code: Giving an input image and 4 (Normalized) coordinates, it draws the circles in those coordinates.
------------------------------------------------------------------------------------
How to compile: gcc <thisfile.c> -o <exefile> `pkg-config --cflags --libs opencv`
How to execute: ./<exefile> <inputImage> <c1> <c2> <c3> <c4> <c5> <c6> <c7> <c8> 
Where c* are the normalized coordinates. The angular parentesis must not be considered.
-----------------------------------------------------------------------------------
For instance:
Compile: gcc circles.c -o circles `pkg-config --cflags --libs opencv`
Execute: ./circles IMG_2348.JPG 30 25 70 25 25 70 75 75
-------------------------------------------------------------------------------------
*/

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include <stdlib.h>
#include <stdio.h>

#define N_CIRCLES 4
#define RED cvScalar(0,0,255,0)
#ifndef JPEG_QUALITY

	#define JPEG_QUALITY 50
	
#endif


int check_bounds(char* bound_char, int *a);
CvMat *get_img(int argc, char **argv);
CvPoint2D32f* get_positions_circles(int argc, char **argv, CvMat *img);
void draw_circles(CvPoint2D32f *pos_circles, CvMat *img);
void view_preview(CvMat *img);


