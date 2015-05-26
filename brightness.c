#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>


#define N_BLOCKS 10
#define JPEG_QUALITY 100



void print_block_coordinates(CvPoint2D32f* srcQuad){
fprintf(stderr,"Block coordinates:\nTopLeft[%d][%d]	TopRight[%d][%d]\nBottomLeft[%d][%d]	BottomRight[%d][%d]\n",(int)srcQuad[0].x, (int)srcQuad[0].y, (int)srcQuad[1].x, (int)srcQuad[1].y, (int)srcQuad[2].x, (int)srcQuad[2].y, (int)srcQuad[3].x, (int)srcQuad[3].y);
}


//Get the average brightness from an (sub)image given the coordinates.
//The image is passed to the fuction in grayscale. 

int getBeta(CvMat* image, CvPoint2D32f* srcQuad){
	unsigned int beta, x, y;
	unsigned int height_block=srcQuad[2].y-srcQuad[0].y;
	unsigned int width_block=srcQuad[1].x-srcQuad[0].x;
	unsigned int resolution_block=height_block*width_block;
	unsigned int tmp=0;

	printf("resolution_block=%d\n",resolution_block);
	int p[]={CV_IMWRITE_JPEG_QUALITY,JPEG_QUALITY,0};
	for(y=srcQuad[0].y; y<srcQuad[2].y; y++){
		for(x=srcQuad[0].x; x<srcQuad[1].x; x++){
	tmp+=CV_MAT_ELEM(*image, uchar, y, x);
		}
	}
	print_block_coordinates(srcQuad);
	beta=tmp/resolution_block;
	printf("Average brightness block:%d\n",beta);	
	cvSaveImage("resultBN.jpg",image, p);
	return beta;
}


//This function prints the blocks coordinates
void divide_in_blocks(CvMat* image, int n){
	if(n<2) return;
	CvPoint2D32f srcQuad[3]; //Source corners.(Top left, top right, bottom left)
	int step_width=image->cols/n;
	int step_height=image->rows/n;
	int n_blocks=pow(n,2);//How many blocks I have (n^2)
	int change_width=image->cols%n;//The width and the height not always are multiple of n. The change (Il resto) of pixel will be spreaded over the block (x and y)
	int change_height=image->rows%n;
	//The image will have (n+1)^2 blocks. Each block will have the following dimension: step_width (+ change_width) x step_height (+ change_height)
	printf("Blocks coordinates:\n");
	int i,j;
	int start_x=0;
	int start_y=0;

	for(i=0; i<3; i++){
		srcQuad[i].x=0;
		srcQuad[i].y=0;
	}

	for(i=0; i<=n; i++){
		int change_width_tmp=change_width;
		for(j=0; j<=n; j++){
			//
			srcQuad[0].x=start_x;//Top left coordinates
			srcQuad[0].y=start_y;

			start_x+=step_width;

			srcQuad[1].x=start_x;//Top right coordinates
			srcQuad[1].y=srcQuad[0].y;

			if(change_width_tmp>0){ 
				start_x++;
				change_width_tmp--;
				}
			}
			start_y+=+step_height;
			srcQuad[2].x=srcQuad[0].x;//Bottom left coordinates
			srcQuad[2].y=start_y;

			if(change_height>0){
				start_y++;
				change_height--;
			}
			start_x=0;
	}
}


uchar saturate(uchar pixel, double alpha, int beta){
	uchar ret;
	unsigned int temp = (unsigned int)pixel;
	if((temp * alpha) + beta > 255)
		return 255;
	else 
		return (pixel * alpha) + beta; 
}

//This function change the contrast (alpha) and brightness (beta) to a some area of image. The corner are defined in srcQuad
//It works but has some strange behavious. To fix
void change_brightness_from_to(CvMat* image, int alpha, int beta, CvPoint2D32f* srcQuad){
	
	CvMat *new_image =cvCloneMat(image);
	uchar pixval;
	int x, y, c;
	int p[3];
	new_image->cols *= 3; 
	image->cols *= 3;

	//fprintf(stderr,"Top left [%d][%d], Top right [%d][%d], bottom left [%d][%d]\n",srcQuad[0].x,srcQuad[0].y,srcQuad[1].x, srcQuad[1].y,  srcQuad[2].x, srcQuad[2].y); 
	 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	 for( y = (int)srcQuad[0].y; y < (int)srcQuad[2].y;  y++ ){//From top left to bottom left
	 	for( x = (int)srcQuad[0].x; x < (int)srcQuad[1].x; x++ ){//From top left to top right
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

	 p[0]=CV_IMWRITE_JPEG_QUALITY;
	 p[1]=JPEG_QUALITY;
	 p[2]=0;
	 cvSaveImage("result2.jpg",new_image, p);

	 /// Wait until user press some key
	cvReleaseMat(&new_image);
	cvWaitKey(0);
	cvDestroyAllWindows();
}

//This function changes the brightness and the constrast to ALL image
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
	 CvMat *image_bn=cvLoadImageM(argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	if(image==NULL | image_bn==NULL){
	fprintf(stderr, "No input image!\n");
	exit(0);
	}
	CvPoint2D32f srcQuad[4]; //Source corners.
	srcQuad[0].x = 284;   //src Top left
    srcQuad[0].y = 106;

    srcQuad[1].x = 396;   //src Top right
    srcQuad[1].y = 106;

    srcQuad[2].x = 284;   //src Bottom left
    srcQuad[2].y = 177;

    srcQuad[3].x = 620;   //src Bottom right
    srcQuad[3].y = 177;
	getBeta(image_bn,srcQuad);

	//divide_in_blocks(image,10);//This function gets the (n+1)^2 coordinates of the n^2 blocks
	 /// Initialize values
	 printf(" Basic Linear Transforms\n");
	 printf("-------------------------\n");
	 printf("* Enter the alpha value [1.0-3.0]: ");
	 scanf("%i", &alpha);
	 printf("* Enter the beta value [0-100]: ");
	 getchar(); 		//Ignore the '\n' 
	 scanf("%i", &beta);
	
	 //change_brightness(image, alpha, beta);//Change the brightness of all image

	 return 0;
}
