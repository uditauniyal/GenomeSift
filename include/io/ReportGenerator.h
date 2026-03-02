#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "../alignment/AlignmentResult.h"
#include "../mutation/Mutation.h"

class ReportGenerator {
public:
    static void generateReport(
        const std::string& outputPath,
        const AlignmentResult& alignment,
        const std::vector<Mutation>& mutations,
        const std::string& patientId = "Unknown"
    );

    static std::string generateSummary(
        const AlignmentResult& alignment,
        const std::vector<Mutation>& mutations
    );
};
