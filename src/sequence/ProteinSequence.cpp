#include "../../include/sequence/ProteinSequence.h"

const std::unordered_set<char> ProteinSequence::validAminoAcids = {
    'A','R','N','D','C','E','Q','G','H','I','L','K','M','F','P','S','T','W','Y','V','*'
};

ProteinSequence::ProteinSequence(const std::string& name, const std::string& seq) 
    : Sequence(name, seq) {
    if (!validate()) {
        for (int i = 0; i < length(); ++i) {
            char base = sequence[i];
            if (validAminoAcids.find(base) == validAminoAcids.end()) {
                throw InvalidBaseException(base, i);
            }
        }
    }
}

bool ProteinSequence::validate() const {
    for (char base : sequence) {
        if (validAminoAcids.find(base) == validAminoAcids.end()) {
            return false;
        }
    }
    return true;
}

std::string ProteinSequence::getType() const {
    return "Protein";
}

char ProteinSequence::complement(char base) const {
    throw SequenceException("Protein sequences do not have complements");
}

double ProteinSequence::molecularWeight() const {
    return static_cast<double>(length()) * 110.0;
}
