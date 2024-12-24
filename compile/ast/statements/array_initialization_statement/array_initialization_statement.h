#ifndef ARRAY_INITIALIZATION_STATEMENT_H
#define ARRAY_INITIALIZATION_STATEMENT_H

#include "../../ast.h"


class ArrayInitializationStatement : public Statement {
    ValueType elemsType;
    std::string arrName;
    std::shared_ptr<Expression> size;
public:
    std::vector<std::shared_ptr<Expression> > elements;

public:
    ArrayInitializationStatement(ValueType elems_type, std::string arr_name,
        const std::shared_ptr<Expression> &size, const std::vector<std::shared_ptr<Expression>> &elements);

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitArrayInitializationStatement(this);
    }
};



#endif //ARRAY_INITIALIZATION_STATEMENT_H
