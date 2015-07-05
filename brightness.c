#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>


#define N_BLOCKS 10	//This variable will be an input (?)
#define JPEG_QUALITY 50

//This function prints out the info about the image which is passed as parameter
void img_info(CvMat* img){
//fprintf(stderr,"Resolution:%dx%d\n",img->width,img->height);
//All other stuff to put here
}

//Constant array used for save image
const int PARAMETERS[3]={CV_IMWRITE_JPEG_QUALITY,JPEG_QUALITY,0}; //Format, quality (DEFAULT is 95) and

//Giving the coordinates in inputs, this functions prints them out
void print_block_coordinates(CvPoint2D32f* srcQuad){
	fprintf(stderr,"Block coordinates:\nTopLeft[%d][%d]	TopRight[%d][%d]\nBottomLeft[%d][%d]	BottomRight[%d][%d]\n", \
	(int)srcQuad[0].x, (int)srcQuad[0].y, 	(int)srcQuad[1].x, (int)srcQuad[1].y, (int)srcQuad[2].x, (int)srcQuad[2].y, (int)srcQuad[1].x, (int)srcQuad[2].y);
}


//Giving a block number, this function gets as a matrix format its neighboors block
int** get_neighboor(int which_block, int n){
	int i,j;
	int tot_blocks=(int)pow(n,2);

	int** first_lev=(int**)calloc(3,sizeof(int));
	for(i=0; i<3; i++)	
		first_lev[i]=(int*)calloc(3,sizeof(int));//It's a 3x3 matrix

	first_lev[1][1]=which_block;

	if((which_block>tot_blocks) | (which_block<1)){//1st check on block
		fprintf(stderr,"This block doesn't exist\n");
		return NULL;
		}

//2nd check on neighboor blocks
//They don't go outside the bound
	if(which_block-n-1>0) first_lev[0][0]=which_block-n-1;
	if(which_block-n>0)first_lev[0][1]=which_block-n;
	if(which_block-n+1>0)first_lev[0][2]=which_block-n+1;

	if(which_block-1>0) first_lev[1][0]=which_block-1;

	if(which_block+1<=tot_blocks)first_lev[1][2]=which_block+1;

	if(which_block+n-1<=tot_blocks) first_lev[2][0]=which_block+n-1;
	if(which_block+n<=tot_blocks)first_lev[2][1]=which_block+n;
	if(which_block+n+1<=tot_blocks)first_lev[2][2]=which_block+n+1;


	if((which_block-1)%n==0){//It is left border?
		for(i=0; i<3; i++){
		first_lev[i][0]=0;//I set to zero the element left to left border because they aren't present
		}
	}

	if(which_block%n==0){//Is right border?
		for(i=0; i<3; i++){
		first_lev[i][2]=0;//I set to zero the element left to left border because they aren't present
		}
	}
	//Print all the neighboor
	/*for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			printf("[%d]",first_lev[i][j]);
			}
		printf("\n");
		}
	printf("\n\n");*/

	return first_lev;
}

//Given a certain block number, it return its coordinates.
//The function needs:
//1. All the coordinates of the all blocks
//2. Which block from to extract the coordinates
//3. N, the N_BLOCK constant
CvPoint2D32f* get_block_coordinates(CvPoint2D32f* all_blocks_coordinates, int which_block, int n){

	CvPoint2D32f* my_block_coordinates=(CvPoint2D32f*)calloc(3,sizeof(CvPoint2D32f));
	int x;
	if((which_block>(int)pow(n,2)) | (which_block<1))
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

//Get the average brightness from an (sub)image given its coordinates.
//The image is passed to the fuction must be in YUV encoding. 
int get_avg_brightness_block(CvMat* image, CvPoint2D32f* srcQuad){
	unsigned int avg_brightness, x, y;
	unsigned int height_block=srcQuad[2].y-srcQuad[0].y;
	unsigned int width_block=srcQuad[1].x-srcQuad[0].x;
	unsigned int resolution_block=height_block*width_block;
	unsigned int tmp=0;

	CvMat *dst=cvCloneMat(image);
	dst->cols *= 3; 
	image->cols *= 3;
	int top_left=(int)srcQuad[0].x*3;
	int top_right=(int)srcQuad[1].x*3;
	

	for(y=srcQuad[0].y; y<srcQuad[2].y; y++){
		for(x=top_left; x<top_right; x+=3){//The Y (brightness) coordinates are 0,3,6,9,...,3*x
			tmp+=CV_MAT_ELEM(*dst, uchar, y, x);
			}		
	}

	dst->cols /= 3; 
	image->cols /= 3;
	avg_brightness=tmp/resolution_block;
	return avg_brightness;
}

//Conversion to RGB encoding to YUV
CvMat* rgb2yuv(CvMat* image){
	CvMat *dst=cvCloneMat(image);
	cvCvtColor(image,dst,CV_RGB2YCrCb);	
	//cvSaveImage("yuv.jpg",dst, PARAMETERS);//Optional save of image
	return dst;
}
//Conversion to YUV encoding to RGB
CvMat* yuv2rgb(CvMat* image){
	CvMat *dst=cvCloneMat(image);
	cvCvtColor(image,dst,CV_YCrCb2RGB);	
	//cvSaveImage("yuv.jpg",dst, PARAMETERS);//Optional save of image
	return dst;
}

//This function gets all the blocks coordinates
CvPoint2D32f* get_all_coordinates(CvMat* image, int n){
	if(n<2) return NULL;
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
	unsigned int temp = (unsigned int)pixel;
	if((temp * alpha) + beta > 255)
		return 255;
	else 
		return (pixel * alpha) + beta; 
}

//This function change the contrast (alpha) and brightness (beta) to a some area of image. The corner are defined in srcQuade
//The image must be in YUV encoding and I have to modify the Y channel

int change_brightness_single_block(CvMat* image_yuv, int alpha, int beta, CvPoint2D32f* srcQuad){
	
	uchar pixval;
	int x, y;

	image_yuv->cols *= 3;
	int top_right=(int)srcQuad[1].x*3;//I must 
	int top_left=(int)srcQuad[0].x*3;
	 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	 for( y = (int)srcQuad[0].y; y < (int)srcQuad[2].y;  y++ ){
	 	for( x = top_left; x <top_right; x+=3 ){//FROM Top left TO Top Right
	 		//leggo il valore del pixel dell'immagine originale
			pixval = CV_MAT_ELEM(*image_yuv, uchar, y, x);
			//ci applico la formula alpha * x + beta
			pixval = saturate(pixval, alpha, beta);
			//scrivo il valore nella solita posizione della nuova immagine
			CV_MAT_ELEM(*image_yuv, uchar, y, x) = pixval;
	    	}
	 }

	 //Rimetto il numero di colonne al loro valore originale, altrimenti non si riesce a visualizzare l'immagine
	image_yuv->cols /= 3;
	return 1;
}

//This function changes the brightness and the constrast of ALL image
void change_brightness(CvMat* image, int alpha, int beta){//Alpha between 1 and 3, beta between 1 and 100
	

	CvMat *new_image =cvCloneMat(image);
	uchar pixval;
	int x, y;
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
	
	 cvSaveImage("changeBrightness.jpg",new_image, PARAMETERS);

	 /// Wait until user press some key
	cvReleaseMat(&new_image);
	cvWaitKey(0);
	cvDestroyAllWindows();
}


/*
* Input: YUV image, all blocks coordinates and the number of blocks
* Output: An array, for each block the respective array element contains the average birghtness
*/
int* compute_bright_vector(CvMat* image_yuv, CvPoint2D32f* all_blocks_coordinates, int num_of_blocks){
	int* vector;
	int i;
	CvPoint2D32f* temp_coord;
	
	if(image_yuv == NULL || all_blocks_coordinates == NULL || num_of_blocks == 0)
		return NULL;
	
	vector = malloc(sizeof(int) * num_of_blocks * num_of_blocks);
	
	for(i = 1; i <= num_of_blocks * num_of_blocks; i++){
		temp_coord = get_block_coordinates(all_blocks_coordinates, i, num_of_blocks);
		vector[i-1] = get_avg_brightness_block(image_yuv, temp_coord);
		free(temp_coord);
	}
	
	return vector;
}

/*
* Input: image_yuv, which_block, brightness vector,coordinates_matrix 
* Output: 1 or -1 if error
*/
int bright_corrections(CvMat* image_yuv, CvPoint2D32f* all_blocks_coordinates, int* bright_vector, int which_block, int num_of_blocks){
	int** neighbors;
	int bright_avg = 0;
	int i, j;
	int num_of_neighbors = 0;
	CvPoint2D32f *central_block_coord;
	int temp;
	int old_block_brightness;
	int beta;
	
	if(image_yuv == NULL || all_blocks_coordinates == NULL || which_block == 0 || num_of_blocks == 0)
		return -1;
	
	/*Gets the IDs of neighbor blocks*/
	neighbors = get_neighboor(which_block, num_of_blocks);
	if(neighbors == NULL)
		return -1;
		
	central_block_coord = get_block_coordinates(all_blocks_coordinates, which_block, num_of_blocks);
	old_block_brightness = bright_vector[which_block];
	
	/*For each ID, computes the relative block's brightness and sums it in a variable*/
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){				//Note that in neighbors[1][1] there is the central block ID.
			if(neighbors[i][j] == 0) continue;
			
			temp = bright_vector[neighbors[i][j]-1];
				
			bright_avg += temp;
			num_of_neighbors++;
			
		}
	}
	/*At the end computes the average value*/
	bright_avg /= num_of_neighbors;
	/*In order to apply the brightness correction to the central block we need the "beta" value*/
	beta = bright_avg - old_block_brightness;
	
	temp = change_brightness_single_block(image_yuv, 1, beta, central_block_coord);
	
	free(central_block_coord);
	free(neighbors);
	return (temp == -1) ? -1 : 1;
}


//return 0 if not present 
int check_presence(int* array, int value, int max_index){
	//printf("check %i\n", value);
	int i;
	for(i=0 ; i <= max_index ; i++){
		if( array[i] == value )
			return 1;
	}
	return 0;
}

/*
* Input: Num_blocks, starting_block
* Output: Array_Indexes_block
*/
int* select_correction_order(int N, int start){
	//politica di aggiornamento
	//	scrivo e aumento index
	
	int* results = (int*)malloc(sizeof(int)*N*N);
	int index = 0;
	int** neighboor;
	
	results[index] = start;
	index++;
	
	int i, j, k;
	for(i=0 ; i < N*N ; i++){
		//printf("a");
		neighboor = get_neighboor(results[i], N);
		if(neighboor == NULL){
			return NULL;
		}
		for(j=0 ; j<3 ; j++){
			for(k=0 ; k<3 ; k++){
				if (check_presence(results, neighboor[j][k], index) == 0 && neighboor[j][k] != 0){
					results[index] = neighboor[j][k];
					index++;
				}
			}
		}
		
		free(neighboor);
	}
	
	return results;
}


int main( int argc, char** argv )
{

	//Input values: Aplha, Beta, N_BLOCKS (FROM 1 (All image) to N (N^2 smaller blocks))
	 //int alpha; /**< Simple contrast control */
	// int beta;  /**< Simple brightness control */
	CvMat *image = cvLoadImageM( argv[1], 1);
	CvPoint2D32f* all_blocks_coordinates=NULL;
	int* brightness_vector;
	int* block_order;
	int i;
	int max_bright = 0, block_index;
	int p[3];
	
	if(image==NULL){
	fprintf(stderr, "No input image!\n");
	exit(0);
	}
	//int tot_blocks=(int)pow(N_BLOCKS,2);
	CvMat *new_image =cvCloneMat(image);
	CvMat* image_yuv=rgb2yuv(image);
	CvMat *result;

	all_blocks_coordinates=(CvPoint2D32f*)calloc((int)pow(N_BLOCKS+1,2),sizeof(CvPoint2D32f));
	all_blocks_coordinates=get_all_coordinates(new_image,N_BLOCKS);//This function gets the (n+1)^2 coordinates of the n^2 blocks
	
	brightness_vector = compute_bright_vector(image_yuv,  all_blocks_coordinates, N_BLOCKS);
	
	for(i = 0; i < N_BLOCKS * N_BLOCKS; i++){
		printf("BEFORE: Brightness of block %i = %i\n", i+1, brightness_vector[i]);
		if(brightness_vector[i] > max_bright){
			max_bright = brightness_vector[i];
			block_index = i+1;
		}

	}
	block_order = select_correction_order(N_BLOCKS, block_index);
	
	for(i = 0; i < N_BLOCKS * N_BLOCKS; i++){
		printf("%i ", block_order[i]);
		bright_corrections(image_yuv, all_blocks_coordinates, brightness_vector, block_order[i], N_BLOCKS);
	}
	
	free(brightness_vector);
	brightness_vector = compute_bright_vector(image_yuv,  all_blocks_coordinates, N_BLOCKS);
	
	for(i = 0; i < N_BLOCKS * N_BLOCKS; i++){
		printf("AFTER: Brightness of block %i = %i\n", i+1, brightness_vector[i]);
	}
	
	result = yuv2rgb(image_yuv);
	
	p[0]=CV_IMWRITE_JPEG_QUALITY;
	p[1]=100;
	p[2]=0;
	cvSaveImage("result.jpg",result, p);
	
	free(brightness_vector);
	free(block_order);
	//Free stuff
	cvReleaseMat(&new_image);
	cvReleaseMat(&image);
	cvReleaseMat(&image_yuv);
	cvReleaseMat(&result);
	free(all_blocks_coordinates);
	return 0;
}
