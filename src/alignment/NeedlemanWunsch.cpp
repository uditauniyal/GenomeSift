#include "../../include/alignment/NeedlemanWunsch.h"
#include <algorithm>

AlignmentResult NeedlemanWunsch::align(const Sequence& seq1, const Sequence& seq2, const ScoringMatrix<int>& scoring) {
    std::string s1 = seq1.getSequenceString();
    std::string s2 = seq2.getSequenceString();
    
    int m = s1.length();
    int n = s2.length();
    
    std::vector<std::vector<int>> matrix(m + 1, std::vector<int>(n + 1, 0));
    
    fillMatrix(matrix, s1, s2, scoring);
    
    auto traceback_result = traceback(matrix, s1, s2, scoring);
    std::string aligned1 = traceback_result.first;
    std::string aligned2 = traceback_result.second;
    
    int finalScore = matrix[m][n];
    
    AlignmentResult result(aligned1, aligned2, seq1.getName(), seq2.getName(), finalScore, getName());
    result.setDpMatrix(matrix);
    
    return result;
}

std::string NeedlemanWunsch::getName() const {
    return "Needleman-Wunsch (Global)";
}

void NeedlemanWunsch::fillMatrix(std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2, const ScoringMatrix<int>& scoring) {
    int m = s1.length();
    int n = s2.length();
    
    // Initialize first row
    for (int j = 0; j <= n; ++j) {
        matrix[0][j] = j * scoring.getGapPenalty();
    }
    
    // Initialize first column
    for (int i = 0; i <= m; ++i) {
        matrix[i][0] = i * scoring.getGapPenalty();
    }
    
    // Fill rest of matrix
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int diagonal = matrix[i-1][j-1] + scoring.getScore(s1[i-1], s2[j-1]);
            int up = matrix[i-1][j] + scoring.getGapPenalty();
            int left = matrix[i][j-1] + scoring.getGapPenalty();
            
            matrix[i][j] = std::max({diagonal, up, left});
        }
    }
}

std::pair<std::string, std::string> NeedlemanWunsch::traceback(const std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2, const ScoringMatrix<int>& scoring) {
    int i = s1.length();
    int j = s2.length();
    
    std::string aligned1 = "";
    std::string aligned2 = "";
    
    while (i > 0 && j > 0) {
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
    
    // Process remaining along the edges
    while (i > 0) {
        aligned1 = s1[i-1] + aligned1;
        aligned2 = '-' + aligned2;
        i--;
    }
    
    while (j > 0) {
        aligned1 = '-' + aligned1;
        aligned2 = s2[j-1] + aligned2;
        j--;
    }
    
    return {aligned1, aligned2};
}
