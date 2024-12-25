#ifndef VINUM_VARIABLEINFO_H
#define VINUM_VARIABLEINFO_H

#include "../../lib/value.h"

class VariableInfo {
public:
    std::shared_ptr<Value> value;
    int modifications;
};


#endif //VINUM_VARIABLEINFO_H
