#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <QString>

class SqlException : public std::runtime_error {
public:
    explicit SqlException(const QString& message)
        : std::runtime_error(message.toStdString()) {}
};

#endif // EXCEPTIONS_H
