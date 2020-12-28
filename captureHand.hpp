#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class CaptureHand {
 private:
  cv::Rect rect_;
  cv::Mat principal_;
  cv::Mat sec_;
  std::vector<cv::Point> pintar_;
  int lr_ = -1;
  bool view = false;
  std::vector<std::vector<cv::Point>> contours_;
  std::vector<cv::Vec4i> defects_;	
  std::vector<int> hull;
  std::vector<double> angulos_;  
  std::vector<cv::Point> puntos_;
  float depth_;
  cv::Rect boundRect_;
  int count_ = 0;
  double ang_;

 public:
  CaptureHand(int, int, int, int, bool);
  
  ~CaptureHand();

  void ApplyFilters();

  void SetFrame(cv::Mat& frame);

  void StopLearningRate();

  cv::Mat& GetResult();

 private:

  void AngleAndDepthFilter(int&);

  void IdentifyNumbers();

  void Draw();

  void IdentifySymbol();

  void AnalizeContour();

  void ObtainBoundinRect();

  void ObtainContours();

  double Angle(cv::Point s, cv::Point e, cv::Point f);

  cv::Mat& ApplyFiltersAndGetImage(cv::Mat& frame, int& lr);


};
