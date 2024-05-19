#ifndef INVALID_MOVE_EXCEPTION_H
#define INVALID_MOVE_EXCEPTION_H

#include <exception>
#include <string>

class InvalidMoveException : public std::exception {
private:
    std::string message;

public:
    InvalidMoveException(const std::string& msg);
    const char* what() const noexcept override;
};

#endif // INVALID_MOVE_EXCEPTION_H
