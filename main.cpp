/*
 * Sample Encode:
 * pxc -e img.png -m msg.txt -o pxc.png
 * 
 * Sample Decode:
 * pxc -d pxc.png
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

    cv::Mat embed = cv::imread(imgFile, cv::IMREAD_COLOR);

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
    cv::Mat embed = cv::imread(imgFile, cv::IMREAD_COLOR);
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
	std::cout << "Must use one of --encode or --decode" << "\n";
	return 1;
    }

    return 0;
}
