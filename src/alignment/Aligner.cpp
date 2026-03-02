#include "../../include/alignment/Aligner.h"
#include "../../include/mutation/Mutation.h" // Expected to be provided later
#include "../../include/observer/MutationObserver.h" // Expected to be provided later
#include <algorithm>
#include <stdexcept>

Aligner::Aligner() 
    : strategy(nullptr), scoring(1, -1, -2) {}

Aligner::Aligner(AlignmentStrategy* strat, const ScoringMatrix<int>& sc) 
    : strategy(strat), scoring(sc) {}

void Aligner::setStrategy(AlignmentStrategy* s) {
    strategy = s;
}

void Aligner::setScoringMatrix(const ScoringMatrix<int>& sc) {
    scoring = sc;
}

AlignmentStrategy* Aligner::getStrategy() const {
    return strategy;
}

AlignmentResult Aligner::performAlignment(const Sequence& seq1, const Sequence& seq2) {
    if (strategy == nullptr) {
        throw std::runtime_error("No alignment strategy set");
    }
    
    auto result = strategy->align(seq1, seq2, scoring);
    
    std::cout << "[INFO] Alignment completed using " << strategy->getName() << std::endl;
    
    return result;
}

void Aligner::addObserver(MutationObserver* obs) {
    observers.push_back(obs);
}

void Aligner::removeObserver(MutationObserver* obs) {
    auto it = std::find(observers.begin(), observers.end(), obs);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Aligner::notifyObservers(const Mutation& m) {
    for (auto* obs : observers) {
        obs->onMutationDetected(m);
    }
}

int Aligner::getObserverCount() const {
    return static_cast<int>(observers.size());
}
