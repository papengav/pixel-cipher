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
