#include"circles.h"

//Simply it checks if the numbers in input giving by the command line, are between 1 and 100.
//If yes returns 1 otherwise -1.
int check_bounds(char* bound_char, int *a){
	*a=atoi(bound_char);
	
	if(*a>0 && *a<=100) return 1;
	else{ 
		return -1;
		a=NULL;		
		}
}

//Giving bt command line the file name, this functions get the image contained in.
CvMat *get_img(int argc, char **argv){

	CvMat *src=NULL;
	char *in_file;

	if(argc<2){
		fprintf(stderr, "Error: too few argument. Insert an input_file\n");
		return NULL;
		}
	
	in_file =  argv[1];
	src=cvLoadImageM( argv[1], 1);

	if(src==NULL){
		fprintf(stderr,"%s input file not present\n",in_file);
		return NULL;
		}
	return src;
}


//Input: argc and argv.
//Output: Get the position of where the function is going to draw the circles.
CvPoint2D32f* get_positions_circles(int argc, char **argv, CvMat *img){

	CvPoint2D32f *pos_circles=calloc(N_CIRCLES,sizeof(CvPoint2D32f));
	//char* in_file=NULL;
	int height_img;
	int width_img;
	int x_tmp=0;
	int y_tmp=0;
	int i;
		
	if(argc<9){//check on argc
		fprintf(stderr, "Error: too few argument. You have to insert:\ninput namefile\nOther stuff to specify..\n");
		return NULL;
	}
	
	height_img=img->rows;
	width_img=img->cols;
	
	for(i=2; i<10; i+=2){
		if((check_bounds(argv[i],&x_tmp)==1) && (check_bounds(argv[i+1],&y_tmp)==1)) {
			pos_circles[i/2-1].x=(x_tmp*width_img)/100;
			pos_circles[i/2-1].y=(y_tmp*height_img)/100;
		} else {
			fprintf(stderr,"Error: insert number between 1 and 100\n");
			return NULL;
		}
	}

	/*DEBUG PRINT IN ORDER TO VERIFT THE COORDINATES IN WHICH THE CIRCLES WILL BE DRAWN
	printf("Resolution image:[%d]x[%d]\nTo draw circles in this position\n", width_img, height_img);
	for(i=0; i<4; i++){
		printf("[%d][%d]\t",(int)pos_circles[i].x,(int)pos_circles[i].y);
	}printf("\n");
	*/

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
	cvNamedWindow( "Preview", CV_WINDOW_NORMAL );
	cvShowImage( "Preview", img );
	cvWaitKey(0);
	cvDestroyWindow("Preview" );
}

/*
int main(int argc, char**argv){
	CvMat *img =get_img(argc, argv);
	if(img==NULL) 
		exit(0);
		
	CvPoint2D32f *pos_circles=get_positions_circles(argc,argv,img);
	if(pos_circles==NULL) 
		exit(0);
	
	draw_circles(pos_circles,img);
	view_preview(img);
	return 0;
}*/
