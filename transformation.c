#include "transformation.h"


void homography_transformation(IplImage* src, CvPoint2D32f* srcQuad,char* out_filename){//Input image, Edges, outputNameFile
	CvPoint2D32f dstQuad[4]; //Destination matrix
    CvMat* warp_matrix = cvCreateMat(3,3,CV_32FC1);     // transformation matrix
    IplImage* dst = cvCloneImage(src);   // clone image  
    
    dstQuad[0].x = 0;       //dst Top left
    dstQuad[0].y = 0;
    dstQuad[1].x = src->width-1;  //dst Top right
    dstQuad[1].y = 0;
    dstQuad[2].x = 0;     //dst Bottom left
    dstQuad[2].y = src->height-1;      
    dstQuad[3].x = src->width-1;  //dst Bot right
    dstQuad[3].y = src->height-1;

    // get transformation matrix that you can use to calculate 
    cvGetPerspectiveTransform(srcQuad,dstQuad, warp_matrix);
    // perspective transformation. Parameters: source, destination, warp_matrix, 
	//type of interpolation: (CV_INTER_LINEAR, CV_INTER_AREA, CV_INTER_CUBIC, CV_INTER_LANCZOS4)
  	///Set all scalar with the same value. 0 means the black color of border
	cvWarpPerspective(src, dst, warp_matrix, 1, cvScalarAll(0)); // 1 = CV_INTER_LINEAR
	cvNamedWindow( out_filename, 1 );//Window name and autosize YES (1)
	int p[3]={CV_IMWRITE_JPEG_QUALITY,JPEG_QUALITY,0};//FORMAT, QUALITY, BOH

	cvSaveImage(out_filename,dst, p);
	//close and destroy all stuff

	cvReleaseMat(&warp_matrix);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}

int homography( char* input, char* output, CvPoint2D32f* srcQuad ) {
	IplImage *src = cvLoadImage(input,1);
	
	//Check on src
	if(input == NULL){
		fprintf(stderr,"input file not present\n");
		return 1;
	}
	
	if(output == NULL){
		output = (char*)malloc(sizeof("output.jpg")+1);
		output = "output.jpg";
	}
	
	if(srcQuad == NULL){
		fprintf(stderr,"source matrix not present\n");
		return 1;
	}

	homography_transformation(src,srcQuad,output);
	
	return 0;
}
