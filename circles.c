#include"circles.h"

CvPoint2D32f* get_positions_circles(int *coordinates, CvMat *img){

	CvPoint2D32f *pos_circles=calloc(N_CIRCLES,sizeof(CvPoint2D32f));
	int i;
	int width=img->cols;
	int height=img->rows;
	
	for(i=0; i<8; i+=2){
		pos_circles[i/2].x=coordinates[i]*width/100;
		pos_circles[i/2].y=coordinates[i+1]*height/100;
	}
		/*for(i=0; i<8; i++){DEBUG PRINT
		printf("coordinates: [%d]\t",coordinates[i]);
	}printf("\n");*/

	

	//DEBUG PRINT IN ORDER TO VERIFT THE COORDINATES IN WHICH THE CIRCLES WILL BE DRAWN
	/*for(i=0; i<4; i++){
		printf("[%d][%d]\n",(int)pos_circles[i].x,(int)pos_circles[i].y);
	}
	printf("\n");*/
	return pos_circles;
	}


//Input:  coordinates where the function will draw the circles;
// The image on which the circles are drawn.
void draw_circles(CvPoint2D32f *pos_circles, CvMat *img){
	CvPoint tmp;
	int i;
	//N.B. The circle size depends on image resolution
	int circle_radius=(img->rows)*(img->cols)/500000;
	int circle_fill=circle_radius*0.5;
	int line_type=8;

	for(i=0; i<N_CIRCLES; i++){
		tmp.x=pos_circles[i].x;
		tmp.y=pos_circles[i].y;
		cvCircle(img,tmp,circle_radius,RED,circle_fill,line_type,0);
	}

	
}

//Print on video the image with the circles on it.
void view_preview(CvMat *img){
	cvNamedWindow( "Preview", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO );
	cvResizeWindow("Preview", 600, 800);
	cvShowImage( "Preview", img );
	cvWaitKey(0);
	cvDestroyWindow("Preview" );
}
