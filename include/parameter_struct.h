//
// Created by Hui Chih Wang on 2020/8/30.
//

#ifndef DEHAZE_PARAMETER_STRUCT_H
#define DEHAZE_PARAMETER_STRUCT_H
#include "opencv2/highgui.hpp"

inline void ShowImage(const cv::Mat& matImage, const std::string& strWinName) {
    cv::imshow(strWinName, matImage);
    cv::waitKey();
}

struct TDarkChannelSetting {
    cv::Size sizePatch = cv::Size{7,7};
};


struct TDehazerParameter {
    TDarkChannelSetting tDarkChannelSetting;
    float airLightRatio = 0.9;
};


#endif //DEHAZE_PARAMETER_STRUCT_H
