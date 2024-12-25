#ifndef vm_H
#define vm_H


#include <string>
#include <memory>
#include "../jit/jit_compiler.h"

class VM
{
public:
    VM();
    ~VM();

    void execute(const std::string& funcName) const;
    void initGC() const;
    bool loadIR(const std::string& path) const;

private:
    std::unique_ptr<JITCompiler> jitCompiler;
};

#endif
