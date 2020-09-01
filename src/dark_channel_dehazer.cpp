//
// Created by Hui Chih Wang on 2020/8/30.
//

#include <iostream>
#include "dark_channel_dehazer.h"

PtrImageDehazer CreateDarkChannelDehazer(const TDehazerParameter& tSetting){
    return std::make_unique<CDarkChannelDehazer>(tSetting);
}

CDarkChannelDehazer::CDarkChannelDehazer(const TDehazerParameter &tSetting) {
    m_tSetting = tSetting;
    m_darkChannelCalculator.SetParameter(&m_tSetting.tDarkChannelSetting);
}

void CDarkChannelDehazer::SetUpImage(const cv::Mat &matImage) {
    m_matImage = matImage;
}

bool CDarkChannelDehazer::Dehaze() {
    if (m_matImage.empty()) {
        return false;
    }

    CalculateDarkChannel();
    EstimateAirLight();

    return true;
}

cv::Mat CDarkChannelDehazer::GetDehazeImage() {
    return m_matDehazeImage;
}

cv::Vec3f CDarkChannelDehazer::GetAirLight() {
    return m_airLight;
}

cv::Mat CDarkChannelDehazer::GetTMap() {
    return m_matTMap;
}

void CDarkChannelDehazer::CalculateDarkChannel() {
    m_darkChannelCalculator.SetImage(m_matImage);

    if (m_darkChannelCalculator.CalDarkChannel()) {
        m_matDarkChannel = m_darkChannelCalculator.GetDarkChannel();
    }
}

void CDarkChannelDehazer::EstimateAirLight() {
    cv::Mat matDC1D = m_matDarkChannel.reshape(1,1);

    cv::Mat matDC1DSortIdx;
    auto&& sortOption = cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING;
    cv::sortIdx(matDC1D, matDC1DSortIdx, sortOption);

    int iAirLightPixelIndex = static_cast<int>(m_tSetting.airLightRatio * static_cast<float>(matDC1DSortIdx.cols));
    int iAirLight1DIndex = matDC1DSortIdx.at<int>(0, iAirLightPixelIndex);

    cv::Point ptAirLightPosition{iAirLight1DIndex % m_matDarkChannel.cols, iAirLight1DIndex / m_matDarkChannel.cols};
    m_airLight = cv::Vec3f{ m_matImage.at<cv::Vec3b>(ptAirLightPosition) };
}


