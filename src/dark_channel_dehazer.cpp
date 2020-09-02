//
// Created by Hui Chih Wang on 2020/8/30.
//

#include "dark_channel_dehazer.h"

#include <iostream>

PtrImageDehazer CreateDarkChannelDehazer(const TDehazerParameter& tSetting) {
	return std::make_unique<CDarkChannelDehazer>(tSetting);
}

CDarkChannelDehazer::CDarkChannelDehazer(const TDehazerParameter& tSetting) {
	m_tSetting = tSetting;
	m_darkChannelCalculator.SetParameter(&m_tSetting.tDarkChannelSetting);
}

void CDarkChannelDehazer::SetUpImage(const cv::Mat& matImage) {
	matImage.convertTo(m_matImage, CV_64FC3);
	m_matImage /= 255.;
}

bool CDarkChannelDehazer::Dehaze() {
	if (m_matImage.empty()) {
		return false;
	}

	CalculateDarkChannel();
	EstimateAirLight();
	EstimateRoughTransmission();
	CapTransmission();
	RefineTransmission();
	RecoverNonHazyImage();

	return true;
}

cv::Mat CDarkChannelDehazer::GetDehazeImage() { return m_matDehazeImage; }

cv::Vec3f CDarkChannelDehazer::GetAirLight() { return m_airLight; }

cv::Mat CDarkChannelDehazer::GetTMap() { return m_matTMap; }

void CDarkChannelDehazer::CalculateDarkChannel() {
	m_darkChannelCalculator.SetImage(m_matImage);

	if (m_darkChannelCalculator.CalculateDarkChannel()) {
		m_matDarkChannel = m_darkChannelCalculator.GetDarkChannel();
	}
}

void CDarkChannelDehazer::EstimateAirLight() {
	cv::Mat matDC1D = m_matDarkChannel.reshape(1, 1);

	cv::Mat matDC1DSortIdx;
	auto&& sortOption =
		cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING;
	cv::sortIdx(matDC1D, matDC1DSortIdx, sortOption);

	int iAirLightPixelIndex = static_cast<int>(
		m_tSetting.airLightRatio * static_cast<float>(matDC1DSortIdx.cols));
	int iAirLight1DIndex = matDC1DSortIdx.at<int>(0, iAirLightPixelIndex);

	cv::Point ptAirLightPosition{ iAirLight1DIndex % m_matDarkChannel.cols,
								 iAirLight1DIndex / m_matDarkChannel.cols };
	m_airLight = m_matImage.at<cv::Vec3d>(ptAirLightPosition);
}

void CDarkChannelDehazer::EstimateRoughTransmission() {
	m_darkChannelCalculator.CalculateDarkChannel(m_airLight);
	auto matDarkChannelNormalize = m_darkChannelCalculator.GetDarkChannel();
	m_matTMap = 1. - matDarkChannelNormalize;
}

void CDarkChannelDehazer::RefineTransmission() {
	m_guidedFilter.SetImage(m_matTMap);
	m_guidedFilter.SetGuidedImage(m_matImage);
	m_guidedFilter.Filtering();
	m_matTMap = m_guidedFilter.GetFilteredImage();
}

void CDarkChannelDehazer::CapTransmission()
{
	m_matTMap = cv::max(m_matTMap, D_LOWEST_TRANSMISSION);
}

void CDarkChannelDehazer::RecoverNonHazyImage()
{
	std::vector<cv::Mat> vecMatBGR;
	cv::split(m_matImage, vecMatBGR);
	auto matNonHazyBlue = m_airLight[0] + (vecMatBGR[0] - m_airLight[0]) / m_matTMap;
	auto matNonHazyGreen = m_airLight[1] + (vecMatBGR[1] - m_airLight[1]) / m_matTMap;
	auto matNonHazyRed = m_airLight[2] + (vecMatBGR[2] - m_airLight[2]) / m_matTMap;

	cv::merge(std::vector<cv::Mat>{matNonHazyBlue, matNonHazyGreen, matNonHazyRed}, m_matDehazeImage);
}
