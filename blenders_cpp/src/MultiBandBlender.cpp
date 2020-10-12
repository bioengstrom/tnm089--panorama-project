#include "MultiBandBlender.h"


void MultiBandBlender::PreProcess() {
	if (_img1.rows != _img2.rows) {
		std::cout << "Images must have the same height, the first image is " << _img1.rows << "px and the other is " << _img2.rows << "\n";
		return;
	}
	if (_overlapWidth > _img1.cols || _overlapWidth > _img2.cols) {
		std::cout << "The overlap is wider than one or both of the input images' width\n";
		return;
	}

	ProcessNumBands();
	PrepImages();

	std::cout << "Finished pre process\n";
}

void MultiBandBlender::ProcessNumBands() {
	std::initializer_list<int> imagesDims = { _img1.cols, _img1.rows, _img2.cols, _img2.rows };
	int maxNumBands = (int)std::floor(std::log2(std::min({ _img1.cols, _img1.rows, _img2.cols, _img2.rows }, std::less<int>())));
	//std::cout << "Max number of bands: " << maxNumBands << "\n";

	if (_numBands > maxNumBands) {
		_numBands = maxNumBands;
		std::cout << "The number of bands was lowered to the maximum possible number of bands for these input images. Lowered to: '" << maxNumBands << "'.\n";
	}
}

std::vector<cv::Mat> MultiBandBlender::LaplacianPyramid(const cv::Mat& inImg) {
	std::vector<cv::Mat> result;
	cv::Mat img = inImg;
	cv::Mat nextImg;
	cv::Mat upSampledImg;

	for (int i = 0; i < _numBands - 1; ++i) {
		cv::pyrDown(img, nextImg, cv::Size(img.cols / 2, img.rows / 2));
		cv::pyrUp(nextImg, upSampledImg, cv::Size(img.cols * 2, img.rows * 2));

		result.push_back(img - upSampledImg);
		img = nextImg;
	}
	result.push_back(img);

	return result;
}

void MultiBandBlender::PrepImages() {
	
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
		/*
		cv::imshow("Mask", _mask);
		cv::waitKey(0);
		*/

	}
	else {
		_resWidth = _img1.cols + _img2.cols - _overlapWidth;
	}
}

MultiBandBlender::MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2, int bands)
	: _img1{ img1 }, _img2{ img2 }, _numBands{bands}, _overlapWidth{ 0 }, _flagHalf{ true }
{
	PreProcess();
}

MultiBandBlender::MultiBandBlender(const cv::Mat& img1, const cv::Mat& img2, int bands, int overlapWidth)
	: _img1{ img1 }, _img2{ img2 }, _numBands{ bands }, _overlapWidth{ overlapWidth }, _flagHalf{ true }
{
	PreProcess();
}
