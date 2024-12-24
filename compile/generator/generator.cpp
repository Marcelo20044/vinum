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
        } else {
            // Добавьте обработку других типов Statement
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

    // Убедимся, что printf объявлен
    llvm::Function *printfFunc = module->getFunction("printf");
    if (!printfFunc) {
        llvm::FunctionType *printfType = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(context),
                {llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)}, // Указатель на символы для строки
                true
        );
        printfFunc = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module.get());
    }

    // Форматная строка для printf
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

    // Создаём базовые блоки: для цикла (loop) и после цикла (afterloop)
    llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(context, "loop", function);
    llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context, "afterloop");

    // Генерация кода инициализации (stmt->initialization)
    if (auto initStmt = std::dynamic_pointer_cast<InitializationStatement>(stmt->initialization)) {
        llvm::Value *initValue = generateExpressionIR(initStmt->expression);
        llvm::Type *varType = resolveLLVMType(initStmt->varType);
        llvm::AllocaInst *alloca = builder.CreateAlloca(varType, nullptr, initStmt->varName);
        namedValues[initStmt->varName] = {varType, alloca};
        builder.CreateStore(initValue, alloca);
    } else {
        throw std::runtime_error("Expected InitializationStatement in for loop");
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
    } else {
        throw std::runtime_error("Expected AssignmentStatement in for loop increment");
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

    // Define argument types based on statement arguments
    std::vector<llvm::Type*> argTypes;
    for (auto &arg : stmt->args) {
        if (arg->type == ValueType::INT) {
            argTypes.push_back(llvm::Type::getInt32Ty(context));
        } else if (arg->type == ValueType::DOUBLE) {
            argTypes.push_back(llvm::Type::getDoubleTy(context));
        } else if (arg->type == ValueType::STRING) {
            argTypes.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0));
        } else if (arg->type == ValueType::LONG) {
            argTypes.push_back(llvm::Type::getInt64Ty(context));
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
    for (auto &arg : function->args()) {
        llvm::AllocaInst *alloca = builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        builder.CreateStore(&arg, alloca);
        namedValues[arg.getName().str()] = {arg.getType(), alloca};
    }

    generateBlockIR(std::dynamic_pointer_cast<BlockStatement>(stmt->body));

    // Ensure all basic blocks are terminated correctly
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

llvm::Value* Generator::generateExpressionIR(std::shared_ptr<Expression> &expr) {
    // Обработка бинарных выражений
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

    // Обработка унарных выражений
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

    // Обработка условных выражений
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

            // Использование Boehm GC для выделения памяти под строку
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
        VariableInfo var = namedValues[varExpr->name];

        return builder.CreateLoad(var.type, var.value, varExpr->name + "_load");
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