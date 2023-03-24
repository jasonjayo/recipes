#ifndef MISSINGFILEEXCEPTION_H
#define MISSINGFILEEXCEPTION_H

#include <QString>


class MissingFileException : public std::exception {
private:
    QString fileName;
public:
    MissingFileException(QString fileName);
    const char* what() const noexcept;
};


#endif // MISSINGFILEEXCEPTION_H
