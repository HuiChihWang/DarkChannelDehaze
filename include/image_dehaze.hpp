//
// Created by Hui Chih Wang on 2020/8/30.
//

#ifndef DEHAZE_IMAGE_DEHAZE_HPP
#define DEHAZE_IMAGE_DEHAZE_HPP
#include "opencv2/core.hpp"
#include "parameter_struct.h"

class IImageDehazer {
public:
    virtual ~IImageDehazer() = default;
    virtual void SetUpImage(const cv::Mat& matImage) = 0;
    virtual bool Dehaze() = 0;

    virtual cv::Mat GetDehazeImage() = 0;
    virtual cv::Vec3f GetAirLight() = 0;
    virtual cv::Mat GetTMap() = 0;
};

typedef std::unique_ptr<IImageDehazer> PtrImageDehazer;
PtrImageDehazer CreateDarkChannelDehazer(const TDehazerParameter& tSetting);

#endif //DEHAZE_IMAGE_DEHAZE_HPP
