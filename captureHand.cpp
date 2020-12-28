#include "captureHand.hpp"


CaptureHand::CaptureHand(int c_x, int c_y, int width , int height, bool see) {
  rect_ = cv::Rect(c_x, c_y, width, height);
  view = see;

}

CaptureHand::~CaptureHand()
{
}


void CaptureHand::SetFrame(cv::Mat& frame) {
  flip(frame,frame,1); //Rota Horizontalmente para efecto espejo
  frame(rect_).copyTo(principal_); //Copio en principal_ el recuadro que usaremos
  rectangle(frame, rect_,cv::Scalar(255,0,0)); //Dibujo en FRAME el rectanglo
  ApplyFilters();
  ObtainContours();
  AnalizeContour();

}

void CaptureHand::ApplyFilters() {

  cv::Mat fgBack;
  cv::Ptr<cv::BackgroundSubtractor> pBackSub = cv::createBackgroundSubtractorMOG2();
  cv::cvtColor(principal_, sec_, cv::COLOR_RGB2GRAY); //Aplicamos a principal_ escala de grises
  cv::threshold(sec_, sec_, 127, 255, cv::THRESH_BINARY); //Se puede quitar, se consigue mayor contraste
  pBackSub->apply(sec_,sec_,lr_); //aplico a gray la substracción de fondo
  cv::threshold(sec_,sec_, 127, 255, cv::THRESH_BINARY); //Mandamos gfMask para que convierta a binario
  
}


void CaptureHand::ObtainContours() {
  cv::findContours(sec_, contours_, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  if(view)
    cv::drawContours(principal_, contours_, -1, cv::Scalar(0,255,0),2);
}



void CaptureHand::AnalizeContour() {

  hull.clear();
  defects_.clear();
  depth_ = -1;

  for (int i = 0; i < contours_.size(); i++) {
  
    cv::convexHull(contours_[i], hull, true, false);
    sort(hull.begin(),hull.end(),std::greater <int>());
    convexityDefects(contours_[i], hull, defects_);

    boundRect_ = cv::boundingRect(contours_[i]);
    
    if(view) cv::rectangle(principal_,boundRect_,cv::Scalar(0,0,255),1);
        
    AngleAndDepthFilter(i);
    IdentifyNumbers();
    IdentifySymbol();
    Draw();
    
  }

}

void CaptureHand::AngleAndDepthFilter(int& i) {

  count_ = 0;
  angulos_.clear();
  puntos_.clear();
  for (int j = 0; j < defects_.size(); j++) {
    depth_ = (float)defects_[j][3] / 256.0; 
    cv::Point p_start = contours_[i][defects_[j][0]];
    cv::Point p_end = contours_[i][defects_[j][1]];
    cv::Point p_far = contours_[i][defects_[j][2]];
    
    ang_ = Angle(p_start,p_end,p_far);
    
    if(ang_ < 100.0 && depth_ > 38 ) { 
      angulos_.push_back(ang_);
      puntos_.push_back(p_far);
      if(view) {
        cv::circle(principal_, p_far ,5,cv::Scalar(0,0,255),-1);
        cv::line(principal_,p_start,p_end,cv::Scalar(255,0,0),1);
      }
      count_++;
    }
    
    
  }
}
void CaptureHand::IdentifyNumbers() {
  if ( boundRect_.width > 80 && boundRect_.height < 150 ) {    
    cv::putText(principal_, std::to_string(count_), cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
    pintar_.clear();
  }
  else {    
    putText(principal_, std::to_string(count_ + 1),cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
  }  
}

void CaptureHand::IdentifySymbol() {

  switch (count_) {
    case 1:
      if(angulos_[0] < 70) {
        putText(principal_, "Paz",cv::Point(100, 180), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
        
      } else if(angulos_[0] > 80){
        putText(principal_, "Rock",cv::Point(100, 180), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
        pintar_.push_back(puntos_[0]);
      }
    break;

    default:
      putText(principal_, "No sign",cv::Point(100, 180), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2, 8, false );
    break;
  }

}

void CaptureHand::Draw() {
  for ( int i = 0; i < pintar_.size(); i++) {
    circle(principal_, pintar_[i], 2, cv::Scalar(255, 0, 255), -1);
  }
}








cv::Mat& CaptureHand::GetResult() {
  return principal_;
}




void CaptureHand::StopLearningRate() {
  lr_ = 0;
}


cv::Mat& CaptureHand::ApplyFiltersAndGetImage(cv::Mat& frame, int& lr) {

  cv::Ptr<cv::BackgroundSubtractor> pBackSub = cv::createBackgroundSubtractorMOG2();
  cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY); //Aplicamos a principal_ escala de grises
  cv::threshold(frame, frame, 127, 255, cv::THRESH_BINARY); //Se puede quitar, se consigue mayor contraste
  pBackSub->apply(frame,frame,lr); //aplico a gray la substracción de fondo
  cv::threshold(frame,frame, 127, 255, cv::THRESH_BINARY); //Mandamos gfMask para que convierta a binario
  return frame;
}


double CaptureHand::Angle(cv::Point s, cv::Point e, cv::Point f) {

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