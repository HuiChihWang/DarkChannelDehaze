//
// Created by Hui Chih Wang on 2020/8/30.
//

#ifndef DEHAZE_DARK_CHANNEL_DEHAZER_H
#define DEHAZE_DARK_CHANNEL_DEHAZER_H
#include "../include/image_dehaze.hpp"
#include "dark_channel.h"


class CDarkChannelDehazer : public IImageDehazer {
public:
    CDarkChannelDehazer(const TDehazerParameter& tSetting);
    ~CDarkChannelDehazer() = default;

    void SetUpImage(const cv::Mat &matImage) override;
    bool Dehaze() override;
    cv::Mat GetDehazeImage() override;
    cv::Vec3f GetAirLight() override;
    cv::Mat GetTMap() override;

private:

    cv::Mat m_matImage;
    cv::Mat m_matDehazeImage;
    cv::Mat m_matDarkChannel;
    cv::Mat m_matTMap;
    cv::Vec3f m_airLight;

    TDehazerParameter m_tSetting;
    CDarkChannel m_darkChannelCalculator;
};


#endif //DEHAZE_DARK_CHANNEL_DEHAZER_H
