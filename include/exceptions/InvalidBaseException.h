#pragma once

#include "SequenceException.h"
#include <string>

class InvalidBaseException : public SequenceException {
private:
    char invalidBase;
    int position;

public:
    InvalidBaseException(char base, int pos)
        : SequenceException("Invalid base '" + std::string(1, base) + "' found at position " + std::to_string(pos)),
          invalidBase(base),
          position(pos) {}

    char getInvalidBase() const { return invalidBase; }
    int getPosition() const { return position; }
};
