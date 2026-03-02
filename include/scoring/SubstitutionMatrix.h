#pragma once

#include "ScoringMatrix.h"

namespace SubstitutionMatrix {

    inline ScoringMatrix<int> simpleDNA() {
        return ScoringMatrix<int>(1, -1, -2);
    }

    inline ScoringMatrix<int> weightedDNA() {
        ScoringMatrix<int> sm(2, -1, -2);
        
        // Transitions (A<->G, C<->T) are more common, less penalty (-1)
        sm.setScore('A', 'G', -1);
        sm.setScore('C', 'T', -1);
        
        // Transversions (A<->C, A<->T, G<->C, G<->T) are less common, more penalty (-2)
        sm.setScore('A', 'C', -2);
        sm.setScore('A', 'T', -2);
        sm.setScore('G', 'C', -2);
        sm.setScore('G', 'T', -2);
        
        return sm;
    }

    inline ScoringMatrix<double> blosum62Simplified() {
        return ScoringMatrix<double>(4.0, -1.0, -4.0);
    }

} // namespace SubstitutionMatrix
