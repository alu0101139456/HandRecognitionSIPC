#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>




int main(int argc, char* argv[])
{
	cv::Mat frame, roi, fgMask,gray;
	cv::VideoCapture cap;
	cap.open(0);

  cv::Ptr<cv::BackgroundSubtractor> pBackSub = cv::createBackgroundSubtractorMOG2();
	std::vector<std::vector<cv::Point> > contours;

	if (!cap.isOpened()) {
		std::cerr << "No se puede abrir camara" << std::endl;
		return -1;
	}
	cv::namedWindow("Frame");
	cv::namedWindow("ROI");
	cv::namedWindow("Foreground Mask");

	cv::Rect rect(400,100,200,200);	

int max = 0;
	double lr = -1;
	while(true) {

		cap>>frame; //Capturo las imagenes al objeto FRAME
		flip(frame,frame,1); //Rota Horizontalmente para efecto espejo

		frame(rect).copyTo(roi); //Copio en roi el recuadro que usaremos
		cv::Point centro(10, 10);
		
		cv::cvtColor(roi, gray, cv::COLOR_RGB2GRAY); //Aplicamos a roi escala de grises

		pBackSub->apply(gray,fgMask,lr); //aplico a roi la substracción de fondo
				
		rectangle(frame, rect,cv::Scalar(255,0,0)); //Dibujo en FRAME el rectanglo
		
		
		cv::threshold(gray, gray, 127, 255, cv::THRESH_BINARY); //sólo se usa para el tutorial

		cv::findContours(gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	  cv::drawContours(roi, contours, -1, cv::Scalar(0,255,0),3);
		
		std::vector<std::vector<cv::Point> > hull(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
			 cv::convexHull(contours[i], hull[i]);
		// cv::drawContours(roi, hull, -1, cv::Scalar(255,0,0),2); //sólo se usa para el tutorial
		
		cv::drawContours(roi, hull, -1, cv::Scalar(255, 0, 0), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point()); 

		cv::imshow("Frame",frame);
		cv::imshow("ROI",roi);
		cv::imshow("Foreground Mask", fgMask);
		cv::imshow("gray", gray);
		int c = cv::waitKey(40);

		if ((char)c =='q') break;
		// if ((char)c =='s') lr = 0;
		if ((char)c =='l') (lr = -1)? lr = 0: lr = -1;  //Se usa para dejar de "aprender" el fondo cambiando el valor del LearningRate

	}
	cap.release();
	cv::destroyAllWindows();
	

}