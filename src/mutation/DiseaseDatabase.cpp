#include "../../include/mutation/DiseaseDatabase.h"
#include <stdexcept>
#include <iostream>

std::string DiseaseDatabase::makeKey(const std::string& gene, int pos, char ref, char var) const {
    return gene + "_" + std::to_string(pos) + "_" + ref + "_" + var;
}

void DiseaseDatabase::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open disease database file: " + filename);
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '\r') continue;
        
        std::istringstream iss(line);
        std::string gene, posStr, refStr, altStr, disease, severityStr;
        
        if (std::getline(iss, gene, ',') &&
            std::getline(iss, posStr, ',') &&
            std::getline(iss, refStr, ',') &&
            std::getline(iss, altStr, ',') &&
            std::getline(iss, disease, ',') &&
            std::getline(iss, severityStr, ',')) {
            
            // Clean strings (handle empty strings or extra whitespace if any)
            if (!severityStr.empty() && severityStr.back() == '\r') {
                severityStr.pop_back();
            }

            int position = std::stoi(posStr);
            char ref = refStr.empty() ? ' ' : refStr[0];
            char alt = altStr.empty() ? ' ' : altStr[0];
            Severity sev = stringToSeverity(severityStr);

            DiseaseEntry entry = {gene, position, ref, alt, disease, sev};
            entries.push_back(entry);
            
            std::string key = makeKey(gene, position, ref, alt);
            lookupMap[key] = entry;
        }
    }
    
    std::cout << "[INFO] Loaded " << entries.size() << " disease entries from database" << std::endl;
}

bool DiseaseDatabase::hasEntry(const std::string& gene, int position, char ref, char var) const {
    std::string key = makeKey(gene, position, ref, var);
    return lookupMap.count(key) > 0;
}

DiseaseEntry DiseaseDatabase::getEntry(const std::string& gene, int position, char ref, char var) const {
    std::string key = makeKey(gene, position, ref, var);
    auto it = lookupMap.find(key);
    if (it == lookupMap.end()) {
        throw std::runtime_error("Disease entry not found: " + key);
    }
    return it->second;
}

void DiseaseDatabase::enrichMutation(Mutation& mutation) const {
    std::string key = makeKey(mutation.getGeneContext(), mutation.getPosition(), 
                              mutation.getReference(), mutation.getVariant());
                              
    auto it = lookupMap.find(key);
    if (it != lookupMap.end()) {
        mutation.setDiseaseAssociation(it->second.diseaseName);
        mutation.setSeverity(it->second.severity);
    }
}

int DiseaseDatabase::getEntryCount() const {
    return static_cast<int>(entries.size());
}

std::vector<DiseaseEntry> DiseaseDatabase::getAllEntries() const {
    return entries;
}
