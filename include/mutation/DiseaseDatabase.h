#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include "Mutation.h"

struct DiseaseEntry {
    std::string gene;
    int position;
    char referenceBase;
    char variantBase;
    std::string diseaseName;
    Severity severity;
};

class DiseaseDatabase {
private:
    std::vector<DiseaseEntry> entries;
    std::map<std::string, DiseaseEntry> lookupMap;

    std::string makeKey(const std::string& gene, int pos, char ref, char var) const;

public:
    DiseaseDatabase() = default;

    void loadFromCSV(const std::string& filename);

    bool hasEntry(const std::string& gene, int position, char ref, char var) const;

    DiseaseEntry getEntry(const std::string& gene, int position, char ref, char var) const;

    void enrichMutation(Mutation& mutation) const;

    int getEntryCount() const;

    std::vector<DiseaseEntry> getAllEntries() const;
};
