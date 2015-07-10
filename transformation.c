#include "transformation.h"

//Input image, Edges, outputNameFile
int homography_transformation(CvMat* src, char* out_filename, CvPoint2D32f* srcQuad){
	if(src == NULL || out_filename == NULL || srcQuad == NULL)
		return -1;
	
	CvPoint2D32f dstQuad[4]; 				//Destination vertices
   	CvMat* warp_matrix = cvCreateMat(3,3,CV_32FC1);  	//transformation matrix
    	CvMat* dst = cvCloneMat(src);   			//clone image  
    	int p[3]={CV_IMWRITE_JPEG_QUALITY,JPEG_QUALITY,0};	//FORMAT, QUALITY
    
    	dstQuad[0].x = 0;       	//dst Top left
    	dstQuad[0].y = 0;
    	dstQuad[1].x = src->cols;  	//dst Top right
    	dstQuad[1].y = 0;
    	dstQuad[2].x = 0;     		//dst Bottom left
    	dstQuad[2].y = src->rows;      
    	dstQuad[3].x = src->cols;  	//dst Bot right
    	dstQuad[3].y = src->rows;

    	// get transformation matrix that you can use to calculate 
    	cvGetPerspectiveTransform(srcQuad,dstQuad, warp_matrix);
    	// perspective transformation. Parameters: source, destination, warp_matrix, 
	//type of interpolation: (CV_INTER_LINEAR, CV_INTER_AREA, CV_INTER_CUBIC, CV_INTER_LANCZOS4)
  	///Set all scalar with the same value. 0 means the black color of border
	cvWarpPerspective(src, dst, warp_matrix, CV_INTER_LINEAR, cvScalarAll(0)); // 1 = CV_INTER_LINEAR

	cvSaveImage(out_filename,dst, p);
	//close and destroy all stuff

	cvReleaseMat(&warp_matrix);
	cvReleaseMat(&dst);
	return 0;
}
