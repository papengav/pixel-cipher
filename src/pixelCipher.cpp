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

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <optional>

#include <opencv2/imgproc.hpp>

#include "pixelCipher.hpp"

bool PixelCipher::getBinaryMsgFromFile(const std::string& msgFile, std::string* binaryMsg) {
  std::ifstream file(msgFile);

  if (!file.is_open()) {
    return false;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  *binaryMsg = textToBinaryString(buffer.str());

  return true;
}

std::string PixelCipher::textToBinaryString(const std::string& text) {
  std::string binaryMsg;
  for (char c : text) {
    binaryMsg += std::bitset<8>(c).to_string();
  }

  return binaryMsg;
}

std::string PixelCipher::binaryStringToText(const std::string& binary) {
  std::string text;
  for (size_t i = 0; i < binary.size(); i += 8) {
    std::bitset<8> bits(binary.substr(i, 8));
    text += static_cast<char>(bits.to_ulong());
  }

  return text;
}

void PixelCipher::binaryXor(const std::string& keyBits, std::string& msgBits) {
  for (size_t i = 0; i < msgBits.size(); ++i) {
    int msgBit = msgBits[i] - '0';
    int keyBit = keyBits[i % keyBits.size()] - '0';
    msgBits[i] = ((msgBit ^ keyBit) & 1) ? '1' : '0';
  }
}

bool PixelCipher::handleUpscale(cv::Mat* img, const std::string& binaryMsg, bool allowUpscale) {
  size_t requiredBits = binaryMsg.size() + 42;
  size_t imgBitCapacity = img->total() * 3;

  if (requiredBits > imgBitCapacity && !allowUpscale) {
    std::cout << "Failed to embed. Message size exceeds image capacity" << "\n";
    return false;
  }
  else if (requiredBits > imgBitCapacity) {
    // minimum necessary upscale factor while avoiding fractional upscaing, and preserving aspect ratio
    size_t scaleFactor = (requiredBits + imgBitCapacity - 1) / imgBitCapacity; // could this overflow if image or message is gingantic?
    cv::Size newSize(img->cols * scaleFactor, img->rows * scaleFactor);
    cv::resize(*img, *img, newSize, 0, 0, cv::INTER_CUBIC);
  }

  return true;
}


bool PixelCipher::encode(cv::Mat* img, std::string& binaryMsg, bool allowUpscale, const std::optional<std::string>& encryptKey) {
  if (!handleUpscale(img, binaryMsg, allowUpscale)) {
    return false;
  }

  std::string msgSizeBinary = std::bitset<42>(binaryMsg.size()).to_string();

  // Perform XOR against key if encrypting
  if (encryptKey) {
    std::string encryptionKeyBits = textToBinaryString(encryptKey.value());
    binaryXor(encryptionKeyBits, msgSizeBinary);
    binaryXor(encryptionKeyBits, binaryMsg);
  }

  std::string fullEmbedBinary = msgSizeBinary + binaryMsg;

  cv::MatIterator_<cv::Vec3b> imgIt = img->begin<cv::Vec3b>();
  cv::MatIterator_<cv::Vec3b> end = imgIt + (fullEmbedBinary.size() / 3) + 1; // plus one to ensure that round down doesnt cut off any bits
  auto binaryIt = fullEmbedBinary.begin(); 

  // For each pixel's Blue Green and Red values, replace each LSB with the corresponding size or message bit
  for (; imgIt != end; ++imgIt) {
    for (int bgr = 0; bgr <= 2 && binaryIt < fullEmbedBinary.end(); ++bgr, ++binaryIt) {
      int msgBit = *binaryIt - '0';
      unsigned char& data = (*imgIt)[bgr];

      data = (data & ~1) | msgBit;
    }
  }

  return true;
}

std::string PixelCipher::extractLSBs(cv::MatIterator_<cv::Vec3b>& it, const cv::MatIterator_<cv::Vec3b>& end) {
  std::string ret;

  for (; it != end; ++it) {
    for (int bgr = 0; bgr <= 2; ++bgr) {
      ret += ((*it)[bgr] & 1) ? '1' : '0';
    }
  }

  return ret;
}

bool PixelCipher::decode(cv::Mat& img, std::string* textMsg, const std::optional<std::string>& decryptKey) {
  auto it = img.begin<cv::Vec3b>();
  auto end = it + 14; // msg size is stored in the first 42bits / 14px
  std::string msgSizeBinary = extractLSBs(it, end);
  std::string decryptKeyBits;

  if (decryptKey) {
    decryptKeyBits = textToBinaryString(decryptKey.value());	
    binaryXor(decryptKeyBits, msgSizeBinary);
  }

  int msgSize = std::bitset<42>(msgSizeBinary).to_ulong();
  if (msgSize <= 0 || msgSize > img.total() * 3 - 42) {
    std::cout << "Error decoding, expected message size does not fit image" << "\n";
    return false;
  }

  // Plus one to avoid cutting off bits if the message wasnt a multiple of three.
  // Excess bits that werent part of the message will be iterated over, but not appended to the output since the buffer
  // only flushes every eight bits, so by most we'll only look at two bits that aren't needed
  auto msgEnd = end + (msgSize / 3) + 1;
  std::string binaryMsg = extractLSBs(it, msgEnd);

  if (decryptKey) {
    binaryXor(decryptKeyBits, binaryMsg);
  }

  *textMsg = binaryStringToText(binaryMsg);

  return true;
}
