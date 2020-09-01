#include "image_guided_filter.h"
#include "opencv2/imgproc.hpp"

#include "../include/parameter_struct.h"

CImageGuidedFilter::CImageGuidedFilter(const TGuidedFilterParameter &tSetting) {
    m_tSetting = tSetting;
}

void CImageGuidedFilter::SetImage(const cv::Mat &matImageFloat) {
    m_matImage = matImageFloat;
}

void CImageGuidedFilter::SetGuidedImage(const cv::Mat &matGuidedImage) {
    cv::Mat matGuided32F;
    matGuidedImage.convertTo(matGuided32F, CV_32F);

    if (matGuidedImage.channels() == 3) {
        cv::cvtColor(matGuided32F, m_matGuidedImage, cv::COLOR_BGR2GRAY);
    }
    else {
     m_matGuidedImgMean = matGuided32F;
    }

    matGuided32F.convertTo(m_matGuidedImage, CV_64F);
}

void CImageGuidedFilter::Filtering() {
    CalculateMean();
    CalculatePatchVariance();
    CalculateLinearCoefficients();
    CalculateBias();
    CalculateGuidedImage();
}

cv::Mat CImageGuidedFilter::GetFilteredImage() {
    return m_matFilteredImage;
}

void CImageGuidedFilter::CalculateGuidedImage() {
    m_matFilteredImage = m_matLinearCoefficients.mul(m_matGuidedImage) + m_matBias;
}

void CImageGuidedFilter::CalculateLinearCoefficients() {
    m_matLinearCoefficients = m_matCorrelationMean - m_matGuidedImgMean.mul(m_matInputMean);
    m_matLinearCoefficients /= (m_matGuidedImgVariance + static_cast<double>(m_tSetting.fRegulizer));
}

void CImageGuidedFilter::CalculateBias() {
    m_matBias = m_matInputMean - m_matLinearCoefficients.mul(m_matGuidedImgMean);
}

void CImageGuidedFilter::CalculateMean() {
    int iPatchSize = m_tSetting.iPatchRadius * 2 + 1;
    cv::Mat matKernel{cv::Size{iPatchSize, iPatchSize}, CV_64F};

    cv::filter2D(m_matGuidedImage, m_matGuidedImgMean, -1, matKernel);
    cv::filter2D(m_matImage, m_matInputMean, -1, matKernel);

    cv::Mat matCorrelation = m_matGuidedImage.mul(m_matImage);
    cv::filter2D(matCorrelation, m_matCorrelationMean, -1, matKernel);
}

void CImageGuidedFilter::CalculatePatchVariance() {
    int iPatchSize = m_tSetting.iPatchRadius * 2 + 1;
    cv::Mat matKernel{cv::Size{iPatchSize, iPatchSize}, CV_64F};

    cv::Mat matGuidedImgSquareMean;
    cv::Mat matGuidedImgSquare = m_matGuidedImage.mul(m_matGuidedImage);
    cv::filter2D(matGuidedImgSquare, matGuidedImgSquareMean, -1, matKernel);

    cv::Mat matGuidedImgMeanSquare = m_matGuidedImgMean.mul(m_matGuidedImgMean);
    m_matGuidedImgVariance = matGuidedImgSquareMean - matGuidedImgMeanSquare;
}
