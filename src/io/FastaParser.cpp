#include "../../include/io/FastaParser.h"
#include <algorithm>
#include <cctype>

std::vector<DNASequence> FastaParser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileParseException("Cannot open file: " + filename);
    }

    std::vector<DNASequence> sequences;
    std::string line, currentName, currentSeq;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        
        // Strip trailing whitespace
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());
        
        // Strip leading whitespace
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));

        if (line.empty()) {
            continue;
        }

        if (line[0] == '>') {
            if (!currentName.empty()) {
                try {
                    sequences.push_back(DNASequence(currentName, currentSeq));
                } catch (const std::exception& e) {
                    throw FileParseException(filename, lineNum, e.what());
                }
            }

            currentName = line.substr(1);
            
            currentName.erase(std::find_if(currentName.rbegin(), currentName.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), currentName.end());
            currentName.erase(currentName.begin(), std::find_if(currentName.begin(), currentName.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            
            size_t spacePos = currentName.find(' ');
            if (spacePos != std::string::npos) {
                currentName = currentName.substr(0, spacePos);
            }
            
            currentSeq = "";
        } else {
            line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char ch) {
                return std::isspace(ch) || !std::isalpha(ch);
            }), line.end());
            
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            
            currentSeq += line;
        }
    }

    // Add the very last sequence in the file, if any exists
    if (!currentName.empty()) {
         try {
             sequences.push_back(DNASequence(currentName, currentSeq));
         } catch (const std::exception& e) {
             throw FileParseException(filename, lineNum, e.what());
         }
    }

    return sequences;
}

void FastaParser::writeFasta(const std::string& filename, const DNASequence& seq) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileParseException("Cannot open file: " + filename);
    }

    file << ">" << seq.getName() << "\n";
    std::string sequenceStr = seq.getSequenceString();
    
    for (int i = 0; i < seq.length(); i += 70) {
        file << sequenceStr.substr(i, 70) << "\n";
    }
}

std::vector<std::pair<std::string, std::string>> FastaParser::parseRaw(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileParseException("Cannot open file: " + filename);
    }

    std::vector<std::pair<std::string, std::string>> sequences;
    std::string line, currentName, currentSeq;

    while (std::getline(file, line)) {
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());
        
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));

        if (line.empty()) {
            continue;
        }

        if (line[0] == '>') {
            if (!currentName.empty()) {
                sequences.push_back({currentName, currentSeq});
            }

            currentName = line.substr(1);
            
            currentName.erase(std::find_if(currentName.rbegin(), currentName.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), currentName.end());
            currentName.erase(currentName.begin(), std::find_if(currentName.begin(), currentName.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            
            size_t spacePos = currentName.find(' ');
            if (spacePos != std::string::npos) {
                currentName = currentName.substr(0, spacePos);
            }
            
            currentSeq = "";
        } else {
            line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char ch) {
                return std::isspace(ch) || !std::isalpha(ch);
            }), line.end());
            
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            
            currentSeq += line;
        }
    }

    if (!currentName.empty()) {
        sequences.push_back({currentName, currentSeq});
    }

    return sequences;
}
