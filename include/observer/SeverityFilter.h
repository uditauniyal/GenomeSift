#pragma once

#include "MutationObserver.h"
#include <iostream>

class SeverityFilter : public MutationObserver {
private:
    Severity minSeverity;

public:
    SeverityFilter(Severity min = Severity::MEDIUM);

    void onMutationDetected(const Mutation& mutation) override;

    std::string getObserverName() const override;

    void setMinSeverity(Severity s);

    Severity getMinSeverity() const;
};
