#ifndef JIT_COMPILER_H
#define JIT_COMPILER_H

#include <string>
#include <functional>
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/ExecutionEngine/Orc/Shared/TargetProcessControlTypes.h>


class JITCompiler {
public:
    typedef std::function<int()> stdFunction;

    JITCompiler();
    ~JITCompiler();

    bool loadIR(const std::string& path) const;
    stdFunction getCompiledFunction(const std::string& funcName) const;

private:
    std::unique_ptr<llvm::orc::LLJIT> jit;
};

#endif
