#include "../../include/mutation/Mutation.h"
#include <sstream>

std::string severityToString(Severity s) {
    switch (s) {
        case Severity::LOW: return "LOW";
        case Severity::MEDIUM: return "MEDIUM";
        case Severity::HIGH: return "HIGH";
        case Severity::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

Severity stringToSeverity(const std::string& s) {
    if (s == "CRITICAL") return Severity::CRITICAL;
    if (s == "HIGH") return Severity::HIGH;
    if (s == "MEDIUM") return Severity::MEDIUM;
    return Severity::LOW; // Default
}

Mutation::Mutation(int pos, char ref, char var, const std::string& type)
    : position(pos), reference(ref), variant(var), type(type), 
      severity(Severity::LOW), geneContext("Unknown"), diseaseAssociation("Unknown") {}

int Mutation::getPosition() const { return position; }
char Mutation::getReference() const { return reference; }
char Mutation::getVariant() const { return variant; }
std::string Mutation::getType() const { return type; }
Severity Mutation::getSeverity() const { return severity; }
std::string Mutation::getGeneContext() const { return geneContext; }
std::string Mutation::getDiseaseAssociation() const { return diseaseAssociation; }

void Mutation::setSeverity(Severity s) { severity = s; }
void Mutation::setGeneContext(const std::string& gene) { geneContext = gene; }
void Mutation::setDiseaseAssociation(const std::string& disease) { diseaseAssociation = disease; }

std::string Mutation::getSeverityString() const {
    return severityToString(severity);
}

std::string Mutation::toString() const {
    std::ostringstream oss;
    oss << "Position " << position << ": " << reference << " -> " << variant
        << " (" << type << ") | Severity: " << getSeverityString()
        << " | Gene: " << geneContext
        << " | Disease: " << diseaseAssociation;
    return oss.str();
}

bool Mutation::operator<(const Mutation& other) const {
    if (severity != other.severity) {
        return static_cast<int>(severity) < static_cast<int>(other.severity);
    }
    return position < other.position;
}

std::ostream& operator<<(std::ostream& os, const Mutation& m) {
    os << m.toString();
    return os;
}
