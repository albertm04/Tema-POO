#include "InvalidMoveException.h"

InvalidMoveException::InvalidMoveException(const std::string& msg) : message(msg) {}

const char* InvalidMoveException::what() const noexcept {
    return message.c_str();
}
