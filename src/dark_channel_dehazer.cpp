//
// Created by Hui Chih Wang on 2020/8/30.
//

#include "dark_channel_dehazer.h"

PtrImageDehazer CreateDarkChannelDehazer(const TDehazerParameter& tSetting){
    return std::make_unique<CDarkChannelDehazer>(tSetting);
}

CDarkChannelDehazer::CDarkChannelDehazer(const TDehazerParameter &tSetting) {
    m_tSetting = tSetting;
}

void CDarkChannelDehazer::SetUpImage(const cv::Mat &matImage) {
    m_matImage = matImage;
}

bool CDarkChannelDehazer::Dehaze() {
    if (m_matImage.empty()) {
        return false;
    }

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


