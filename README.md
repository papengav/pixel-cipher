# Pixel Cipher

A CLI tool for encoding and decoding messages within images via LSB Steganography.

## Table of Contents
1. [Features](#features)
2. [Installation](#installation)
3. [Usage](#usage)
   - [Encoding](#encoding)
   - [Decoding](#decoding)
4. [Options](#options)
5. [Example Usage](#example)
6. [Dependencies](#dependencies)

## Features
- Encode messages into image files.
- Decode hidden messages from image files.
- Simple command-line interface.
- Supports color images in `.png` and `.bmp` formats.

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/papengav/pixelcipher.git
    cd pixelcipher
    ```

2. Build the project using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

## Usage
### Encoding
```sh
pxc -e <input-image> -m <message-file> -o <output-image>
```

*Exporting as `.jpg` will produce unexpected results since lossy compression skews message integrity*

### Decoding
```sh
pxc -d <input.image>
```

### Options
Generic:
* `--help` : Display help menu.
* `-e | --encode-image` `<filepath>` : Specify the input image for encoding.
* `-d | --decode-image` `<filepath>` : Specify the input image for decoding.

Encoding:
* `-m | --msg` `<filepath.txt>` : Specify the file containing data to encode.
* `-o | --output-file` `<filepath>` : Specify the output image after encoding. If none is provided, input image will be overriden.
* `--upscale` : Allow image upscaing if message exceeds image capacity
* `--encrypt` `<secret-txt>` : Encrypt message contents before embedding.

Decoding:
* `--decrypt` `<secret-txt>` : Decrypt message contents after decoding

## Example
```sh
pxc -e img.png -m msg.txt -o encoded.png --encrypt mySecret
pxc -d encoded.png --decrypt mySecret
```

## Dependencies
* OpenCV: Used for image processing.
* Boost Program Options: Used for command-line parsing.
