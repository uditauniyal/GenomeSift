#include "../../include/observer/FileLogger.h"
#include <iostream>
#include <ctime>
#include <chrono>

FileLogger::FileLogger(const std::string& filename, bool append)
    : filename(filename), appendMode(append) {}

void FileLogger::onMutationDetected(const Mutation& mutation) {
    std::ofstream file;
    if (appendMode) {
        file.open(filename, std::ios::app);
    } else {
        file.open(filename, std::ios::out);
    }

    if (!file.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = std::ctime(&time);
    
    // Remove the trailing newline character from ctime
    if (!timestamp.empty() && timestamp.back() == '\n') {
        timestamp.pop_back();
    }

    file << "[" << timestamp << "] " << mutation.toString() << "\n";
    // File automatically closed when std::ofstream goes out of scope
}

std::string FileLogger::getObserverName() const {
    return "FileLogger(" + filename + ")";
}

std::string FileLogger::getFilename() const {
    return filename;
}
