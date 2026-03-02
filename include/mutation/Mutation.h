#pragma once

#include <string>
#include <iostream>

enum class Severity { LOW, MEDIUM, HIGH, CRITICAL };

std::string severityToString(Severity s);
Severity stringToSeverity(const std::string& s);

class Mutation {
private:
    int position;
    char reference;
    char variant;
    std::string type;
    Severity severity;
    std::string geneContext;
    std::string diseaseAssociation;

public:
    Mutation(int pos, char ref, char var, const std::string& type);

    // Getters
    int getPosition() const;
    char getReference() const;
    char getVariant() const;
    std::string getType() const;
    Severity getSeverity() const;
    std::string getGeneContext() const;
    std::string getDiseaseAssociation() const;

    // Setters
    void setSeverity(Severity s);
    void setGeneContext(const std::string& gene);
    void setDiseaseAssociation(const std::string& disease);

    std::string getSeverityString() const;
    
    std::string toString() const;

    bool operator<(const Mutation& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Mutation& m);
};
