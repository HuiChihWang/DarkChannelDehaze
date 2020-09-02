#pragma once
#include "opencv2/core.hpp"

struct TGuidedFilterParameter {
	int iPatchRadius = 3;
	float fRegulizer = 0.003;
};

class CImageGuidedFilter {
public:
	CImageGuidedFilter(const TGuidedFilterParameter& tSetting = TGuidedFilterParameter{});
	~CImageGuidedFilter() = default;

	void SetImage(const cv::Mat& matImageFloat);
	void SetGuidedImage(const cv::Mat& matGuidedImage);

	void Filtering();
	cv::Mat GetFilteredImage();


private:
	void CalculateMean();
	void CalculatePatchVariance();
	void CalculateLinearCoefficients();
	void CalculateBias();
	void CalculateGuidedImage();

	cv::Mat m_matImage;
	cv::Mat m_matGuidedImage;
	cv::Mat m_matFilteredImage;

	cv::Mat m_matLinearCoefficients;
	cv::Mat m_matBias;

	cv::Mat m_matGuidedImgMean;
	cv::Mat m_matGuidedImgVariance;
	cv::Mat m_matInputMean;
	cv::Mat m_matCorrelationMean;

	TGuidedFilterParameter m_tSetting;
};
