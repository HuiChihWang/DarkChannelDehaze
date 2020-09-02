//
// Created by Hui Chih Wang on 2020/8/30.
//

#ifndef DEHAZE_DARK_CHANNEL_DEHAZER_H
#define DEHAZE_DARK_CHANNEL_DEHAZER_H
#include "../include/image_dehaze.hpp"
#include "dark_channel.h"
#include "image_guided_filter.h"

static const double D_LOWEST_TRANSMISSION = 0.1;

class CDarkChannelDehazer : public IImageDehazer {
public:
	CDarkChannelDehazer(const TDehazerParameter& tSetting);
	~CDarkChannelDehazer() = default;

	void SetUpImage(const cv::Mat& matImage) override;
	bool Dehaze() override;
	cv::Mat GetDehazeImage() override;
	cv::Vec3f GetAirLight() override;
	cv::Mat GetTMap() override;

private:
	void CalculateDarkChannel();
	void EstimateAirLight();
	void EstimateRoughTransmission();
	void RefineTransmission();
	void CapTransmission();
	void RecoverNonHazyImage();

	cv::Mat m_matImage;
	cv::Mat m_matDehazeImage;
	cv::Mat m_matDarkChannel;
	cv::Mat m_matTMap;
	cv::Vec3d m_airLight;

	TDehazerParameter m_tSetting;
	CDarkChannel m_darkChannelCalculator;
	CImageGuidedFilter m_guidedFilter;
};

#endif  // DEHAZE_DARK_CHANNEL_DEHAZER_H
