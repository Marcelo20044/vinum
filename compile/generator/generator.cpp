#include "generator.h"
#include "../ast/statements/assigment_statement/assigment_statement.h"
#include "../ast/statements/initialization_statement/initialization_statement.h"
#include "../ast/statements/print_statement/print_statement.h"
#include "../ast/statements/for_statement/for_statement.h"
#include "../ast/statements/if_statement/if_statement.h"
#include "../ast/statements/return_statement/return_statement.h"
#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include "../ast/expressions/binary_expression/binary_expression.h"
#include "../ast/expressions/conditional_expression/conditional_expression.h"
#include "../ast/expressions/functional_expression/functional_expression.h"
#include "../ast/expressions/unary_expression/unary_expression.h"
#include "../ast/expressions/value_expression/value_expression.h"
#include "../ast/expressions/variable_expression/variable_expression.h"
#include "../lib/double_value/double_value.h"
#include "../lib/int_value/int_value.h"
#include "../ast/statements/function_statement/function_statement.h"
#include "../ast/expressions/array_access_expression/array_access_expression.h"
#include <fstream>
#include <llvm/Support/FileSystem.h>


void Generator::generateIR(std::shared_ptr<BlockStatement> &block) {
    llvm::FunctionType *concatType = llvm::FunctionType::get(
            llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0),
            {llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)},
            false
    );

    llvm::Function::Create(
            concatType,
            llvm::Function::ExternalLinkage,
            "string_concat",
            module.get()
    );

    llvm::FunctionType *mainType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    llvm::Function *mainFunction = llvm::Function::Create(
            mainType,
            llvm::Function::ExternalLinkage,
            llvm::StringRef("main"),
            module.get()
    );

    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", mainFunction);
    builder.SetInsertPoint(entryBlock);

    generateBlockIR(block);

    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));

    saveToFile("output.ll");
}

void Generator::generateBlockIR(std::shared_ptr<BlockStatement> block) {
    auto statements = block->getStatements();
    for (const auto &statement : statements) {
        if (auto assignStmt = std::dynamic_pointer_cast<AssignmentStatement>(statement)) {
            generateAssignmentIR(assignStmt);
        } else if (auto initStmt = std::dynamic_pointer_cast<InitializationStatement>(statement)) {
            generateInitializationIR(initStmt);
        } else if (auto printStmt = std::dynamic_pointer_cast<PrintStatement>(statement)) {
            generatePrintIR(printStmt);
        } else if (auto returnStmt = std::dynamic_pointer_cast<ReturnStatement>(statement)) {
            generateReturnIR(returnStmt);
        } else if (auto funcStmt = std::dynamic_pointer_cast<FunctionStatement>(statement)) {
            generateFunctionIR(funcStmt);
        } else if (auto funcDefineStmt = std::dynamic_pointer_cast<FunctionDefineStatement>(statement)) {
            generateFunctionDefineIR(funcDefineStmt);
        } else if (auto ifStmt = std::dynamic_pointer_cast<IfStatement>(statement)) {
            generateIfIR(ifStmt);
        } else if (auto forStmt = std::dynamic_pointer_cast<ForStatement>(statement)) {
            generateForIR(forStmt);
        } else if (auto arrayInitStmt = std::dynamic_pointer_cast<ArrayInitializationStatement>(statement)) {
            generateArrayInitializationIR(arrayInitStmt);
        } else if (auto arrayAssignStmt = std::dynamic_pointer_cast<ArrayAssignmentStatement>(statement)) {
            generateArrayAssignmentIR(arrayAssignStmt);
        }
    }
}

void Generator::generateAssignmentIR(std::shared_ptr<AssignmentStatement> &stmt) {
    llvm::Value *value = generateExpressionIR(stmt->expression);

    auto it = namedValues.find(stmt->variable);
    if (it != namedValues.end()) {
        builder.CreateStore(value, it->second.value);
        return;
    }

    llvm::GlobalVariable *globalVar = module->getGlobalVariable(stmt->variable);
    if (!globalVar) {
        throw std::runtime_error("Variable '" + stmt->variable + "' not found");
    }

    builder.CreateStore(value, globalVar);
}

void Generator::generateInitializationIR(std::shared_ptr<InitializationStatement> &stmt) {
    llvm::Type *type;

    if (stmt->varType == ValueType::INT) {
        type = llvm::Type::getInt32Ty(context);
    } else if (stmt->varType == ValueType::DOUBLE) {
        type = llvm::Type::getDoubleTy(context);
    } else if (stmt->varType == ValueType::LONG) {
        type = llvm::Type::getInt64Ty(context);
        auto valExpr = std::dynamic_pointer_cast<ValueExpression>(stmt->expression);
        if (valExpr) {
            if (valExpr->value->getType() != ValueType::LONG) {
                valExpr->value->type = ValueType::LONG;
            }
            stmt->expression = valExpr;
        }
        if (!valExpr) {
            auto funExpr = std::dynamic_pointer_cast<FunctionalExpression>(stmt->expression);
            if (!funExpr) {
                throw std::runtime_error("No no no");
            } else {
                for (auto &arg : funExpr->arguments)
                {
                    auto valArg = std::dynamic_pointer_cast<ValueExpression>(arg);
                    if (valArg->value->getType() != ValueType::LONG) {
                        valArg->value->type = ValueType::LONG;
                    }
                }
                stmt->expression = funExpr;
            }
        }
    } else if (stmt->varType == ValueType::STRING) {
        type = llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0);
    } else {
        return;
    }

    llvm::Value *initialValue = generateExpressionIR(stmt->expression);
    if (stmt->varType == ValueType::STRING) {
        llvm::Function *gcMallocFunc = module->getFunction("GC_malloc");
        if (!gcMallocFunc) {
            llvm::FunctionType *gcMallocType = llvm::FunctionType::get(
                llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0),
                {llvm::Type::getInt64Ty(context)},
                false
            );
            gcMallocFunc = llvm::Function::Create(
                gcMallocType, llvm::Function::ExternalLinkage, "GC_malloc", module.get());
        }
        llvm::Value *stringSize = nullptr;
        if (initialValue) {
            llvm::Function *strlenFunc = module->getFunction("strlen");
            if (!strlenFunc) {
                llvm::FunctionType *strlenType = llvm::FunctionType::get(
                    llvm::Type::getInt64Ty(context),
                    {llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)},                    false                                                              // Не vararg
                );
                strlenFunc = llvm::Function::Create(strlenType, llvm::Function::ExternalLinkage, "strlen", module.get());
            }
            llvm::Value *strLen = builder.CreateCall(strlenFunc, {initialValue});
            stringSize = builder.CreateAdd(
                strLen,
                llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 1));
        } else {
            stringSize = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 1);
        }
        llvm::Value *gcAlloc = builder.CreateCall(gcMallocFunc, {stringSize});
        if (initialValue) {
            llvm::Value *srcPtr = builder.CreatePointerCast(initialValue, llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0));
            builder.CreateMemCpy(gcAlloc, llvm::MaybeAlign(), srcPtr, llvm::MaybeAlign(), stringSize);
        }
        initialValue = gcAlloc;
    } else if (!initialValue) {
        initialValue = llvm::Constant::getNullValue(type);
    }

    auto *alloca = builder.CreateAlloca(type, nullptr, stmt->varName);
    builder.CreateStore(initialValue, alloca);

    VariableInfo info{};
    info.type = type;
    info.value = alloca;
    namedValues[stmt->varName] = info;
}

void Generator::generatePrintIR(std::shared_ptr<PrintStatement> &stmt) {
    llvm::Value *value = generateExpressionIR(stmt->expression);

    llvm::Function *printfFunc = module->getFunction("printf");
    if (!printfFunc) {
        llvm::FunctionType *printfType = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(context),
                {llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)},
                true
        );
        printfFunc = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module.get());
    }

    llvm::Value *formatStr;

    if (value->getType()->isIntegerTy()) {
        if (value->getType()->getIntegerBitWidth() == 32) {
            formatStr = builder.CreateGlobalStringPtr("%d\n", "intFormat");
        } else if (value->getType()->getIntegerBitWidth() == 64) {
            formatStr = builder.CreateGlobalStringPtr("%ld\n", "longFormat");
        } else {
            throw std::runtime_error("Unsupported integer type for printing");
        }
    } else if (value->getType()->isDoubleTy()) {
        formatStr = builder.CreateGlobalStringPtr("%f\n", "doubleFormat");
    } else if (value->getType()->isPointerTy()) {
        formatStr = builder.CreateGlobalStringPtr("%s\n", "stringFormat");
    } else {
        throw std::runtime_error("Unsupported type for printing");
    }


    builder.CreateCall(printfFunc, {formatStr, value});
}

void Generator::generateReturnIR(std::shared_ptr<ReturnStatement> &stmt) {
    llvm::Value *value = generateExpressionIR(stmt->expression);
    builder.CreateRet(value);
}

void Generator::generateIfIR(std::shared_ptr<IfStatement> &stmt) {
    llvm::Value *cond = generateExpressionIR(stmt->expression);
    llvm::Function *function = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(context, "then", function);
    llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(context, "else");
    llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context, "ifcont");

    builder.CreateCondBr(cond, thenBB, elseBB);

    builder.SetInsertPoint(thenBB);
    generateBlockIR(std::dynamic_pointer_cast<BlockStatement>(stmt->ifStatement));
    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(mergeBB);
    }

    function->insert(function->end(), elseBB);
    builder.SetInsertPoint(elseBB);

    if (stmt->elseStatement) {
        generateBlockIR(std::dynamic_pointer_cast<BlockStatement>(stmt->elseStatement));
    }
    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(mergeBB);
    }

    function->insert(function->end(), mergeBB);
    builder.SetInsertPoint(mergeBB);
}

llvm::Type* Generator::resolveLLVMType(ValueType type) {
    switch (type) {
        case ValueType::INT:
            return llvm::Type::getInt32Ty(context);
        case ValueType::DOUBLE:
            return llvm::Type::getDoubleTy(context);
        case ValueType::LONG:
            return llvm::Type::getInt64Ty(context);
        default:
            throw std::runtime_error("Unsupported variable type");
    }
}

void Generator::generateForIR(std::shared_ptr<ForStatement> &stmt) {
    llvm::Function *function = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(context, "loop", function);
    llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context, "afterloop");

    if (auto initStmt = std::dynamic_pointer_cast<InitializationStatement>(stmt->initialization)) {
        llvm::Value *initValue = generateExpressionIR(initStmt->expression);
        llvm::Type *varType = resolveLLVMType(initStmt->varType);
        llvm::AllocaInst *alloca = builder.CreateAlloca(varType, nullptr, initStmt->varName);
        namedValues[initStmt->varName] = {varType, alloca};
        builder.CreateStore(initValue, alloca);
    }

    llvm::Value *cond = generateExpressionIR(stmt->termination);
    builder.CreateCondBr(cond, loopBB, afterBB);

    builder.SetInsertPoint(loopBB);

    if (stmt->statement) {
        generateBlockIR(std::dynamic_pointer_cast<BlockStatement>(stmt->statement));
    }

    if (auto incrementStmt = std::dynamic_pointer_cast<AssignmentStatement>(stmt->increment)) {
        llvm::Value *incrementValue = generateExpressionIR(incrementStmt->expression);
        auto varInfo = namedValues[incrementStmt->variable];
        if (!varInfo.value) {
            throw std::runtime_error("Variable '" + incrementStmt->variable + "' not found in for loop increment");
        }
        builder.CreateStore(incrementValue, varInfo.value);
    }

    llvm::Value *updatedCond = generateExpressionIR(stmt->termination);
    builder.CreateCondBr(updatedCond, loopBB, afterBB);

    function->insert(function->end(), afterBB);
    builder.SetInsertPoint(afterBB);
}

void Generator::generateFunctionIR(std::shared_ptr<FunctionStatement> &stmt) {
    llvm::Function *function = module->getFunction(stmt->function->name);
    if (!function) {
        throw std::runtime_error("Function not defined: " + stmt->function->name);
    }

    std::vector<llvm::Value*> args;
    for (auto &arg : stmt->function->arguments) {
        llvm::Value *argValue = generateExpressionIR(arg);
        if (!argValue) {
            throw std::runtime_error("Failed to generate IR for argument in function: " + stmt->function->name);
        }
        args.push_back(argValue);
    }

    if (args.size() != function->arg_size()) {
        throw std::runtime_error("Argument count mismatch for function: " + stmt->function->name);
    }

    builder.CreateCall(function, args);
}

void Generator::generateFunctionDefineIR(std::shared_ptr<FunctionDefineStatement> &stmt) {
    llvm::Type *returnType;
    if (stmt->returnType == ValueType::INT) {
        returnType = llvm::Type::getInt32Ty(context);
    } else if (stmt->returnType == ValueType::LONG) {
        returnType = llvm::Type::getInt64Ty(context);
    } else if (stmt->returnType == ValueType::DOUBLE) {
        returnType = llvm::Type::getDoubleTy(context);
    } else if (stmt->returnType == ValueType::STRING) {
        returnType = llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0);
    } else if (stmt->returnType == ValueType::VOID) {
        returnType = llvm::Type::getVoidTy(context);
    } else {
        throw std::runtime_error("Unsupported return type");
    }

    llvm::BasicBlock *previousBlock = builder.GetInsertBlock();

    std::vector<llvm::Type *> argTypes;
    for (auto &arg : stmt->args) {
        if (arg->type == ValueType::INT) {
            argTypes.push_back(llvm::Type::getInt32Ty(context));
        } else if (arg->type == ValueType::DOUBLE) {
            argTypes.push_back(llvm::Type::getDoubleTy(context));
        } else if (arg->type == ValueType::STRING) {
            argTypes.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0));
        } else if (arg->type == ValueType::LONG) {
            argTypes.push_back(llvm::Type::getInt64Ty(context));
        } else if (arg->type == ValueType::ARRAY) {
            argTypes.push_back(llvm::PointerType::get(llvm::Type::getInt32Ty(context), 0));
        } else {
            throw std::runtime_error("Unsupported argument type");
        }
    }

    llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, argTypes, false);
    llvm::Function *function = llvm::Function::Create(
            funcType,
            llvm::Function::ExternalLinkage,
            stmt->name,
            module.get()
    );

    unsigned idx = 0;
    for (auto &arg : function->args()) {
        arg.setName(stmt->args[idx++]->name);
    }

    llvm::BasicBlock *block = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(block);

    namedValues.clear();
    arrayNamedValues.clear();
    idx = 0;
    for (auto &arg : function->args()) {
        llvm::AllocaInst *alloca = builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        builder.CreateStore(&arg, alloca);

        if (stmt->args[idx]->type == ValueType::ARRAY) {
            arrayNamedValues[arg.getName().str()] = {llvm::ArrayType::get(llvm::Type::getInt32Ty(context), 5), llvm::Type::getInt32Ty(context), alloca};
        } else {
            namedValues[arg.getName().str()] = {arg.getType(), alloca};
        }
        ++idx;
    }

    generateBlockIR(std::dynamic_pointer_cast<BlockStatement>(stmt->body));

    for (auto &bb : *function) {
        if (!bb.getTerminator()) {
            builder.SetInsertPoint(&bb);
            if (returnType->isVoidTy()) {
                builder.CreateRetVoid();
            } else if (returnType->isIntegerTy()) {
                builder.CreateRet(llvm::ConstantInt::get(returnType, 0));
            } else if (returnType->isDoubleTy()) {
                builder.CreateRet(llvm::ConstantFP::get(returnType, 0.0));
            } else {
                throw std::runtime_error("Unhandled return type");
            }
        }
    }

    if (previousBlock) {
        builder.SetInsertPoint(previousBlock);
    } else {
        llvm::Function *mainFunction = module->getFunction("main");
        if (mainFunction) {
            llvm::BasicBlock &mainEntryBlock = mainFunction->getEntryBlock();
            builder.SetInsertPoint(&mainEntryBlock);
        }
    }
}

void Generator::generateArrayAssignmentIR(std::shared_ptr<ArrayAssignmentStatement> &stmt) {
    llvm::Value *indexValue = generateExpressionIR(stmt->index);
    if (!indexValue) {
        throw std::runtime_error("Failed to generate IR for array index");
    }

    ArrayVariableInfo arrayInfo = arrayNamedValues[stmt->variable];
    llvm::Value *arrayPtr = arrayInfo.value;
    llvm::Type *arrayType = arrayInfo.type;

    llvm::Value *elementPtr = builder.CreateGEP(
            arrayType,
            arrayPtr,
            {llvm::ConstantInt::get(context, llvm::APInt(32, 0)), indexValue},
            "array_elem_ptr"
    );

    llvm::Value *value = generateExpressionIR(stmt->expression);
    if (!value) {
        throw std::runtime_error("Failed to generate IR for assigned value");
    }

    builder.CreateStore(value, elementPtr);
}

void Generator::generateArrayInitializationIR(std::shared_ptr<ArrayInitializationStatement> &stmt) {
    llvm::Type *elementType = nullptr;
    if (stmt->elemsType == ValueType::INT) {
        elementType = llvm::Type::getInt32Ty(context);
    } else if (stmt->elemsType == ValueType::DOUBLE) {
        elementType = llvm::Type::getDoubleTy(context);
    } else {
        throw std::runtime_error("Unsupported array element type");
    }

    llvm::Value *sizeValue = generateExpressionIR(stmt->size);
    if (!sizeValue) {
        throw std::runtime_error("Failed to generate IR for array size");
    }

    llvm::Function *gcMallocFunc = module->getFunction("GC_malloc");
    if (!gcMallocFunc) {
        llvm::FunctionType *gcMallocType = llvm::FunctionType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0),
            {llvm::Type::getInt64Ty(context)},
            false
        );
        gcMallocFunc = llvm::Function::Create(
            gcMallocType,
            llvm::Function::ExternalLinkage,
            "GC_malloc",
            module.get()
        );
    }

    llvm::Value *elementSize = llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(context),
        elementType->getPrimitiveSizeInBits() / 8
    );

    llvm::Value *totalSize = builder.CreateMul(
        sizeValue,
        elementSize,
        "total_array_size"
    );

    llvm::Value *rawMemory = builder.CreateCall(gcMallocFunc, {totalSize}, "raw_memory");

    llvm::Value *typedMemory = builder.CreateBitCast(
        rawMemory,
        llvm::PointerType::getUnqual(elementType),
        "typed_memory"
    );
    llvm::ArrayType *arrayType = llvm::ArrayType::get(elementType, stmt->elements.size());

    arrayNamedValues[stmt->arrName] = {arrayType, elementType, typedMemory};

    for (size_t i = 0; i < stmt->elements.size(); ++i) {
        llvm::Value *elementValue = generateExpressionIR(stmt->elements[i]);
        if (!elementValue) {
            throw std::runtime_error("Failed to generate IR for array element");
        }

        llvm::Value *elementPtr = builder.CreateGEP(
            elementType,
            typedMemory,
            llvm::ConstantInt::get(context, llvm::APInt(64, i)),
            "array_elem_ptr"
        );

        builder.CreateStore(elementValue, elementPtr);
    }
}


llvm::Value* Generator::generateExpressionIR(std::shared_ptr<Expression> &expr) {
    if (auto binExpr = std::dynamic_pointer_cast<BinaryExpression>(expr)) {
        llvm::Value *left = generateExpressionIR(binExpr->expr1);
        llvm::Value *right = generateExpressionIR(binExpr->expr2);

        if (!left || !right) {
            throw std::runtime_error("Failed to generate IR for binary expression operands");
        }

        auto leftValExpr = std::dynamic_pointer_cast<ValueExpression>(binExpr->expr1);
        auto rightValExpr = std::dynamic_pointer_cast<ValueExpression>(binExpr->expr2);

        switch (binExpr->operation) {
            case '+':
                if (leftValExpr && leftValExpr->value->getType() == ValueType::DOUBLE || rightValExpr && rightValExpr->value->getType() == ValueType::DOUBLE) {
                    return builder.CreateFAdd(left, right, "addtmp");
                }
                return builder.CreateAdd(left, right, "addtmp");
            case '-':
                if (leftValExpr && leftValExpr->value->getType() == ValueType::DOUBLE || rightValExpr && rightValExpr->value->getType() == ValueType::DOUBLE) {
                    return builder.CreateFSub(left, right, "subtmp");
                }
                return builder.CreateSub(left, right, "subtmp");
            case '*':
                if (leftValExpr && leftValExpr->value->getType() == ValueType::DOUBLE || rightValExpr && rightValExpr->value->getType() == ValueType::DOUBLE) {
                    return builder.CreateFMul(left, right, "multmp");
                }
                return builder.CreateMul(left, right, "multmp");
            case '/':
                if (leftValExpr && leftValExpr->value->getType() == ValueType::DOUBLE || rightValExpr && rightValExpr->value->getType() == ValueType::DOUBLE) {
                    return builder.CreateFDiv(left, right, "divtmp");
                }
                return builder.CreateSDiv(left, right, "divtmp");
            case '%':
                if (leftValExpr && leftValExpr->value->getType() == ValueType::DOUBLE || rightValExpr && rightValExpr->value->getType() == ValueType::DOUBLE) {
                    return builder.CreateFRem(left, right, "remtmp");
                }
                return builder.CreateSRem(left, right, "remtmp");
            default:
                throw std::runtime_error("Unsupported binary operation");
            }
    }

    if (auto unaryExpr = std::dynamic_pointer_cast<UnaryExpression>(expr)) {
        llvm::Value *operand = generateExpressionIR(unaryExpr->expr1);

        if (!operand) {
            throw std::runtime_error("Failed to generate IR for unary expression operand");
        }

        switch (unaryExpr->operation) {
            case '-':
                return builder.CreateNeg(operand, "negtmp");
            default:
                throw std::runtime_error("Unsupported unary operation");
        }
    }

    if (auto condExpr = std::dynamic_pointer_cast<ConditionalExpression>(expr)) {
        llvm::Value *left = generateExpressionIR(condExpr->expr1);
        llvm::Value *right = generateExpressionIR(condExpr->expr2);

        if (!left || !right) {
            throw std::runtime_error("Failed to generate IR for conditional expression operands");
        }

        switch (condExpr->operation) {
            case ConditionalExpression::Operator::EQUALS:
                return builder.CreateICmpEQ(left, right, "eqtmp");
            case ConditionalExpression::Operator::NOT_EQUALS:
                return builder.CreateICmpNE(left, right, "netmp");
            case ConditionalExpression::Operator::LT:
                return builder.CreateICmpSLT(left, right, "lttmp");
            case ConditionalExpression::Operator::LTEQ:
                return builder.CreateICmpSLE(left, right, "ltetmp");
            case ConditionalExpression::Operator::GT:
                return builder.CreateICmpSGT(left, right, "gttmp");
            case ConditionalExpression::Operator::GTEQ:
                return builder.CreateICmpSGE(left, right, "gtetmp");
            default:
                throw std::runtime_error("Unsupported conditional operation");
        }
    }

    if (auto valueExpr = std::dynamic_pointer_cast<ValueExpression>(expr)) {
        if (valueExpr->value->getType() == ValueType::INT) {
            return llvm::ConstantInt::get(context, llvm::APInt(32, valueExpr->value->asInt()));
        }

        if (valueExpr->value->getType() == ValueType::LONG) {
            return llvm::ConstantInt::get(context, llvm::APInt(64, valueExpr->value->asLong()));
        }

        if (valueExpr->value->getType() == ValueType::DOUBLE) {
            return llvm::ConstantFP::get(context, llvm::APFloat(valueExpr->value->asDouble()));
        }

        if (valueExpr->value->getType() == ValueType::STRING) {
            const std::string &strValue = valueExpr->value->asString();

            llvm::Type *charType = llvm::Type::getInt8Ty(context);
            llvm::Function *gcMallocFunc = module->getFunction("GC_malloc");
            if (!gcMallocFunc) {
                llvm::FunctionType *gcMallocType = llvm::FunctionType::get(
                    llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0),
                    {llvm::Type::getInt64Ty(context)},
                    false
                );
                gcMallocFunc = llvm::Function::Create(
                    gcMallocType, llvm::Function::ExternalLinkage, "GC_malloc", module.get());
            }
            llvm::Value *strLen = llvm::ConstantInt::get(context, llvm::APInt(32, strValue.size() + 1));
            llvm::Value *mallocCall = builder.CreateCall(gcMallocFunc, {strLen}, "strAlloc");
            llvm::Value *strConst = llvm::ConstantDataArray::getString(context, strValue);

            builder.CreateStore(strConst, mallocCall);

            return builder.CreateBitCast(mallocCall, llvm::PointerType::get(charType, 0));
        }

        throw std::runtime_error("Unsupported value type in ValueExpression");
    }

    if (auto varExpr = std::dynamic_pointer_cast<VariableExpression>(expr)) {
        if (namedValues.count(varExpr->name)) {
            VariableInfo var = namedValues[varExpr->name];
            if (var.value == nullptr || var.type == nullptr) {
                throw std::runtime_error("Variable not found or uninitialized: " + varExpr->name);
            }

            return builder.CreateLoad(var.type, var.value, varExpr->name + "_load");
        }

        if (arrayNamedValues.count(varExpr->name)) {
            ArrayVariableInfo varArray = arrayNamedValues[varExpr->name];
            if (varArray.value == nullptr || varArray.elementType == nullptr) {
                throw std::runtime_error("Array variable not found or uninitialized: " + varExpr->name);
            }

            llvm::Value *elementPtr = builder.CreateGEP(
                    varArray.type,
                    varArray.value,
                    {llvm::ConstantInt::get(context, llvm::APInt(32, 0)), llvm::ConstantInt::get(context, llvm::APInt(32, 0))},
                    varExpr->name + "_first_elem_ptr"
            );

            return builder.CreateLoad(varArray.elementType, elementPtr, varExpr->name + "_first_elem_load");
        }

        throw std::runtime_error("Variable not found: " + varExpr->name);
    }


    if (auto funcExpr = std::dynamic_pointer_cast<FunctionalExpression>(expr)) {
        llvm::Function *function = module->getFunction(funcExpr->name);
        if (!function) {
            throw std::runtime_error("Function " + funcExpr->name + " is not defined");
        }

        std::vector<llvm::Value*> args;
        for (auto &argExpr : funcExpr->arguments) {
            llvm::Value *argValue = generateExpressionIR(argExpr);
            if (!argValue) {
                throw std::runtime_error("Failed to generate IR for function argument");
            }
            args.push_back(argValue);
        }

        return builder.CreateCall(function, args, funcExpr->name + "_call");
    }

    if (auto arrayAccessExpr = std::dynamic_pointer_cast<ArrayAccessExpression>(expr)) {
        llvm::Value *indexValue = generateExpressionIR(arrayAccessExpr->index);
        if (!indexValue) {
            throw std::runtime_error("Failed to generate IR for array index");
        }

        ArrayVariableInfo arrayInfo = arrayNamedValues[arrayAccessExpr->variable];
        llvm::Value *arrayPtr = arrayInfo.value;
        llvm::Type *arrayType = arrayInfo.type;
        llvm::Type *elementType = arrayInfo.elementType;

        llvm::Value *elementPtr = builder.CreateGEP(
                arrayType,
                arrayPtr,
                {llvm::ConstantInt::get(context, llvm::APInt(32, 0)), indexValue},
                "array_elem_ptr"
        );

        return builder.CreateLoad(elementType, elementPtr, "array_elem_load");
    }

    throw std::runtime_error("Unsupported expression type");
}

void Generator::saveToFile(const std::string &filename) {
    std::error_code ec;
    llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::OF_None);
    if (ec) {
        llvm::errs() << "Could not open file: " << ec.message();
        return;
    }
    module->print(dest, nullptr);
}