#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "../exceptions/InvalidBaseException.h"

class Sequence {
protected:
    std::string name;
    std::string sequence;

public:
    Sequence(const std::string& name, const std::string& seq);

    virtual ~Sequence() = default;

    // Pure virtual methods
    virtual bool validate() const = 0;
    virtual std::string getType() const = 0;
    virtual char complement(char base) const = 0;

    // Concrete methods
    int length() const;
    std::string getName() const;
    std::string getSequenceString() const;
    std::string getSubsequence(int start, int end) const;
    double gcContent() const;

    // Operator overloading
    bool operator==(const Sequence& other) const;
    bool operator!=(const Sequence& other) const;
    char operator[](int index) const;
    
    // Sequence operator+(const Sequence& other) const 
    // Implementation for concatenation would be appropriate in derived classes.

    friend std::ostream& operator<<(std::ostream& os, const Sequence& seq) {
        os << ">" << seq.name << "\n" << seq.sequence;
        return os;
    }
};
