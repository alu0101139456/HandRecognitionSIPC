#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
	Mat frame,roi;
	VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened()) {
		printf("Error opening cam\n");
		return -1;
	}
	namedWindow("Frame");
	namedWindow("ROI");

	Rect rect(400,100,200,200);	
	while(true) {

		cap>>frame;
		flip(frame,frame,1);

		frame(rect).copyTo(roi);
		//roi = frame(rect);
		
		rectangle(frame, rect,Scalar(255,0,0));

		imshow("Frame",frame);
		imshow("ROI",roi);

		int c = waitKey(40);

		if ((char)c =='q') break;
	}
	cap.release();
	destroyAllWindows();
}
