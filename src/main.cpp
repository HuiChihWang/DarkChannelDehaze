#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "../include/image_dehaze.hpp"

int main(int argc, char *argv[]) {

    const std::string strFilePath{argv[1]};
    const cv::Mat matImage = cv::imread(strFilePath);

    TDehazerParameter tDehazerParameter;

    const auto pDehazer = CreateDarkChannelDehazer(tDehazerParameter);
    pDehazer->SetUpImage(matImage);
    pDehazer->Dehaze();
    auto matDehazeImage = pDehazer->GetDehazeImage();

    return 0;
}

