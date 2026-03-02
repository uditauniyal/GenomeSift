#include "../../include/io/ReportGenerator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

void ReportGenerator::generateReport(
    const std::string& outputPath,
    const AlignmentResult& alignment,
    const std::vector<Mutation>& mutations,
    const std::string& patientId
) {
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file for writing report: " << outputPath << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = std::ctime(&time);
    if (!timestamp.empty() && timestamp.back() == '\n') {
        timestamp.pop_back();
    }

    file << "╔══════════════════════════════════════════════════════╗\n"
         << "║        GenomeSift - Mutation Analysis Report         ║\n"
         << "╚══════════════════════════════════════════════════════╝\n\n"
         << "Patient ID: " << patientId << "\n"
         << "Date: " << timestamp << "\n"
         << "Algorithm: " << alignment.getAlgorithmUsed() << "\n\n"
         << "── Alignment Summary ──────────────────────────────\n"
         << "Score: " << alignment.getScore() << "\n"
         << "Identity: " << std::fixed << std::setprecision(2) << alignment.getIdentityPercentage() << "%\n"
         << "Matches: " << alignment.getMatches() << "  Mismatches: " << alignment.getMismatches() << "  Gaps: " << alignment.getGaps() << "\n\n"
         << "── Alignment Visualization ────────────────────────\n";

    std::string visualization = alignment.getAlignmentVisualization();
    size_t firstNewline = visualization.find('\n');
    size_t secondNewline = visualization.find('\n', firstNewline + 1);
    
    std::string line1 = visualization.substr(0, firstNewline);
    std::string line2 = visualization.substr(firstNewline + 1, secondNewline - firstNewline - 1);
    std::string line3 = visualization.substr(secondNewline + 1);

    if (line1.length() > 100) {
        file << line1.substr(0, 100) << "...\n"
             << line2.substr(0, 100) << "...\n"
             << line3.substr(0, 100) << "...\n\n";
    } else {
        file << visualization << "\n\n";
    }

    file << "── Mutations Detected: " << mutations.size() << " ────────────────────\n";
    int critical = 0, high = 0, medium = 0, low = 0;
    
    for (size_t i = 0; i < mutations.size(); ++i) {
        const auto& m = mutations[i];
        file << "#" << (i + 1) << " Position " << m.getPosition() << ": " 
             << m.getReference() << " -> " << m.getVariant() << " (" << m.getType() << ") "
             << "| Severity: " << m.getSeverityString() << " "
             << "| Disease: " << m.getDiseaseAssociation() << "\n";
             
        switch (m.getSeverity()) {
            case Severity::CRITICAL: critical++; break;
            case Severity::HIGH: high++; break;
            case Severity::MEDIUM: medium++; break;
            case Severity::LOW: low++; break;
        }
    }
    
    file << "\n── Summary by Severity ────────────────────────────\n"
         << "CRITICAL: " << critical << "\n"
         << "HIGH: " << high << "\n"
         << "MEDIUM: " << medium << "\n"
         << "LOW: " << low << "\n\n"
         << "── End of Report ──────────────────────────────────\n";
         
    file.close();
    std::cout << "[INFO] Report saved to: " << outputPath << std::endl;
}

std::string ReportGenerator::generateSummary(
    const AlignmentResult& alignment,
    const std::vector<Mutation>& mutations
) {
    int highCount = 0;
    for (const auto& m : mutations) {
        if (m.getSeverity() == Severity::HIGH || m.getSeverity() == Severity::CRITICAL) {
            highCount++;
        }
    }

    std::ostringstream oss;
    oss << "Algorithm: " << alignment.getAlgorithmUsed() << " | "
        << "Score: " << alignment.getScore() << " | "
        << "Identity: " << std::fixed << std::setprecision(2) << alignment.getIdentityPercentage() << "% | "
        << "Mutations: " << mutations.size() << " (" << highCount << " High/Critical)";
        
    return oss.str();
}
