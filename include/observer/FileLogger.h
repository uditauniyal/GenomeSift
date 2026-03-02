#pragma once

#include "MutationObserver.h"
#include <string>
#include <fstream>

class FileLogger : public MutationObserver {
private:
    std::string filename;
    bool appendMode;

public:
    FileLogger(const std::string& filename, bool append = true);

    void onMutationDetected(const Mutation& mutation) override;

    std::string getObserverName() const override;

    std::string getFilename() const;
};
