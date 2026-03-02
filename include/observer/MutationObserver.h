#pragma once

#include "../mutation/Mutation.h"
#include <string>

class MutationObserver {
public:
    virtual ~MutationObserver() = default;

    virtual void onMutationDetected(const Mutation& mutation) = 0;

    virtual std::string getObserverName() const = 0;
};
