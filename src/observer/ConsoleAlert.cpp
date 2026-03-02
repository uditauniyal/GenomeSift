#include "../../include/observer/ConsoleAlert.h"
#include <iostream>

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"
#define BOLD    "\033[1m"

ConsoleAlert::ConsoleAlert(bool colors) : useColors(colors) {}

void ConsoleAlert::onMutationDetected(const Mutation& mutation) {
    std::string color = "";
    if (useColors) {
        switch (mutation.getSeverity()) {
            case Severity::CRITICAL:
            case Severity::HIGH:
                color = RED;
                break;
            case Severity::MEDIUM:
                color = YELLOW;
                break;
            case Severity::LOW:
            default:
                color = GREEN;
                break;
        }
    }

    if (useColors) {
        std::cout << BOLD << color << "[ALERT] " << RESET
                  << color << mutation.getType() << " at position "
                  << mutation.getPosition() << ": "
                  << mutation.getReference() << " -> " << mutation.getVariant()
                  << RESET << std::endl;
                  
        if (mutation.getDiseaseAssociation() != "Unknown") {
            std::cout << color << " Disease: " << mutation.getDiseaseAssociation()
                      << " | Severity: " << mutation.getSeverityString() << RESET << std::endl;
        }
    } else {
        std::cout << "[ALERT] " << mutation.getType() << " at position "
                  << mutation.getPosition() << ": "
                  << mutation.getReference() << " -> " << mutation.getVariant() << std::endl;
                  
        if (mutation.getDiseaseAssociation() != "Unknown") {
            std::cout << " Disease: " << mutation.getDiseaseAssociation()
                      << " | Severity: " << mutation.getSeverityString() << std::endl;
        }
    }
}

std::string ConsoleAlert::getObserverName() const {
    return "ConsoleAlert";
}
