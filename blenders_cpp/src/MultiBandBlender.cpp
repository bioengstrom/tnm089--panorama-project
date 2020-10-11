#include "MultiBandBlender.h"


void MultiBandBlender::preProcess() {
	if (_img1.rows != _img2.rows) {
		std::cout << "Images must have the same height, the first image is " << _img1.rows << "px and the other is " << _img2.rows << "\n";
		return;
	}
	if (_overlapWidth > _img1.cols || _overlapWidth > _img2.cols) {
		std::cout << "The overlap is wider than one or both of the input images' width\n";
		return;
	}

	calcImgResultSize();

	std::cout << "Finished pre process\n";
}

void MultiBandBlender::calcImgResultSize() {
	
	_resHeight = _img1.rows;
	
	if (_flagHalf == true) {
		
		_resWidth = (_img1.cols + _img2.cols) / 2;
		_resSize = cv::Size(_resWidth, _resHeight);
		
		std::cout << "\nresWidth: " << _resWidth << " resHeight: " << _resHeight << " resSize: " << _resSize << "\n";
		
		_subA = cv::Mat::zeros(_resSize, _img1.type());
		cv::Rect roiA = cv::Rect(0, 0, (_img1.cols + _overlapWidth) / 2, _resHeight);
		_img1(roiA).copyTo(_subA(roiA));
		
		cv::imshow("subA", _subA);
		cv::waitKey(0);

		_subB = cv::Mat::zeros(_resSize, _img1.type());
		cv::Rect roiB = cv::Rect(_resWidth / 2 + _overlapWidth / 2, 0, (_img2.cols - _overlapWidth) / 2, _resHeight);
		_img2(roiB).copyTo(_subB(roiB));

		cv::imshow("subB", _subB);
		cv::waitKey(0);

		_mask = cv::Mat::zeros(_resSize, _img1.type());
		cv::Rect roiMask = cv::Rect(_resWidth / 2 + _overlapWidth / 2, 0, (_img1.cols - _overlapWidth) / 2, _resHeight);
		_mask(roiMask).setTo(255);

		cv::imshow("Mask", _mask);
		cv::waitKey(0);

	}
	else {
		_resWidth = _img1.cols + _img2.cols - _overlapWidth;

	}
	
}

MultiBandBlender::MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2)
	: _img1{ img1 }, _img2{ img2 }, _overlapWidth{ 0 }, _flagHalf{ true }
{
	preProcess();
}

MultiBandBlender::MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2, int overlapWidth)
	: _img1{ img1 }, _img2{ img2 }, _overlapWidth{ overlapWidth }, _flagHalf{ true }
{
	preProcess();
}
