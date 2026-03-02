#pragma once

#include <vector>
#include <queue>
#include <string>

#include "Mutation.h"
#include "DiseaseDatabase.h"
#include "../alignment/AlignmentResult.h"

class MutationDetector {
private:
    DiseaseDatabase* diseaseDb;
    std::string geneContext;

public:
    MutationDetector(const std::string& gene = "Unknown", DiseaseDatabase* db = nullptr);

    void setDiseaseDatabase(DiseaseDatabase* db);
    void setGeneContext(const std::string& gene);

    std::vector<Mutation> detectMutations(const AlignmentResult& alignment);

    std::priority_queue<Mutation> detectAndRank(const AlignmentResult& alignment);

    int countBySeverity(const std::vector<Mutation>& mutations, Severity severity) const;
};
