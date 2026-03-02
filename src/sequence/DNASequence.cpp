#include "../../include/sequence/DNASequence.h"
#include "../../include/sequence/RNASequence.h"
#include <algorithm>

DNASequence::DNASequence(const std::string& name, const std::string& seq) 
    : Sequence(name, seq) {
    if (!validate()) {
        for (int i = 0; i < length(); ++i) {
            char base = sequence[i];
            if (base != 'A' && base != 'T' && base != 'G' && base != 'C') {
                throw InvalidBaseException(base, i);
            }
        }
    }
}

bool DNASequence::validate() const {
    for (char base : sequence) {
        if (base != 'A' && base != 'T' && base != 'G' && base != 'C') {
            return false;
        }
    }
    return true;
}

std::string DNASequence::getType() const {
    return "DNA";
}

char DNASequence::complement(char base) const {
    switch (toupper(base)) {
        case 'A': return 'T';
        case 'T': return 'A';
        case 'G': return 'C';
        case 'C': return 'G';
        default: throw InvalidBaseException(base, -1);
    }
}

RNASequence DNASequence::transcribe() const {
    std::string transcribedString = sequence;
    std::replace(transcribedString.begin(), transcribedString.end(), 'T', 'U');
    return RNASequence(name + "_transcribed", transcribedString);
}

DNASequence DNASequence::reverseComplement() const {
    std::string reversed(sequence.rbegin(), sequence.rend());
    for (char& base : reversed) {
        base = complement(base);
    }
    return DNASequence(name + "_revcomp", reversed);
}

DNASequence DNASequence::operator+(const DNASequence& other) const {
    return DNASequence("merged", sequence + other.getSequenceString());
}
