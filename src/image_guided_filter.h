#pragma once
#include "opencv2/core.hpp"

struct TGuidedFilterParameter {
	int iPatchRadius = 3;
	float fRequlizer = 1.f;
};

class CImageGuidedFilter {
public:
	CImageGuidedFilter(const TGuidedFilterParameter& tSetting);
	~CImageGuidedFilter() = default;

	void SetImage(const cv::Mat& matImageFloat);
	void SetGuidedImage(const cv::Mat& matGuidedImage);


private:
	cv::Mat m_matImage;
	TGuidedFilterParameter m_tSetting;
};
