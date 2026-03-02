#pragma once

#include "MutationObserver.h"
#include <iostream>
#include <string>

class ConsoleAlert : public MutationObserver {
private:
    bool useColors;

public:
    ConsoleAlert(bool colors = true);

    void onMutationDetected(const Mutation& mutation) override;

    std::string getObserverName() const override;
};
