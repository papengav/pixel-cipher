#pragma once

#include <opencv2/core.hpp>

class PixelCipher {
public:
    bool getBinaryMsgFromFile(const std::string& msgFile, std::string* binaryMsg);
    bool encode(cv::Mat* img, const std::string& binaryMsg, bool allowUpscale);
    bool decode(cv::Mat& img, std::string* textMsg);

private:
    std::string textToBinaryString(const std::string& text);
    std::string binaryStringToText(const std::string& binary);
    std::string extractLSBs(cv::MatIterator_<cv::Vec3b>& it, const cv::MatIterator_<cv::Vec3b>& end);
};
