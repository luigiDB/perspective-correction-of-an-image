/*
2nd Project Advanced Topics in Computer Systems and Network.
I give in input these parameters: input file (edge ?) output file.

If the inpunt file is not specified the source code get for default the inputfile.jpg. 

If the file it doesn't exist will appeare an error
The default output file is  output.jpg.
It will be shown.

In order to compile you have to write this on the console:
gcc transformation.c `pkg-config --cflags --libs opencv`

Where transformation.c is the name of this source code.
If it doesn't compile check if the libraries are installed.

In order to execute:
/a.out RMR5y.png ciao.jpg

Where RMR5y.png is the input file and ciao.jpg is the output file.
*/


#include <opencv/highgui.h>
//#include <opencv/cv.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef JPEG_QUALITY

#define JPEG_QUALITY 80
	
int homography_transformation(CvMat* src, char* out_filename, CvPoint2D32f* srcQuad);

#endif



