#pragma once

#include <string>
#include <vector>
#include <iostream>

class AlignmentResult {
private:
    std::string alignedSeq1;
    std::string alignedSeq2;
    std::string seq1Name;
    std::string seq2Name;
    int score;
    int matches;
    int mismatches;
    int gaps;
    std::string algorithmUsed;
    std::vector<std::vector<int>> dpMatrix;

public:
    AlignmentResult();

    AlignmentResult(const std::string& seq1, const std::string& seq2, 
                    const std::string& name1, const std::string& name2, 
                    int s, const std::string& algo);

    // Getters
    std::string getAlignedSeq1() const;
    std::string getAlignedSeq2() const;
    std::string getSeq1Name() const;
    std::string getSeq2Name() const;
    int getScore() const;
    int getMatches() const;
    int getMismatches() const;
    int getGaps() const;
    std::string getAlgorithmUsed() const;
    std::vector<std::vector<int>> getDpMatrix() const;

    // Setters
    void setAlignedSeq1(const std::string& seq);
    void setAlignedSeq2(const std::string& seq);
    void setSeq1Name(const std::string& name);
    void setSeq2Name(const std::string& name);
    void setScore(int s);
    void setMatches(int m);
    void setMismatches(int m);
    void setGaps(int g);
    void setAlgorithmUsed(const std::string& algo);
    void setDpMatrix(const std::vector<std::vector<int>>& matrix);

    void calculateStats();

    double getIdentityPercentage() const;

    std::string getAlignmentVisualization() const;

    friend std::ostream& operator<<(std::ostream& os, const AlignmentResult& result);
};
