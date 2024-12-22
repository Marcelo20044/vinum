#ifndef VINUM_GENERATOR_H
#define VINUM_GENERATOR_H

#include "../ast/ast.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "../ast/statements/assigment_statement/assigment_statement.h"
#include "../ast/statements/initialization_statement/initialization_statement.h"
#include "../ast/statements/block_statement/block_statement.h"
#include "../ast/statements/print_statement/print_statement.h"
#include "../ast/statements/return_statement/return_statement.h"
#include "../ast/statements/if_statement/if_statement.h"
#include "../ast/statements/for_statement/for_statement.h"
#include "../ast/statements/function_statement/function_statement.h"
#include "../ast/statements/function_define_statement/function_define_statement.h"

struct VariableInfo {
    llvm::Type* type;
    llvm::Value* value;
};

class Generator {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::unordered_map<std::string, VariableInfo> namedValues;

public:
    Generator()
            : builder(context), module(std::make_unique<llvm::Module>("program", context)) {}

    void generateIR(std::shared_ptr<BlockStatement>& block);

private:
    void generateBlockIR(std::shared_ptr<BlockStatement> block);
    void generateAssignmentIR(std::shared_ptr<AssignmentStatement> &stmt);
    void generateInitializationIR(std::shared_ptr<InitializationStatement> &stmt);
    void generatePrintIR(std::shared_ptr<PrintStatement> &stmt);
    void generateReturnIR(std::shared_ptr<ReturnStatement> &stmt);
    void generateIfIR(std::shared_ptr<IfStatement> &stmt);
    void generateForIR(std::shared_ptr<ForStatement> &stmt);
    void generateFunctionIR(std::shared_ptr<FunctionStatement> &stmt);
    void generateFunctionDefineIR(std::shared_ptr<FunctionDefineStatement> &stmt);
    llvm::Value *generateExpressionIR(std::shared_ptr<Expression> &expr);
    void saveToFile(const std::string &filename);
    llvm::Type* resolveLLVMType(ValueType type);
};

#endif //VINUM_GENERATOR_H
