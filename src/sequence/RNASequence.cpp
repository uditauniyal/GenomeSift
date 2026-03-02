#include "../../include/sequence/RNASequence.h"
#include "../../include/sequence/ProteinSequence.h"
#include <map>
#include <string>

// Note: Constructor, validate(), getType(), and complement() are implemented inline in the header

ProteinSequence RNASequence::translate() const {
    std::string proteinString = "";
    
    for (size_t i = 0; i + 2 < sequence.length(); i += 3) {
        std::string codon = sequence.substr(i, 3);
        
        auto it = getCodonTable().find(codon);
        if (it != getCodonTable().end()) {
            char aminoAcid = it->second;
            if (aminoAcid == '*') {
                break; // Stop codon
            }
            proteinString += aminoAcid;
        } else {
            proteinString += 'X'; // Unknown codon
        }
    }
    
    return ProteinSequence(name + "_translated", proteinString);
}
