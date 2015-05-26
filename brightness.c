#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>


#define N_BLOCKS 10
#define JPEG_QUALITY 100

void print_block_coordinates(CvPoint2D32f* srcQuad){
fprintf(stderr,"Block coordinates:\nTopLeft[%d][%d]	TopRight[%d][%d]\nBottomLeft[%d][%d]	BottomRight[%d][%d]\n",(int)srcQuad[0].x, (int)srcQuad[0].y, (int)srcQuad[1].x, (int)srcQuad[1].y, (int)srcQuad[2].x, (int)srcQuad[2].y, (int)srcQuad[1].x, (int)srcQuad[2].y);
}


//Given a certain block number, it return its coordinates.
//The function need all the coordinates of the all blocks, which block where get the info and how many are the blocks.
CvPoint2D32f* getBlock(CvPoint2D32f* all_blocks_coordinates, int which_block, int how_many_block){
	CvPoint2D32f my_block_coordinates[3];
	
	my_block_coordinates[0].x=all_blocks_coordinates[which_block-1].x;//Top left
	my_block_coordinates[0].y=all_blocks_coordinates[which_block-1].y;
	
	my_block_coordinates[1].x=all_blocks_coordinates[which_block].x;//Top right
	my_block_coordinates[1].y=all_blocks_coordinates[which_block].y;

	my_block_coordinates[2].x=all_blocks_coordinates[which_block+how_many_block].x;//Bottom left
	my_block_coordinates[2].y=all_blocks_coordinates[which_block+how_many_block].y;
	print_block_coordinates(my_block_coordinates);
	return my_block_coordinates;
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
	int n_coordinates=(int)pow(n+1,2);//
	CvPoint2D32f all_blocks_coordinates[n_coordinates];
	int step_width=image->cols/n;
	int step_height=image->rows/n;
//The width and the height not always are multiple of n. The change (Il resto) of pixel will be spreaded over the block (x and y)
	int change_width=image->cols%n;
	int change_height=image->rows%n;
	//The image will have (n+1)^2 blocks. Each block will have the following dimension: step_width (+ change_width) x step_height (+ change_height)
	int x,y;
	int start_x=0;
	int start_y=0;
	int k=0;
	for(x=0; x<=n; x++){//Row for cycle
		int change_width_tmp=change_width;
		for(y=0; y<=n; y++){//Column for cycle
			all_blocks_coordinates[k].x=start_x;
			all_blocks_coordinates[k].y=start_y;
			k++;
			start_x+=step_width;
			if(change_width_tmp>0){ 
				start_x++;;
				change_width_tmp--;
				}
			}
			start_y+=+step_height;
			if(change_height>0){
				start_y++;
				change_height--;
			}
			start_x=0;
	}
int which_block=7;
getBlock(all_blocks_coordinates,which_block,n);
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
	//getBeta(image_bn,srcQuad);

	divide_in_blocks(image,N_BLOCKS);//This function gets the (n+1)^2 coordinates of the n^2 blocks
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
