/*
===========================================================================

MIT License

Copyright (c) 2024 Gavin Papenthien

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/

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
