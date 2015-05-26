/*
2nd Project Advanced Topics in Computer Systems and Network.
I give in input these parameters: input file (edge ?) output file.

If the inpunt file is not specified the source code get for default the inputfile.jpg. 

If the file it doesn't exist will appeare an error
The default output file is  output.jpg.
It will be shown.

In order to compile you have to write this on the console:
gcc transformation.c `pkg-config --cflags --libs opencv`

Where transformation.c is the name of this source code.
If it doesn't compile check if the libraries are installed.

In order to execute:
/a.out RMR5y.png ciao.jpg

Where RMR5y.png is the input file and ciao.jpg is the output file.
*/

#include <opencv/highgui.h>
#include<opencv/cv.h>
#include <stdlib.h>
#include <stdio.h>

#define AUTO_SIZE_YES 1
#define JPEG_QUALITY 80
void homography_transformation(IplImage* src, CvPoint2D32f* srcQuad,char* out_filename){//Input image, Edges, outputNameFile
	CvPoint2D32f dstQuad[4]; //Destination matrix
    CvMat* warp_matrix = cvCreateMat(3,3,CV_32FC1);     // transformation matrix
    IplImage* dst = cvCloneImage(src);   // clone image  
	srcQuad[0].x = 1290;   //src Top left
	srcQuad[0].y = 1430;

	srcQuad[1].x = 3050;   //src Top right
	srcQuad[1].y = 1430;

	srcQuad[2].x = 77;   //src Bottom left
	srcQuad[2].y = 3200;

	srcQuad[3].x = 4326;   //src Bottom right
	srcQuad[3].y = 3200;

	    //coordinates of rectangle in src image 
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
	cvWarpPerspective(src, dst, warp_matrix, CV_INTER_LINEAR, cvScalarAll(0));
	cvNamedWindow( out_filename, 1 );//Window name and autosize YES (1)
	cvShowImage( out_filename, dst ); //Show the dst image on window
int p[3]={CV_IMWRITE_JPEG_QUALITY,JPEG_QUALITY,0};//FORMAT, QUALITY, BOH

	cvSaveImage(out_filename,dst, p);
//close and destroy all stuff
	cvWaitKey(0);

	cvReleaseMat(&warp_matrix);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	cvDestroyAllWindows();
}

int main( int argc, char** argv ) {
IplImage *dst=NULL;
char* filename = argc >= 2 ? argv[1] : "Input.jpg";//Se il secondo argomento non è specificato, prende di default Input.jpg
char* out_filename= argc ==3 ? argv[2] : "output.jpg"; //Se Il terzo argomento non è specificato, come nome dell'output mette output.jpg
IplImage *src = cvLoadImage(filename,1);
//Check on src
if(src==NULL){
fprintf(stderr,"%s input file not present\n",filename);
exit(0);
}

    CvPoint2D32f srcQuad[4]; //Source corners.
//Corner of trapezoid. Arbitrary values.

    srcQuad[0].x = 284;   //src Top left
    srcQuad[0].y = 106;

    srcQuad[1].x = 396;   //src Top right
    srcQuad[1].y = 107;

    srcQuad[2].x = 77;   //src Bottom left
    srcQuad[2].y = 177;

    srcQuad[3].x = 620;   //src Bottom right
    srcQuad[3].y = 177;

homography_transformation(src,srcQuad,out_filename);
}
