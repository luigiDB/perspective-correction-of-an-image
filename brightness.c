#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

#define N_BLOCKS 10
#define JPEG_QUALITY 100
//This function divides the image in a certain numbers of blocks. The number of blocks depend on accuracy. 
//From 1, a unique block, to 100, thus each pixel is a block.//to define better
//Moreover, this function applies a "fuzzy" brightness to the image.
//Finish to implement da Pietro
/*
void divide_in_blocks(int accuracy){
 image->cols *= 3;
 new_image->cols *= 3; 
	 
	 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	 for( y = 0; y < image->rows ;  y++ ){
	 	for( x = 0; x < image->cols; x++ ){
	 		//leggo il valore del pixel dell'immagine originale
			pixval = CV_MAT_ELEM(*image, uchar, y, x);
			//ci applico la formula alpha * x + beta
			pixval = saturate(pixval, alpha, beta);
			//scrivo il valore nella solita posizione della nuova immagine
			CV_MAT_ELEM(*new_image, uchar, y, x) = pixval;
	    	}
	 }
	 printf("#Rows = %i #Column = %i\n", y, x);
	 //Rimetto il numero di colonne al loro valore originale, altrimenti non si riesce a visualizzare l'immagine
	 image->cols /= 3;
	 new_image->cols /= 3; 
	
}*/

uchar saturate(uchar pixel, double alpha, int beta){
	uchar ret;
	unsigned int temp = (unsigned int)pixel;
	if((temp * alpha) + beta > 255)
		return 255;
	else 
		return (pixel * alpha) + beta; 
}


void change_brightness(CvMat* image, int alpha, int beta){
	
	CvMat *new_image =cvCloneMat(image);
	uchar pixval;
	int x, y, c;
	int p[3];
	new_image->cols *= 3; 
	image->cols *= 3;

	printf("#Rows = %i #Column = %i\n", image->rows, image->cols);
	 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	 for( y = 0; y < image->rows ;  y++ ){
	 	for( x = 0; x < image->cols; x++ ){
	 		//leggo il valore del pixel dell'immagine originale
			pixval = CV_MAT_ELEM(*image, uchar, y, x);
			//ci applico la formula alpha * x + beta
			pixval = saturate(pixval, alpha, beta);
			//scrivo il valore nella solita posizione della nuova immagine
			CV_MAT_ELEM(*new_image, uchar, y, x) = pixval;
	    	}
	 }
	 printf("#Rows = %i #Column = %i\n", y, x);
	 //Rimetto il numero di colonne al loro valore originale, altrimenti non si riesce a visualizzare l'immagine
	 image->cols /= 3;
	 new_image->cols /= 3; 
	 
	/// Create Windows
	 cvNamedWindow("Original Image", 1);
	 cvNamedWindow("New Image", 1);

	 /// Show stuff
	 cvShowImage("Original Image", image);
	 cvShowImage("New Image", new_image);

	 p[0]=CV_IMWRITE_JPEG_QUALITY;
	 p[1]=JPEG_QUALITY;
	 p[2]=0;
	 cvSaveImage("result.jpg",new_image, p);

	 /// Wait until user press some key
	cvReleaseMat(&new_image);
	cvWaitKey(0);
	cvDestroyAllWindows();
}


int main( int argc, char** argv )
{
	 int alpha; /**< Simple contrast control */
	 int beta;  /**< Simple brightness control */
	 uchar pixval;
	 int x, y, c;
	 /// Read image given by user
	 CvMat *image = cvLoadImageM( argv[1], 1);

	 /// Initialize values
	 printf(" Basic Linear Transforms\n");
	 printf("-------------------------\n");
	 printf("* Enter the alpha value [1.0-3.0]: ");
	 scanf("%i", &alpha);
	 printf("* Enter the beta value [0-100]: ");
	 getchar(); 		//Ignore the '\n' 
	 scanf("%i", &beta);

	 change_brightness(image, alpha, beta);
	 return 0;
}
