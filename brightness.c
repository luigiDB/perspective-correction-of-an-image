#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>


#define N_BLOCKS 10
#define JPEG_QUALITY 50

const int PARAMETERS[3]={CV_IMWRITE_JPEG_QUALITY,JPEG_QUALITY,0}; 

//Giving the coordinates in inputs, this functions prints them out
void print_block_coordinates(CvPoint2D32f* srcQuad){
fprintf(stderr,"Block coordinates:\nTopLeft[%d][%d]	TopRight[%d][%d]\nBottomLeft[%d][%d]	BottomRight[%d][%d]\n",(int)srcQuad[0].x, (int)srcQuad[0].y, (int)srcQuad[1].x, (int)srcQuad[1].y, (int)srcQuad[2].x, (int)srcQuad[2].y, (int)srcQuad[1].x, (int)srcQuad[2].y);
}


//Given a certain block number, it return its coordinates.
//The function need all the coordinates of the all blocks, which block where get the info and how many are the blocks.
CvPoint2D32f* get_block_coordinates(CvPoint2D32f* all_blocks_coordinates, int which_block, int n){
	CvPoint2D32f* my_block_coordinates=(CvPoint2D32f*)calloc(3,sizeof(CvPoint2D32f));
	int x;
	if(which_block>(int)pow(n,2) | which_block<1)
	{
	fprintf(stderr,"This block doesn't exist\n");
	return NULL;
	}

	
	if(which_block%n==0) x=2;
	else x=1;

	int top_left_coordinate=which_block+which_block/n-x;
	my_block_coordinates[0].x=all_blocks_coordinates[top_left_coordinate].x;//Top left
	my_block_coordinates[0].y=all_blocks_coordinates[top_left_coordinate].y;

	my_block_coordinates[1].x=all_blocks_coordinates[top_left_coordinate+1].x;//Top right
	my_block_coordinates[1].y=all_blocks_coordinates[top_left_coordinate+1].y;

	my_block_coordinates[2].x=all_blocks_coordinates[top_left_coordinate+n+1].x;//Bottom left
	my_block_coordinates[2].y=all_blocks_coordinates[top_left_coordinate+n+1].y;
	return my_block_coordinates;
}

//Get the average brightness from an (sub)image given the coordinates.
//The image is passed to the fuction must be in grayscale. 
int get_beta(CvMat* image, CvPoint2D32f* srcQuad){
	unsigned int beta, x, y;
	unsigned int height_block=srcQuad[2].y-srcQuad[0].y;
	unsigned int width_block=srcQuad[1].x-srcQuad[0].x;
	unsigned int resolution_block=height_block*width_block;
	unsigned int tmp=0;

	for(y=srcQuad[0].y; y<srcQuad[2].y; y++){
		for(x=srcQuad[0].x; x<srcQuad[1].x; x++){
	tmp+=CV_MAT_ELEM(*image, uchar, y, x);
		}
	}
	beta=tmp/resolution_block;
	return beta;
}

//This function gets all the blocks coordinates
CvPoint2D32f* get_all_coordinates(CvMat* image, int n){
	if(n<2) return;
	int n_coordinates=(int)pow(n+1,2);
	CvPoint2D32f* all_blocks_coordinates=(CvPoint2D32f*)calloc(n_coordinates,sizeof(CvPoint2D32f));
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

	return all_blocks_coordinates;
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
//Some problem with number of columns
CvMat* change_brightness_single_block(CvMat* image, int alpha, int beta, CvPoint2D32f* srcQuad){
	
	CvMat *new_image =cvCloneMat(image);
	uchar pixval;
	int x, y, c;

	new_image->cols *= 3; 
	image->cols *= 3;
	int top_right=(int)srcQuad[1].x*3;//I must 
	int top_left=(int)srcQuad[0].x*3;
	 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	 for( y = (int)srcQuad[0].y; y < (int)srcQuad[2].y;  y++ ){
	 	for( x = top_left; x <top_right; x++ ){//FROM Top left TO Top Right
	 		//leggo il valore del pixel dell'immagine originale
			pixval = CV_MAT_ELEM(*image, uchar, y, x);
			//ci applico la formula alpha * x + beta
			pixval = saturate(pixval, alpha, beta);
			//scrivo il valore nella solita posizione della nuova immagine
			CV_MAT_ELEM(*new_image, uchar, y, x) = pixval;
	    	}
	 }

	 //Rimetto il numero di colonne al loro valore originale, altrimenti non si riesce a visualizzare l'immagine
	 image->cols /= 3;
	new_image->cols /= 3; 
	return new_image;
}

//This function changes the brightness and the constrast to ALL image
void change_brightness(CvMat* image, int alpha, int beta){//Alpha between 1 and 3, beta between 1 and 100
	

	CvMat *new_image =cvCloneMat(image);
	uchar pixval;
	int x, y, c;
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
	
	 cvSaveImage("result.jpg",new_image, PARAMETERS);

	 /// Wait until user press some key
	cvReleaseMat(&new_image);
	cvWaitKey(0);
	cvDestroyAllWindows();
}


int main( int argc, char** argv )
{

	//Input values: Aplha, Beta, N_BLOCKS (FROM 1 (All image) to N (N^2 small blocks))
	 int alpha; /**< Simple contrast control */
	 int beta;  /**< Simple brightness control */
	 uchar pixval;
	 int x, y, c,i;
	CvMat *image = cvLoadImageM( argv[1], 1);
	CvMat *image_bn=cvLoadImageM(argv[1], CV_LOAD_IMAGE_GRAYSCALE );//Load the image in grayscale version.


	CvPoint2D32f* all_blocks_coordinates=NULL;
	if(image==NULL | image_bn==NULL){
	fprintf(stderr, "No input image!\n");
	exit(0);
	}
	CvMat *new_image =cvCloneMat(image);
	all_blocks_coordinates=(CvPoint2D32f*)calloc((int)pow(N_BLOCKS+1,2),sizeof(CvPoint2D32f));
	all_blocks_coordinates=get_all_coordinates(new_image,N_BLOCKS);//This function gets the (n+1)^2 coordinates of the n^2 blocks

	/*for(i=0; i<100; i++){
	//It works ma è pesante
	new_image=change_brightness_single_block(new_image,3, i, get_block_coordinates(all_blocks_coordinates,i+1,N_BLOCKS));
	}*///cvSaveImage("birghtnessFuzzy.jpg",new_image, PARAMETERS);

	//The image is passed to the fuction must be in grayscale. 
	for(i=0; i<100; i++){
	printf("Block n° %d. Average brightness:%d\n",i+1,get_beta(image_bn, get_block_coordinates(all_blocks_coordinates,i+1,N_BLOCKS)));
	}
	//Free stuff
	cvReleaseMat(&new_image);
	cvReleaseMat(&image);
	cvReleaseMat(&image_bn);
	free(all_blocks_coordinates);
	 return 0;
}
