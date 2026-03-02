#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <vector>

#include "../include/sequence/DNASequence.h"
#include "../include/sequence/RNASequence.h"
#include "../include/sequence/ProteinSequence.h"

#include "../include/alignment/Aligner.h"
#include "../include/alignment/NeedlemanWunsch.h"
#include "../include/alignment/SmithWaterman.h"
#include "../include/alignment/AlignmentResult.h"

#include "../include/scoring/ScoringMatrix.h"
#include "../include/scoring/SubstitutionMatrix.h"

#include "../include/mutation/Mutation.h"
#include "../include/mutation/MutationDetector.h"
#include "../include/mutation/DiseaseDatabase.h"

#include "../include/observer/ConsoleAlert.h"
#include "../include/observer/FileLogger.h"
#include "../include/observer/SeverityFilter.h"

#include "../include/io/FastaParser.h"
#include "../include/io/ReportGenerator.h"

void displayMenu() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║                      GenomeSift                      ║\n";
    std::cout << "║ DNA Sequence Alignment & Mutation Detection Engine   ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n\n";
    std::cout << "1. Load Reference Sequence (FASTA)\n";
    std::cout << "2. Load Patient Sequence (FASTA)\n";
    std::cout << "3. Select Alignment Algorithm\n";
    std::cout << "4. Configure Scoring Matrix\n";
    std::cout << "5. Run Alignment & Detect Mutations\n";
    std::cout << "6. View Mutation Report\n";
    std::cout << "7. Save Report to File\n";
    std::cout << "8. Configure Observers\n";
    std::cout << "9. Transcribe DNA -> RNA -> Protein\n";
    std::cout << "0. Exit\n";
    std::cout << "\nChoose an option: ";
}

void clearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pauseAction() {
    std::cout << "\nPress Enter to continue...";
    clearCin();
    std::cin.get();
}

int main(int argc, char* argv[]) {
    // Check for JSON mode FIRST
    bool jsonMode = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--json") {
            jsonMode = true;
            break;
        }
    }

    if (jsonMode) {
        try {
            std::string refPath, patientPath, algoType = "global", gene = "HBB";
            int match = 1, mismatch = -1, gap = -2;

            for (int i = 1; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg == "--ref" && i + 1 < argc) refPath = argv[++i];
                else if (arg == "--patient" && i + 1 < argc) patientPath = argv[++i];
                else if (arg == "--algorithm" && i + 1 < argc) algoType = argv[++i];
                else if (arg == "--gene" && i + 1 < argc) gene = argv[++i];
                else if (arg == "--match" && i + 1 < argc) match = std::stoi(argv[++i]);
                else if (arg == "--mismatch" && i + 1 < argc) mismatch = std::stoi(argv[++i]);
                else if (arg == "--gap" && i + 1 < argc) gap = std::stoi(argv[++i]);
            }

            if (refPath.empty() || patientPath.empty()) {
                throw std::runtime_error("Missing --ref or --patient arguments");
            }

            auto refSeqs = FastaParser::parse(refPath);
            auto patSeqs = FastaParser::parse(patientPath);

            if (refSeqs.empty() || patSeqs.empty()) {
                throw std::runtime_error("Could not load sequences");
            }

            DNASequence referenceSeq(refSeqs[0].getName(), refSeqs[0].getSequenceString());
            DNASequence patientSeq(patSeqs[0].getName(), patSeqs[0].getSequenceString());

            Aligner aligner;
            NeedlemanWunsch nw;
            SmithWaterman sw;
            
            if (algoType == "local") {
                aligner.setStrategy(&sw);
            } else {
                aligner.setStrategy(&nw);
            }
            
            aligner.setScoringMatrix(ScoringMatrix<int>(match, mismatch, gap));

            AlignmentResult result = aligner.performAlignment(referenceSeq, patientSeq);

            DiseaseDatabase diseaseDb;
            try {
                // Keep standard error clear if possible, but load normally
                diseaseDb.loadFromCSV("data/disease_db.csv");
            } catch(...) {}

            MutationDetector detector(gene, &diseaseDb);
            std::vector<Mutation> mutations = detector.detectMutations(result);

            RNASequence rna = referenceSeq.transcribe();
            ProteinSequence prot = rna.translate();

            int critical_count = 0, high_count = 0, medium_count = 0, low_count = 0;
            std::string mutJson = "";
            for (size_t i = 0; i < mutations.size(); ++i) {
                const auto& m = mutations[i];
                std::string sevStr;
                switch(m.getSeverity()) {
                    case Severity::CRITICAL: sevStr = "CRITICAL"; critical_count++; break;
                    case Severity::HIGH: sevStr = "HIGH"; high_count++; break;
                    case Severity::MEDIUM: sevStr = "MEDIUM"; medium_count++; break;
                    case Severity::LOW: sevStr = "LOW"; low_count++; break;
                }
                
                mutJson += "    {\n";
                mutJson += "      \"position\": " + std::to_string(m.getPosition()) + ",\n";
                // Properly escape strings
                std::string refStr(1, m.getReference());
                std::string varStr(1, m.getVariant());
                mutJson += "      \"reference\": \"" + refStr + "\",\n";
                mutJson += "      \"variant\": \"" + varStr + "\",\n";
                mutJson += "      \"type\": \"" + m.getType() + "\",\n";
                mutJson += "      \"severity\": \"" + sevStr + "\",\n";
                mutJson += "      \"gene\": \"" + m.getGeneContext() + "\",\n";
                
                // If the disease string is empty, fall back to "Unknown"
                std::string diseaseAssoc = m.getDiseaseAssociation();
                if (diseaseAssoc.empty()) {
                    diseaseAssoc = "Unknown";
                }
                
                mutJson += "      \"disease\": \"" + diseaseAssoc + "\"\n";
                mutJson += "    }";
                if (i < mutations.size() - 1) mutJson += ",";
                mutJson += "\n";
            }

            auto dp = result.getDpMatrix();
            std::string dpJson = "[";
            bool truncated = false;
            size_t maxRows = std::min((size_t)50, dp.size());
            if (maxRows < dp.size()) truncated = true;
            
            for (size_t i = 0; i < maxRows; ++i) {
                dpJson += "[";
                size_t maxCols = std::min((size_t)50, dp[i].size());
                if (maxCols < dp[i].size()) truncated = true;
                for (size_t j = 0; j < maxCols; ++j) {
                    dpJson += std::to_string(dp[i][j]);
                    if (j < maxCols - 1) dpJson += ",";
                }
                dpJson += "]";
                if (i < maxRows - 1) dpJson += ",";
            }
            dpJson += "]";

            // Generate clean JSON output to stdout
            std::cout << "{\n"
                      << "  \"status\": \"success\",\n"
                      << "  \"alignment\": {\n"
                      << "    \"algorithm\": \"" << result.getAlgorithmUsed() << "\",\n"
                      << "    \"score\": " << result.getScore() << ",\n"
                      << "    \"identity_percentage\": " << result.getIdentityPercentage() << ",\n"
                      << "    \"matches\": " << result.getMatches() << ",\n"
                      << "    \"mismatches\": " << result.getMismatches() << ",\n"
                      << "    \"gaps\": " << result.getGaps() << ",\n"
                      << "    \"aligned_seq1\": \"" << result.getAlignedSeq1() << "\",\n"
                      << "    \"aligned_seq2\": \"" << result.getAlignedSeq2() << "\",\n"
                      << "    \"seq1_name\": \"" << result.getSeq1Name() << "\",\n"
                      << "    \"seq2_name\": \"" << result.getSeq2Name() << "\",\n"
                      << "    \"seq1_length\": " << referenceSeq.length() << ",\n"
                      << "    \"seq2_length\": " << patientSeq.length() << ",\n"
                      << "    \"dp_matrix_truncated\": " << (truncated ? "true" : "false") << ",\n"
                      << "    \"dp_matrix\": " << dpJson << "\n"
                      << "  },\n"
                      << "  \"mutations\": [\n" << mutJson << "  ],\n"
                      << "  \"transcription\": {\n"
                      << "    \"dna\": \"" << referenceSeq.getSequenceString() << "\",\n"
                      << "    \"rna\": \"" << rna.getSequenceString() << "\",\n"
                      << "    \"protein\": \"" << prot.getSequenceString() << "\"\n"
                      << "  },\n"
                      << "  \"summary\": {\n"
                      << "    \"total_mutations\": " << mutations.size() << ",\n"
                      << "    \"critical_count\": " << critical_count << ",\n"
                      << "    \"high_count\": " << high_count << ",\n"
                      << "    \"medium_count\": " << medium_count << ",\n"
                      << "    \"low_count\": " << low_count << "\n"
                      << "  }\n"
                      << "}\n";

            return 0; // Exit program immediately after printing JSON
        } catch (const std::exception& e) {
            std::cout << "{\n  \"status\": \"error\",\n  \"message\": \"" << e.what() << "\"\n}\n";
            return 1;
        }
    }
    // Global State
    DNASequence* referenceSeq = nullptr;
    DNASequence* patientSeq = nullptr;
    
    NeedlemanWunsch nwAlgorithm;
    SmithWaterman swAlgorithm;
    Aligner aligner;
    aligner.setStrategy(&nwAlgorithm); // Default
    
    DiseaseDatabase diseaseDb;
    try {
        diseaseDb.loadFromCSV("data/disease_db.csv");
    } catch (const std::exception& e) {
        std::cerr << "[WARNING] Could not load disease database: " << e.what() << std::endl;
    }
    
    MutationDetector detector("Unknown", &diseaseDb);
    
    ConsoleAlert consoleAlert(true);
    FileLogger fileLogger("reports/mutation_log.txt", true);
    SeverityFilter severityFilter(Severity::HIGH);
    
    aligner.addObserver(&consoleAlert); // Default observer
    
    AlignmentResult lastResult;
    std::vector<Mutation> lastMutations;
    
    // Quick Mode (Command Line Args)
    if (argc > 1) {
        std::string refPath, patientPath, algoType;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--ref" && i + 1 < argc) refPath = argv[++i];
            else if (arg == "--patient" && i + 1 < argc) patientPath = argv[++i];
            else if (arg == "--algorithm" && i + 1 < argc) algoType = argv[++i];
        }
        
        if (!refPath.empty() && !patientPath.empty()) {
            try {
                auto refSeqs = FastaParser::parse(refPath);
                auto patSeqs = FastaParser::parse(patientPath);
                
                if (refSeqs.empty() || patSeqs.empty()) {
                    std::cerr << "[ERROR] Could not load sequences." << std::endl;
                    return 1;
                }
                
                referenceSeq = new DNASequence(refSeqs[0].getName(), refSeqs[0].getSequenceString());
                patientSeq = new DNASequence(patSeqs[0].getName(), patSeqs[0].getSequenceString());
                
                // IMPORTANT: Set Context for diseases database mappings to be resolved!
                detector.setGeneContext(referenceSeq->getName());
                
                if (algoType == "local") {
                    aligner.setStrategy(&swAlgorithm);
                }
                
                lastResult = aligner.performAlignment(*referenceSeq, *patientSeq);
                lastMutations = detector.detectMutations(lastResult);
                
                for (const auto& m : lastMutations) {
                    aligner.notifyObservers(m);
                }
                
                std::cout << "\n" << ReportGenerator::generateSummary(lastResult, lastMutations) << std::endl;
                
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] " << e.what() << std::endl;
                return 1;
            }
        }
        
        delete referenceSeq;
        delete patientSeq;
        return 0;
    }

    // Interactive Menu Loop
    int choice = -1;
    while (choice != 0) {
        displayMenu();
        if (!(std::cin >> choice)) {
            clearCin();
            std::cout << "[ERROR] Invalid numeric input. Try again.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    std::cout << "Enter path to reference FASTA: ";
                    std::string path;
                    std::cin >> path;
                    auto seqs = FastaParser::parse(path);
                    if (!seqs.empty()) {
                        delete referenceSeq;
                        referenceSeq = new DNASequence(seqs[0].getName(), seqs[0].getSequenceString());
                        std::cout << "[INFO] Loaded: " << referenceSeq->getName() << " (" << referenceSeq->length() << " bp)\n";
                        detector.setGeneContext(referenceSeq->getName());
                    } else {
                        std::cout << "[ERROR] No sequences found in file.\n";
                    }
                    break;
                }
                case 2: {
                    std::cout << "Enter path to patient FASTA: ";
                    std::string path;
                    std::cin >> path;
                    auto seqs = FastaParser::parse(path);
                    if (!seqs.empty()) {
                        delete patientSeq;
                        patientSeq = new DNASequence(seqs[0].getName(), seqs[0].getSequenceString());
                        std::cout << "[INFO] Loaded: " << patientSeq->getName() << " (" << patientSeq->length() << " bp)\n";
                    } else {
                        std::cout << "[ERROR] No sequences found in file.\n";
                    }
                    break;
                }
                case 3: {
                    std::cout << "Algorithm: [G]lobal (Needleman-Wunsch) / [L]ocal (Smith-Waterman): ";
                    char algo;
                    std::cin >> algo;
                    if (std::toupper(algo) == 'G') {
                        aligner.setStrategy(&nwAlgorithm);
                        std::cout << "[INFO] Set to Global Alignment.\n";
                    } else if (std::toupper(algo) == 'L') {
                        aligner.setStrategy(&swAlgorithm);
                        std::cout << "[INFO] Set to Local Alignment.\n";
                    } else {
                        std::cout << "[ERROR] Invalid choice.\n";
                    }
                    break;
                }
                case 4: {
                    std::cout << "Scoring: [S]imple / [W]eighted DNA / [C]ustom: ";
                    char sc;
                    std::cin >> sc;
                    if (std::toupper(sc) == 'S') {
                        aligner.setScoringMatrix(SubstitutionMatrix::simpleDNA());
                        std::cout << "[INFO] Set to Simple DNA Scoring Matrix.\n";
                    } else if (std::toupper(sc) == 'W') {
                        aligner.setScoringMatrix(SubstitutionMatrix::weightedDNA());
                        std::cout << "[INFO] Set to Weighted DNA Scoring Matrix.\n";
                    } else if (std::toupper(sc) == 'C') {
                        int m, mm, g;
                        std::cout << "Enter Match Score: "; std::cin >> m;
                        std::cout << "Enter Mismatch Score: "; std::cin >> mm;
                        std::cout << "Enter Gap Penalty: "; std::cin >> g;
                        aligner.setScoringMatrix(ScoringMatrix<int>(m, mm, g));
                        std::cout << "[INFO] Set to Custom Scoring Matrix.\n";
                    } else {
                        std::cout << "[ERROR] Invalid choice.\n";
                    }
                    break;
                }
                case 5: {
                    if (!referenceSeq || !patientSeq) {
                        std::cout << "[ERROR] Must load BOTH Reference and Patient sequences first.\n";
                        break;
                    }
                    
                    std::cout << "[INFO] Starting alignment...\n";
                    lastResult = aligner.performAlignment(*referenceSeq, *patientSeq);
                    std::cout << "[INFO] Detecting mutations...\n";
                    lastMutations = detector.detectMutations(lastResult);
                    
                    std::cout << "\n========== RESULTS ==========\n";
                    std::cout << lastResult << "\n\n";
                    
                    std::cout << "========== MUTATIONS ==========\n";
                    for (const auto& m : lastMutations) {
                        aligner.notifyObservers(m);
                    }
                    break;
                }
                case 6: {
                    if (lastResult.getAlignedSeq1().empty()) {
                        std::cout << "[ERROR] No alignment performed yet.\n";
                        break;
                    }
                    std::cout << "\n========== SUMMARY ==========\n";
                    std::cout << ReportGenerator::generateSummary(lastResult, lastMutations) << "\n";
                    break;
                }
                case 7: {
                    if (lastResult.getAlignedSeq1().empty()) {
                        std::cout << "[ERROR] No alignment performed yet.\n";
                        break;
                    }
                    std::cout << "Enter output file path: ";
                    std::string path;
                    std::cin >> path;
                    std::string patName = patientSeq ? patientSeq->getName() : "Unknown";
                    ReportGenerator::generateReport(path, lastResult, lastMutations, patName);
                    break;
                }
                case 8: {
                    std::cout << "[1] Toggle Console Alert\n";
                    std::cout << "[2] Toggle File Logger\n";
                    std::cout << "[3] Toggle Severity Filter\n";
                    int obsChoice;
                    std::cin >> obsChoice;
                    
                    if (obsChoice == 1) {
                        // Very naive toggle
                        aligner.removeObserver(&consoleAlert);
                        std::cout << "[INFO] Console Alert Disabled.\n";
                        // Note: For a robust toggle we'd need to check if it's already in the vector. 
                        // Assuming simple usage for demo.
                    } else if (obsChoice == 2) {
                        aligner.addObserver(&fileLogger);
                        std::cout << "[INFO] File Logger Enabled.\n";
                    } else if (obsChoice == 3) {
                        aligner.addObserver(&severityFilter);
                        std::cout << "[INFO] Severity Filter Enabled.\n";
                    }
                    break;
                }
                case 9: {
                    if (!referenceSeq) {
                        std::cout << "[ERROR] Must load Reference Sequence first.\n";
                        break;
                    }
                    
                    std::cout << "\n--- TRANSCRIPTION ---\n";
                    RNASequence rna = referenceSeq->transcribe();
                    std::cout << "DNA: " << referenceSeq->getSequenceString() << "\n";
                    std::cout << "RNA: " << rna.getSequenceString() << "\n";
                    
                    std::cout << "\n--- TRANSLATION ---\n";
                    ProteinSequence prot = rna.translate();
                    std::cout << "Protein: " << prot.getSequenceString() << "\n";
                    break;
                }
                case 0:
                    std::cout << "[INFO] Exiting GenomeSift. Goodbye!\n";
                    break;
                default:
                    std::cout << "[ERROR] Invalid choice. Try again.\n";
                    break;
            }
            if (choice != 0) pauseAction();
            
        } catch (const std::exception& e) {
            std::cerr << "\n[CRITICAL ERROR] " << e.what() << "\n";
            if (choice != 0) pauseAction();
        }
    }

    delete referenceSeq;
    delete patientSeq;
    
    return 0;
}
