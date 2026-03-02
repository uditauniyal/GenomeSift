#pragma once

#include <map>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

template<typename T>
class ScoringMatrix {
private:
    std::map<std::pair<char, char>, T> scores;
    T matchScore;
    T mismatchScore;
    T gapPenalty;

public:
    ScoringMatrix(T match, T mismatch, T gap) 
        : matchScore(match), mismatchScore(mismatch), gapPenalty(gap) {
        
        char bases[] = {'A', 'T', 'G', 'C', 'U'};
        for (char a : bases) {
            for (char b : bases) {
                if (a == b) {
                    scores[{a, b}] = matchScore;
                } else {
                    scores[{a, b}] = mismatchScore;
                }
            }
        }
    }

    T getScore(char a, char b) const {
        char upperA = std::toupper(a);
        char upperB = std::toupper(b);
        
        auto it = scores.find({upperA, upperB});
        if (it != scores.end()) {
            return it->second;
        }
        
        return (upperA == upperB) ? matchScore : mismatchScore;
    }

    T getGapPenalty() const {
        return gapPenalty;
    }

    T getMatchScore() const {
        return matchScore;
    }

    T getMismatchScore() const {
        return mismatchScore;
    }

    void setScore(char a, char b, T score) {
        char upperA = std::toupper(a);
        char upperB = std::toupper(b);
        
        scores[{upperA, upperB}] = score;
        scores[{upperB, upperA}] = score;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open scoring matrix file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip empty or comment lines

            std::istringstream iss(line);
            char a, b;
            T score;

            if (iss >> a >> b >> score) {
                setScore(a, b, score);
            }
        }
    }
};
