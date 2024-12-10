#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

class CliParser {
public:
    CliParser();
   
    void parse(int argc, char* argv[]);

    const po::options_description& getGenericOptions() const;
    const po::options_description& getVisibleOptions() const;
    const po::variables_map& getVarMap() const;
private:
    po::options_description m_genericOptions;
    po::options_description m_visibleOptions;
    po::variables_map m_varMap;
};
