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

#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <optional>

#include "cliParser.hpp"
#include "pixelCipher.hpp"

void handleEncode(const po::variables_map& varMap, PixelCipher& pxc) {
  const std::string imgFile (varMap["encode-image"].as<std::string>());
  const std::string msgFile (varMap["msg"].as<std::string>());
  bool upscale = varMap.count("upscale");
  std::optional<std::string> encryptKey;

  if (varMap.count("encrypt")) {
    encryptKey = varMap["encrypt"].as<std::string>();
  }

  // If output file specified, use that, otherwise replace input file.
  std::string outputFile = imgFile;
  if (varMap.count("output-file")) {
    outputFile = varMap["output-file"].as<std::string>();
  }

  std::string binaryMsg;
  if (!pxc.getBinaryMsgFromFile(msgFile, &binaryMsg)) {
    std::cout << "Error reading msg-file: " << msgFile << "\n";
    return;
  }

  cv::Mat embed = cv::imread(imgFile, cv::IMREAD_UNCHANGED);

  if (embed.empty()) {
    std::cout << "Error reading image file" << "\n";
    return;
  }

  if (!pxc.encode(&embed, binaryMsg, upscale, encryptKey)) {
    return;
  }

  if (!cv::imwrite(outputFile, embed)) {
    std::cout << "Error writing to output-file: " << outputFile << "\n";
    return;
  }
}

void handleDecode(const po::variables_map& varMap, PixelCipher& pxc) {
  const std::string imgFile(varMap["decode-image"].as<std::string>());
  cv::Mat embed = cv::imread(imgFile, cv::IMREAD_UNCHANGED);
  std::optional<std::string> decryptKey;

  if (embed.empty()) {
    std::cout << "Error reading image file" << "\n";
    return;
  }

  if (varMap.count("decrypt")) {
    decryptKey = varMap["decrypt"].as<std::string>();
  }

  std::string txt;
  if (!pxc.decode(embed, &txt, decryptKey)) {
    return;
  }

  std::cout << txt << "\n";
}

int main(int argc, char *argv[]) {
  CliParser parser = CliParser();
  PixelCipher pxc = PixelCipher();

  try {
    parser.parse(argc, argv);
  }
  catch (const po::error& e) {
    std::cout << e.what() << "\n";
    return 1;
  }

  const auto& varMap = parser.getVarMap();

  if (varMap.count("help")) {
    std::cout << parser.getVisibleOptions() << "\n";
    return 0;
  }

  if (varMap.count("encode-image") && !varMap.count("decode-image")) {
    handleEncode(varMap, pxc);
  }
  else if (varMap.count("decode-image") && !varMap.count("encode-image")) {
    handleDecode(varMap, pxc);
  }
  else {
    std::cout << parser.getGenericOptions() << "\n";
    return 1;
  }

  return 0;
}
