#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <list>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
//#include "opencv2/world.hpp"

class MultiBandBlender {
private:
	cv::Mat _img1, _img2, _imgResult, _subA, _subB, _mask;
	int _numBands, _overlapWidth;
	int _resWidth, _resHeight;
	cv::Size _resSize;
	//std::string _edgeFlag;
	bool _flagHalf;

	void PreProcess();
	void ProcessNumBands();
	void PrepImages();
	void TestPyramids();

	std::vector<cv::Mat> BlendPyramids(const std::vector<cv::Mat>& LPA, const std::vector<cv::Mat>& LPB, const std::vector<cv::Mat>& MP);
	cv::Mat ReconstructImage(const std::vector<cv::Mat>& BP);
public:
	MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2, int bands);
	MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2, int bands, int overlap);
	
	~MultiBandBlender() {}

	std::vector<cv::Mat> LaplacianPyramid(const cv::Mat& inImg);
	std::vector<cv::Mat> GaussianPyramid(const cv::Mat& inImg);
	
	cv::Mat GetMask() { return _mask; }

};
