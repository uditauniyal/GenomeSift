#pragma once

#include "SequenceException.h"
#include <string>

class FileParseException : public SequenceException {
private:
    std::string filename;
    int lineNumber;

public:
    explicit FileParseException(const std::string& msg)
        : SequenceException(msg), filename(""), lineNumber(-1) {}

    FileParseException(const std::string& filename, int lineNumber, const std::string& detail)
        : SequenceException("Parse error in '" + filename + "' at line " + std::to_string(lineNumber) + ": " + detail),
          filename(filename),
          lineNumber(lineNumber) {}

    std::string getFilename() const { return filename; }
    int getLineNumber() const { return lineNumber; }
};
