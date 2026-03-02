#pragma once

#include <string>
#include "../sequence/Sequence.h"
#include "../scoring/ScoringMatrix.h"
#include "AlignmentResult.h"

class AlignmentStrategy {
public:
    virtual ~AlignmentStrategy() = default;

    virtual AlignmentResult align(const Sequence& seq1, const Sequence& seq2, const ScoringMatrix<int>& scoring) = 0;

    virtual std::string getName() const = 0;
};
