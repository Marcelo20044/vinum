#ifndef STRING_VALUE_H
#define STRING_VALUE_H

#include "../value.h"
#include <string>

class StringValue : public Value {
private:
    std::string value;

public:
    explicit StringValue(const std::string& value);

    double asNumber() const override;
    std::string asString() const override;
};

#endif // STRING_VALUE_H
