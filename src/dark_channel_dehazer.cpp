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
    cv::Mat matTest = cv::Mat::eye(4,4,CV_32F);
    cv::Mat matSortByRow;
    cv::sortIdx(matTest, matSortByRow,
                cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING);

    std::cout << matSortByRow;
}


