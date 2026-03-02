#include "../../include/mutation/MutationDetector.h"
#include <algorithm>
#include <cctype>

MutationDetector::MutationDetector(const std::string& gene, DiseaseDatabase* db) 
    : diseaseDb(db), geneContext(gene) {}

void MutationDetector::setDiseaseDatabase(DiseaseDatabase* db) {
    diseaseDb = db;
}

void MutationDetector::setGeneContext(const std::string& gene) {
    geneContext = gene;
}

std::vector<Mutation> MutationDetector::detectMutations(const AlignmentResult& alignment) {
    std::string alignedSeq1 = alignment.getAlignedSeq1(); // Reference
    std::string alignedSeq2 = alignment.getAlignedSeq2(); // Patient
    
    std::vector<Mutation> mutations;
    int refPos = 0;
    
    int len = std::min(alignedSeq1.length(), alignedSeq2.length());
    
    for (int i = 0; i < len; ++i) {
        char ref = alignedSeq1[i];
        char patient = alignedSeq2[i];
        
        bool isMutation = false;
        Mutation m(refPos, ref, patient, "Unknown");
        
        if (ref == '-' && patient != '-') {
            m = Mutation(refPos, '-', patient, "Insertion");
            isMutation = true;
        } else if (ref != '-' && patient == '-') {
            m = Mutation(refPos, ref, '-', "Deletion");
            isMutation = true;
            refPos++;
        } else if (ref != '-' && patient != '-' && std::toupper(ref) != std::toupper(patient)) {
            m = Mutation(refPos, ref, patient, "SNP");
            isMutation = true;
            refPos++;
        } else {
            // Match
            if (ref != '-') {
                refPos++;
            }
        }
        
        if (isMutation) {
            m.setGeneContext(geneContext);
            if (diseaseDb != nullptr) {
                diseaseDb->enrichMutation(m);
            }
            mutations.push_back(m);
        }
    }
    
    return mutations;
}

std::priority_queue<Mutation> MutationDetector::detectAndRank(const AlignmentResult& alignment) {
    auto mutations = detectMutations(alignment);
    std::priority_queue<Mutation> pq;
    
    for (const auto& m : mutations) {
        pq.push(m);
    }
    
    return pq;
}

int MutationDetector::countBySeverity(const std::vector<Mutation>& mutations, Severity severity) const {
    int count = 0;
    for (const auto& m : mutations) {
        if (m.getSeverity() == severity) {
            count++;
        }
    }
    return count;
}
