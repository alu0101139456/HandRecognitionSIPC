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
	Mat frame,roi,fgMask;
	VideoCapture cap;
	cap.open(0);

  Ptr<BackgroundSubtractor> pBackSub=createBackgroundSubtractorMOG2();

	if (!cap.isOpened()) {
		printf("Error opening cam\n");
		return -1;
	}
	namedWindow("Frame");
	namedWindow("ROI");
	namedWindow("Foreground Mask");


	Rect rect(400,100,200,200);	
	bool st = true;
	while(true) {

		cap>>frame;
		flip(frame,frame,1);

		frame(rect).copyTo(roi);
		if (st) {
			pBackSub->apply(roi,fgMask);
		}//roi = frame(rect);
		
		rectangle(frame, rect,Scalar(255,0,0));

		imshow("Frame",frame);
		imshow("ROI",roi);
		imshow("Foreground Mask", fgMask);

		int c = waitKey(40);

		if ((char)c =='q') break;
		if ((char)c =='s') st = false;
	}
	cap.release();
	destroyAllWindows();
}
