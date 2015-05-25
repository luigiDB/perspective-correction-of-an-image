#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>


uchar saturate(uchar pixel, double alpha, int beta){
	uchar ret;
	unsigned int temp = (unsigned int)pixel;
	if((temp * alpha) + beta > 255)
		return 255;
	else 
		return (pixel * alpha) + beta; 
}

int main( int argc, char** argv )
{
	 double alpha; /**< Simple contrast control */
	 int beta;  /**< Simple brightness control */
	 uchar pixval;
	 int x, y, c;
	 int p[3];
	 /// Read image given by user
	 CvMat *image = cvLoadImageM( argv[1], 1 );
	 CvMat *new_image =cvCloneMat(image);
	 

	 /// Initialize values
	 printf(" Basic Linear Transforms\n");
	 printf("-------------------------\n");
	 printf("* Enter the alpha value [1.0-3.0]: ");
	 scanf("%d", &alpha);
	 printf("* Enter the beta value [0-100]: ");
	 getchar(); 		//Ignore the '\n' 
	 scanf("%i", &beta);

	 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	 for( y = 0; y < image->rows;  y++ ){
	 	for( x = 0; x < image->cols; x++ ){
		      	//printf("Y = %i, X = %i, C = %i \n", y, x, c);
		        pixval = CV_MAT_ELEM(*image, uchar, y, x);
		        CV_MAT_ELEM(*new_image, uchar, y, x) = saturate(pixval, alpha, beta);
	      		/*new_image.at<Vec3b>(y,x)[c] =
	      			saturate_cast<uchar>(alpha*( image.at<Vec3b>(y,x)[c] ) + beta );*/
		  
	    	}
	 }

	 /// Create Windows
	 cvNamedWindow("Original Image", 1);
	 cvNamedWindow("New Image", 1);

	 /// Show stuff
	 cvShowImage("Original Image", image);
	 cvShowImage("New Image", new_image);

	 p[0]=CV_IMWRITE_JPEG_QUALITY;
	 p[1]=100;
	 p[2]=0;
	 cvSaveImage("result.jpg",new_image, p);

	 /// Wait until user press some key
	 cvWaitKey(0);
	 return 0;
}
