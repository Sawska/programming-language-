#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include "token.h"

class Variable
{
public:

    Variable() = default;
    Variable(const std::string& varName, const TOKEN& varValue)
        : name(varName), value(varValue) {}


    std::string getName() const { return name; }
    TOKEN getValue() const { return value; }


    void setName(const std::string& varName) { name = varName; }
    void setValue(const TOKEN& varValue) { value = varValue; }

private:
    std::string name;
    TOKEN value;
};

#endif // VARIABLE_H
