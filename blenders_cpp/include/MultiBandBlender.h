#pragma once

#include <iostream>
#include <string>

#include "opencv2/highgui.hpp"
//#include "opencv2/world.hpp"

class MultiBandBlender {
private:
	cv::Mat _img1, _img2, _imgResult, _subA, _subB, _mask;
	int _overlapWidth;
	int _resWidth, _resHeight;
	cv::Size _resSize;
	//std::string _edgeFlag;
	bool _flagHalf;

	void preProcess();
	void calcImgResultSize();


public:
	MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2);
	MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2, int overlap);
	
	
	~MultiBandBlender() {}
};
