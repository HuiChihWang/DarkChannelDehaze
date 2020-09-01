//
// Created by Hui Chih Wang on 2020/8/28.
//

#include "dark_channel.h"

#include "opencv2/imgproc.hpp"

void CDarkChannel::SetImage(const cv::Mat &matImage) { m_matImage = matImage; }

void CDarkChannel::SetParameter(TDarkChannelSetting *pTSetting) {
  m_pTSetting = pTSetting;
}

bool CDarkChannel::CalculateDarkChannel(const cv::Vec3f &vecAirLight) {
  if (m_matImage.empty()) {
    return false;
  }

  std::vector<cv::Mat> vecBGRChannels;
  cv::split(m_matImage, vecBGRChannels);
  auto matDarkBlue = FilterGrayMin(vecBGRChannels[0] / vecAirLight[0], m_pTSetting->sizePatch);
  auto matDarkGreen = FilterGrayMin(vecBGRChannels[1] / vecAirLight [1], m_pTSetting->sizePatch);
  auto matDarkRed = FilterGrayMin(vecBGRChannels[2] / vecAirLight[2], m_pTSetting->sizePatch);

  // TODO: implment normalized dark channel

  cv::Mat matDarkChannelRGB;
  cv::merge(std::vector<cv::Mat>{matDarkBlue, matDarkGreen, matDarkRed},
            matDarkChannelRGB);
  m_matDarkChannel = FilterRGBMin(matDarkChannelRGB);

  return true;
}

const cv::Mat &CDarkChannel::GetDarkChannel() { return m_matDarkChannel; }

void CDarkChannel::ClearResult() {
  m_matDarkChannel.release();
  m_matImage.release();
}

CDarkChannel::~CDarkChannel() { ClearResult(); }

cv::Mat CDarkChannel::FilterRGBMin(const cv::Mat &matImage) {
  std::vector<cv::Mat> vecChannels{3};
  cv::split(matImage, vecChannels);

  return cv::Mat(
      cv::min(vecChannels[0], cv::min(vecChannels[1], vecChannels[2])));
}

cv::Mat CDarkChannel::FilterGrayMin(const cv::Mat &matImage,
                                    const cv::Size sizePatch) {
  cv::Mat matGrayMin;
  auto matKernel = cv::getStructuringElement(cv::MORPH_RECT, sizePatch);
  cv::erode(matImage, matGrayMin, matKernel);

  return matGrayMin;
}
