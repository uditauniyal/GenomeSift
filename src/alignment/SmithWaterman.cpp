#include "../../include/alignment/SmithWaterman.h"
#include <algorithm>

AlignmentResult SmithWaterman::align(const Sequence& seq1, const Sequence& seq2, const ScoringMatrix<int>& scoring) {
    std::string s1 = seq1.getSequenceString();
    std::string s2 = seq2.getSequenceString();
    
    int m = s1.length();
    int n = s2.length();
    
    std::vector<std::vector<int>> matrix(m + 1, std::vector<int>(n + 1, 0));
    
    int maxScore = 0;
    int maxI = 0;
    int maxJ = 0;
    
    fillMatrix(matrix, s1, s2, scoring, maxScore, maxI, maxJ);
    
    auto traceback_result = traceback(matrix, s1, s2, scoring, maxI, maxJ);
    std::string aligned1 = traceback_result.first;
    std::string aligned2 = traceback_result.second;
    
    AlignmentResult result(aligned1, aligned2, seq1.getName(), seq2.getName(), maxScore, getName());
    result.setDpMatrix(matrix);
    
    return result;
}

std::string SmithWaterman::getName() const {
    return "Smith-Waterman (Local)";
}

void SmithWaterman::fillMatrix(std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2, const ScoringMatrix<int>& scoring, int& maxScore, int& maxI, int& maxJ) {
    int m = s1.length();
    int n = s2.length();
    
    // First row and column are already 0
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int diagonal = matrix[i-1][j-1] + scoring.getScore(s1[i-1], s2[j-1]);
            int up = matrix[i-1][j] + scoring.getGapPenalty();
            int left = matrix[i][j-1] + scoring.getGapPenalty();
            
            matrix[i][j] = std::max({diagonal, up, left, 0});
            
            if (matrix[i][j] > maxScore) {
                maxScore = matrix[i][j];
                maxI = i;
                maxJ = j;
            }
        }
    }
}

std::pair<std::string, std::string> SmithWaterman::traceback(const std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2, const ScoringMatrix<int>& scoring, int startI, int startJ) {
    int i = startI;
    int j = startJ;
    
    std::string aligned1 = "";
    std::string aligned2 = "";
    
    while (i > 0 && j > 0 && matrix[i][j] > 0) {
        int diagonal = matrix[i-1][j-1] + scoring.getScore(s1[i-1], s2[j-1]);
        
        if (matrix[i][j] == diagonal) {
            aligned1 = s1[i-1] + aligned1;
            aligned2 = s2[j-1] + aligned2;
            i--;
            j--;
        } else if (matrix[i][j] == matrix[i-1][j] + scoring.getGapPenalty()) {
            aligned1 = s1[i-1] + aligned1;
            aligned2 = '-' + aligned2;
            i--;
        } else {
            aligned1 = '-' + aligned1;
            aligned2 = s2[j-1] + aligned2;
            j--;
        }
    }
    
    return {aligned1, aligned2};
}
