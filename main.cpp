#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>

#include<string>


double angle(cv::Point s, cv::Point e, cv::Point f) {

    double v1[2],v2[2];
    v1[0] = s.x - f.x;
    v1[1] = s.y - f.y;
    v2[0] = e.x - f.x;
    v2[1] = e.x - f.x;
    double ang1 = atan2(v1[1], v1[0]);
    double ang2 = atan2(v2[1], v2[0]);

    double ang = ang1 - ang2;
    if (ang > CV_PI) ang -= 2*CV_PI;
    if (ang < -CV_PI) ang += 2*CV_PI;

    return ang*180/CV_PI;


} 


int main(int argc, char* argv[])
{
	cv::Mat frame, roi, fgMask,gray, image;
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
	int index=-1;
	int max = 0;
	std::vector<cv::Point> color;
	double lr = -1;
	while(true) {

		cap>>frame; //Capturo las imagenes al objeto FRAME
		flip(frame,frame,1); //Rota Horizontalmente para efecto espejo

		frame(rect).copyTo(roi); //Copio en roi el recuadro que usaremos
		cv::Point centro(10, 10);
		

		cv::cvtColor(roi, gray, cv::COLOR_RGB2GRAY); //Aplicamos a roi escala de grises

		rectangle(frame, rect,cv::Scalar(255,0,0)); //Dibujo en FRAME el rectanglo
		
		cv::threshold(gray, gray, 127, 255, cv::THRESH_BINARY); //Se puede quitar, se consigue mayor contraste
		// cv::GaussianBlur( gray, gray, kse , 1.0); //Para reducir ruido
		
		pBackSub->apply(gray,fgMask,lr); //aplico a gray la substracción de fondo
		
		cv::threshold(fgMask, image, 127, 255, cv::THRESH_BINARY); //Mandamos gfMask para que convierta a binario


		cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); 

	
	  cv::drawContours(roi, contours, -1, cv::Scalar(0,255,0),2);
		// drawContours(roi, contours, index, cv::Scalar(0, 255, 0), 2, 2, std::vector<cv::Vec4i>(), 0, cv::Point());
		
		// std::vector<int> hull;
		std::vector<std::vector<cv::Point>> malla(contours.size());
		// std::vector<int> hull(contours.size());;
		std::vector<int> hull;
		std::vector<cv::Vec4i> defects;	
		float depth;
		for (size_t i = 0; i < contours.size(); i++) {
			
			cv::convexHull(contours[i], hull, true, false);
			sort(hull.begin(),hull.end(),std::greater <int>());
			
			convexityDefects(contours[i], hull, defects);

			cv::Rect boundRect = boundingRect(contours[i]);
			cv::rectangle(roi,boundRect,cv::Scalar(0,0,255),1);
			int punto_medio = boundRect.width / boundRect.height;
			
			int cont = 0;


			for (int j = 0; j < defects.size(); j++) {
				depth = (float)defects[j][3] / 256.0; 
				// if( depth > 20*256 && depth < 75 ) { // filter defects by depth
					cv::Point p_start = contours[i][defects[j][0]];
					cv::Point p_end = contours[i][defects[j][1]];
					cv::Point p_far = contours[i][defects[j][2]];
					
				
					double ang = angle(p_start,p_end,p_far);
					// cv::putText(roi, std::to_string(ang), cv::Point(j*50, j*50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
					if(ang < 90.0 && depth > 45) { 
						cv::circle(roi, p_far ,5,cv::Scalar(0,0,255),-1);
						cv::line(roi,p_start,p_end,cv::Scalar(255,0,0),1);
						cont++;
					}
				}

			// Contar dedos
			if (punto_medio < 1.2 && punto_medio > 0.5)
			{
				cont = 0;
				cv::putText(roi, std::to_string(cont), cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
				//si cerramos la mano nos borra el dibujo
				color.clear();
			}
			else
			{
				putText(roi, std::to_string(cont + 1),cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
			}


			}
		
		// cv::drawContours(roi, malla, -1, cv::Scalar(255,0,0),2); //sólo se usa para el tutorial
		
		// cv::drawContours(roi, malla, -1, cv::Scalar(255, 0, 0), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point()); 

		cv::putText(roi, "TEST", cv::Point(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );

		cv::imshow("Frame",frame);
		cv::imshow("ROI",roi);
		cv::imshow("Foreground Mask", fgMask);
		cv::imshow("gray", gray);
		// cv::imshow("image", image);
		int c = cv::waitKey(40);

		if ((char)c =='q') break;
		
		if ((char)c =='l') lr = 0;  //Se usa para dejar de "aprender" el fondo cambiando el valor del LearningRate

	}
	cap.release();
	cv::destroyAllWindows();

}