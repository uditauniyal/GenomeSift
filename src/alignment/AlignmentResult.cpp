#include "../../include/alignment/AlignmentResult.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

AlignmentResult::AlignmentResult() 
    : alignedSeq1(""), alignedSeq2(""), seq1Name(""), seq2Name(""), 
      score(0), matches(0), mismatches(0), gaps(0), algorithmUsed("") {}

AlignmentResult::AlignmentResult(const std::string& seq1, const std::string& seq2, 
                const std::string& name1, const std::string& name2, 
                int s, const std::string& algo)
    : alignedSeq1(seq1), alignedSeq2(seq2), seq1Name(name1), seq2Name(name2),
      score(s), matches(0), mismatches(0), gaps(0), algorithmUsed(algo) {
    calculateStats();
}

std::string AlignmentResult::getAlignedSeq1() const { return alignedSeq1; }
std::string AlignmentResult::getAlignedSeq2() const { return alignedSeq2; }
std::string AlignmentResult::getSeq1Name() const { return seq1Name; }
std::string AlignmentResult::getSeq2Name() const { return seq2Name; }
int AlignmentResult::getScore() const { return score; }
int AlignmentResult::getMatches() const { return matches; }
int AlignmentResult::getMismatches() const { return mismatches; }
int AlignmentResult::getGaps() const { return gaps; }
std::string AlignmentResult::getAlgorithmUsed() const { return algorithmUsed; }
std::vector<std::vector<int>> AlignmentResult::getDpMatrix() const { return dpMatrix; }

void AlignmentResult::setAlignedSeq1(const std::string& seq) { alignedSeq1 = seq; }
void AlignmentResult::setAlignedSeq2(const std::string& seq) { alignedSeq2 = seq; }
void AlignmentResult::setSeq1Name(const std::string& name) { seq1Name = name; }
void AlignmentResult::setSeq2Name(const std::string& name) { seq2Name = name; }
void AlignmentResult::setScore(int s) { score = s; }
void AlignmentResult::setMatches(int m) { matches = m; }
void AlignmentResult::setMismatches(int m) { mismatches = m; }
void AlignmentResult::setGaps(int g) { gaps = g; }
void AlignmentResult::setAlgorithmUsed(const std::string& algo) { algorithmUsed = algo; }
void AlignmentResult::setDpMatrix(const std::vector<std::vector<int>>& matrix) { dpMatrix = matrix; }

void AlignmentResult::calculateStats() {
    matches = 0;
    mismatches = 0;
    gaps = 0;

    int len = std::min(alignedSeq1.length(), alignedSeq2.length());
    
    for (int i = 0; i < len; ++i) {
        if (alignedSeq1[i] == '-' || alignedSeq2[i] == '-') {
            gaps++;
        } else if (std::toupper(alignedSeq1[i]) == std::toupper(alignedSeq2[i])) {
            matches++;
        } else {
            mismatches++;
        }
    }
}

double AlignmentResult::getIdentityPercentage() const {
    int total = matches + mismatches + gaps;
    if (total == 0) return 0.0;
    return (matches / static_cast<double>(total)) * 100.0;
}

std::string AlignmentResult::getAlignmentVisualization() const {
    std::ostringstream oss;
    
    std::string matchLine = "";
    int len = std::min(alignedSeq1.length(), alignedSeq2.length());
    
    for (int i = 0; i < len; ++i) {
        if (alignedSeq1[i] == '-' || alignedSeq2[i] == '-') {
            matchLine += " ";
        } else if (std::toupper(alignedSeq1[i]) == std::toupper(alignedSeq2[i])) {
            matchLine += "|";
        } else {
            matchLine += "x";
        }
    }

    oss << "Seq1: " << alignedSeq1 << "\n"
        << "      " << matchLine << "\n"
        << "Seq2: " << alignedSeq2;
        
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const AlignmentResult& result) {
    int total = result.matches + result.mismatches + result.gaps;
    
    os << "Algorithm: " << result.algorithmUsed << "\n"
       << "Score: " << result.score << "\n"
       << "Identity: " << std::fixed << std::setprecision(2) << result.getIdentityPercentage() << "% (" << result.matches << "/" << total << ")\n"
       << "Matches: " << result.matches << " Mismatches: " << result.mismatches << " Gaps: " << result.gaps << "\n\n"
       << result.getAlignmentVisualization();
       
    return os;
}
