//
// Created by Hui Chih Wang on 2020/8/28.
//
#ifndef DEHAZE_DARK_CHANNEL_H
#define DEHAZE_DARK_CHANNEL_H

#include "../include/parameter_struct.h"
#include "opencv2/core.hpp"

class CDarkChannel {
 public:
  CDarkChannel() = default;
  ~CDarkChannel();

  void SetImage(const cv::Mat& matImage);
  void SetParameter(TDarkChannelSetting* pTSetting);

  bool CalculateDarkChannel(const cv::Vec3f& vecAirLight = cv::Vec3f{1.f, 1.f,
                                                                     1.f});

  const cv::Mat& GetDarkChannel();
  void ClearResult();

 private:
  static cv::Mat FilterRGBMin(const cv::Mat& matImage);
  static cv::Mat FilterGrayMin(const cv::Mat& matImage,
                               const cv::Size sizePatch);

  cv::Mat m_matImage;
  cv::Mat m_matDarkChannel;
  TDarkChannelSetting* m_pTSetting = new TDarkChannelSetting;
};

#endif  // DEHAZE_DARK_CHANNEL_H
