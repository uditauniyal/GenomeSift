#include "../../include/observer/SeverityFilter.h"
#include <iostream>

SeverityFilter::SeverityFilter(Severity min) : minSeverity(min) {}

void SeverityFilter::onMutationDetected(const Mutation& mutation) {
    if (static_cast<int>(mutation.getSeverity()) >= static_cast<int>(minSeverity)) {
        std::cout << "\033[1;31m[CRITICAL ALERT]\033[0m "
                  << mutation.getDiseaseAssociation()
                  << " detected at position " << mutation.getPosition()
                  << " (Severity: " << mutation.getSeverityString() << ")"
                  << std::endl;
    }
}

std::string SeverityFilter::getObserverName() const {
    return "SeverityFilter(min=" + severityToString(minSeverity) + ")";
}

void SeverityFilter::setMinSeverity(Severity s) {
    minSeverity = s;
}

Severity SeverityFilter::getMinSeverity() const {
    return minSeverity;
}
