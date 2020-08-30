//
// Created by Hui Chih Wang on 2020/8/28.
//

#include "dark_channel.h"
#include "opencv2/imgproc.hpp"

void CDarkChannel::SetImage(const cv::Mat &matImage) {
    m_matImage = matImage;
}

void CDarkChannel::SetParameter(TDarkChannelSetting* pTSetting) {
    m_pTSetting = pTSetting;
}

bool CDarkChannel::CalDarkChannel() {
    if (m_matImage.empty()) {
        return false;
    }

    auto matRGBMin = FilterRGBMin(m_matImage);

    auto matDarkChannelRough = FilterGrayMin(matRGBMin, m_pTSetting->sizePatch);
    ShowImage(matDarkChannelRough, "DCRGB");
    m_matDarkChannel = matDarkChannelRough;
    return true;
}

const cv::Mat &CDarkChannel::GetDarkChannel() {
    return m_matDarkChannel;
}

void CDarkChannel::ClearResult() {
    m_matDarkChannel.release();
    m_matImage.release();
}

CDarkChannel::~CDarkChannel() {
    ClearResult();
}

cv::Mat CDarkChannel::FilterRGBMin(const cv::Mat &matImage) {

    std::vector<cv::Mat> vecChannels{3};
    cv::split(matImage, vecChannels);

    return cv::Mat(cv::min(vecChannels[0], cv::min(vecChannels[1], vecChannels[2])));
}

cv::Mat CDarkChannel::FilterGrayMin(const cv::Mat &matImage, const cv::Size sizePatch) {
    cv::Mat matGrayMin;
    auto matKernel = cv::getStructuringElement(cv::MORPH_RECT, sizePatch);
    cv::erode(matImage, matGrayMin, matKernel);

    return matGrayMin;
}

