#include "jit_compiler.h"


JITCompiler::JITCompiler() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    auto llJit = llvm::orc::LLJITBuilder().create();
    if (!llJit) {
        throw std::runtime_error("Failed to create LLJIT.");
    }

    jit = std::move(*llJit);
}

JITCompiler::~JITCompiler() {
    jit.reset();
}

bool JITCompiler::loadIR(const std::string& path) const
{
    llvm::SMDiagnostic error;

    auto context = std::make_unique<llvm::LLVMContext>();

    auto module = llvm::parseIRFile(path, error, *context);
    if (!module) {
        error.print("Failed to create module:", llvm::errs());
        return false;
    }

    if (auto err = jit->addIRModule(llvm::orc::ThreadSafeModule(std::move(module), std::move(context)))) {
        error.print("Failed to add IR module:", llvm::errs());
        llvm::errs() << "Failed to add IR module to JIT\n";
        return false;
    }

    return true;
}

JITCompiler::stdFunction JITCompiler::getCompiledFunction(const std::string& funcName) const
{
    auto symbol = jit->lookup(funcName);
    if (!symbol) {
        llvm::errs() << "Function '" << funcName << "' wasn't found in JIT.\n";
        return nullptr;
    }

    auto* funcPtr = llvm::jitTargetAddressToPointer<int (*)()>(symbol->getValue());
    return [funcPtr]() -> int {
        return funcPtr();
    };
}
