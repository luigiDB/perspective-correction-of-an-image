#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

#define N_CIRCLES 4
#define RED cvScalar(0,0,255,0)
#ifndef JPEG_QUALITY
#define JPEG_QUALITY 50
	
#endif


#ifndef CIRCLES_H
#define CIRCLES_H 101010

CvPoint2D32f* get_positions_circles(int *coordinates, CvMat *img);
void draw_circles(CvPoint2D32f *pos_circles, CvMat *img);
void view_preview(CvMat *img);

#endif
