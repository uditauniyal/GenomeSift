#pragma once

#include "Sequence.h"
#include <string>

// Forward declaration
class RNASequence;

class DNASequence : public Sequence {
public:
    DNASequence(const std::string& name, const std::string& seq);

    bool validate() const override;

    std::string getType() const override;

    char complement(char base) const override;

    RNASequence transcribe() const;

    DNASequence reverseComplement() const;

    DNASequence operator+(const DNASequence& other) const;
};
