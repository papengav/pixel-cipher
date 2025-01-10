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

#include "cliParser.hpp"

#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

CliParser::CliParser()
  : m_genericOptions()
    , m_visibleOptions()
{}

void CliParser::parse(int argc, char* argv[]) {
  po::options_description generic("Generic");
  generic.add_options()
    ("help", "print the help menu")
    ("encode-image,e", po::value<std::string>(), "encode data into specified image")
    ("decode-image,d", po::value<std::string>(), "extract data from specified image")
    ;

  po::options_description encode("Encode Options");
  encode.add_options()
    ("msg,m", po::value<std::string>(), "txt file to embed into image")
    ("output-file,o", po::value<std::string>(), "name of file to produce with embedded data. Input file will be overwritten if ommitted")
    ("upscale", "attempt upscale if message exceeds image capacity")
    ("encrypt", po::value<std::string>(), "encrypt message with specified key before embedding")
    ;

  po::options_description decode("Decode Options");
  decode.add_options()
    ("decrypt", po::value<std::string>(), "decrypt message with specified key after extracting")
    ;

  m_visibleOptions.add(generic).add(encode).add(decode);

  po::store(po::command_line_parser(argc, argv).
      options(m_visibleOptions).run(), m_varMap);
  po::notify(m_varMap);

  m_genericOptions.add(generic);
}

const po::options_description& CliParser::getGenericOptions() const {
  return m_genericOptions;
}

const po::options_description& CliParser::getVisibleOptions() const {
  return m_visibleOptions;
}

const po::variables_map& CliParser::getVarMap() const {
  return m_varMap;
}
