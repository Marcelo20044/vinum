#include "vm.h"
#include <iostream>

VM::VM() : jitCompiler(std::make_unique<JITCompiler>()) {}

VM::~VM() = default;

void VM::execute(const std::string& funcName) const
{
    auto func = jitCompiler->getCompiledFunction(funcName);
    if (!func) {return;}

    int result = func();
    std::cout << std::endl << "JIT process finished with exit code " << result << std::endl;
}

bool VM::loadIR(const std::string& path) const
{
    return jitCompiler->loadIR(path);
}