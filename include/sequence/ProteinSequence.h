#pragma once

#include "Sequence.h"
#include <string>
#include <unordered_set>

class ProteinSequence : public Sequence {
private:
    static const std::unordered_set<char> validAminoAcids;

public:
    ProteinSequence(const std::string& name, const std::string& seq);

    bool validate() const override;

    std::string getType() const override;

    char complement(char base) const override;

    double molecularWeight() const;
};
