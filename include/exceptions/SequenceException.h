#pragma once

#include <stdexcept>
#include <string>

class SequenceException : public std::runtime_error {
public:
    explicit SequenceException(const std::string& msg)
        : std::runtime_error(msg) {}

    virtual ~SequenceException() = default;

    virtual const char* what() const noexcept override {
        return std::runtime_error::what();
    }
};
