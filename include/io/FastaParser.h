#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "../sequence/DNASequence.h"
#include "../exceptions/FileParseException.h"

class FastaParser {
public:
    static std::vector<DNASequence> parse(const std::string& filename);
    static void writeFasta(const std::string& filename, const DNASequence& seq);
    static std::vector<std::pair<std::string, std::string>> parseRaw(const std::string& filename);
};
