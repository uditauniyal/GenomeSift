#pragma once

#include "AlignmentStrategy.h"
#include <vector>

class SmithWaterman : public AlignmentStrategy {
public:
    AlignmentResult align(const Sequence& seq1, const Sequence& seq2, const ScoringMatrix<int>& scoring) override;

    std::string getName() const override;

private:
    void fillMatrix(std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2, const ScoringMatrix<int>& scoring, int& maxScore, int& maxI, int& maxJ);

    std::pair<std::string, std::string> traceback(const std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2, const ScoringMatrix<int>& scoring, int startI, int startJ);
};
