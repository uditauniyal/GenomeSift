#pragma once

#include "AlignmentStrategy.h"
#include "AlignmentResult.h"
#include "../scoring/ScoringMatrix.h"
#include "../sequence/Sequence.h"

#include <vector>
#include <memory>
#include <iostream>

// Forward declarations
class Mutation;
class MutationObserver;

class Aligner {
private:
    AlignmentStrategy* strategy;
    std::vector<MutationObserver*> observers;
    ScoringMatrix<int> scoring;

public:
    Aligner();
    
    Aligner(AlignmentStrategy* strat, const ScoringMatrix<int>& sc);
    
    void setStrategy(AlignmentStrategy* s);
    
    void setScoringMatrix(const ScoringMatrix<int>& sc);
    
    AlignmentStrategy* getStrategy() const;
    
    AlignmentResult performAlignment(const Sequence& seq1, const Sequence& seq2);
    
    void addObserver(MutationObserver* obs);
    
    void removeObserver(MutationObserver* obs);
    
    void notifyObservers(const Mutation& m);
    
    int getObserverCount() const;
};
