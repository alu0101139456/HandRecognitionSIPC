#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>

#include<string>
#include "captureHand.hpp"


int main(int argc, char* argv[])
{
	CaptureHand capture(400,100,200,200, true);
	cv::Mat frame;
	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened()) {
		std::cerr << "No se puede abrir camara" << std::endl;
		return -1;
	}
	cv::namedWindow("Frame");
	cv::namedWindow("ROI");
	
	int lr = -1;
	while(true) {

		cap>>frame; 		
		capture.SetFrame(frame);
		cv::imshow("Frame",frame);
		cv::imshow("ROI",capture.GetResult());

		int c = cv::waitKey(40);

		if ((char)c =='q') break;
		if ((char)c =='l') capture.StopLearningRate();  //Se usa para dejar de "aprender" el fondo cambiando el valor del LearningRate

	}
	cap.release();
	cv::destroyAllWindows();

}