#ifndef STRINGABLE_H
#define STRINGABLE_H

#include <string>

class Stringable
{
public:
    virtual std::string to_short_string() = 0;
    virtual std::string to_long_string() = 0;
};

#endif // STRINGABLE_H
