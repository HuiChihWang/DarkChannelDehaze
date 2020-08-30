//
// Created by Hui Chih Wang on 2020/8/30.
//

#ifndef DEHAZE_PARAMETER_STRUCT_H
#define DEHAZE_PARAMETER_STRUCT_H

struct TDarkChannelSetting {
    cv::Size sizePatch = cv::Size{7,7};
};


struct TDehazerParameter {
    TDarkChannelSetting tDarkChannelSetting;
};


#endif //DEHAZE_PARAMETER_STRUCT_H
