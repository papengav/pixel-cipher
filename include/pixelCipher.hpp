#pragma once

#include <opencv2/core.hpp>
#include <optional>

class PixelCipher {
  public:
    bool getBinaryMsgFromFile(const std::string& msgFile, std::string* binaryMsg);
    bool encode(cv::Mat* img, std::string& binaryMsg, bool allowUpscale, const std::optional<std::string>& encryptKey);
    bool decode(cv::Mat& img, std::string* textMsg, const std::optional<std::string>& decryptKey);

  private:
    std::string textToBinaryString(const std::string& text);
    std::string binaryStringToText(const std::string& binary);
    std::string extractLSBs(cv::MatIterator_<cv::Vec3b>& it, const cv::MatIterator_<cv::Vec3b>& end);
    bool handleUpscale(cv::Mat* img, const std::string& binaryMsg, bool allowUpscale);
    void binaryXor(const std::string& keyBits, std::string& msgBits);
};
