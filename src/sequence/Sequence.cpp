#include "../../include/sequence/Sequence.h"
#include <algorithm>
#include <cctype>

Sequence::Sequence(const std::string& name, const std::string& seq) 
    : name(name) {
    sequence = seq;
    std::transform(sequence.begin(), sequence.end(), sequence.begin(), ::toupper);
}

int Sequence::length() const {
    return static_cast<int>(sequence.length());
}

std::string Sequence::getName() const {
    return name;
}

std::string Sequence::getSequenceString() const {
    return sequence;
}

std::string Sequence::getSubsequence(int start, int end) const {
    if (start < 0 || end > length() || start >= end) {
        throw InvalidBaseException('?', start);
    }
    return sequence.substr(start, end - start);
}

double Sequence::gcContent() const {
    if (sequence.empty()) return 0.0;
    
    int gcCount = 0;
    for (char base : sequence) {
        if (base == 'G' || base == 'C') {
            gcCount++;
        }
    }
    return (static_cast<double>(gcCount) / sequence.length()) * 100.0;
}

bool Sequence::operator==(const Sequence& other) const {
    return sequence == other.sequence;
}

bool Sequence::operator!=(const Sequence& other) const {
    return !(*this == other);
}

char Sequence::operator[](int index) const {
    if (index < 0 || index >= length()) {
        throw InvalidBaseException('?', index);
    }
    return sequence[index];
}

// Note: operator<< is defined inline in the header file.
