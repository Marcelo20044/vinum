cmake_minimum_required(VERSION 3.30)
project(vinum)

set(CMAKE_CXX_STANDARD 20)

add_executable(vinum main.cpp compile/lexer/lexer.cpp
        compile/ast/expressions/binary_expression/binary_expression.cpp
        compile/ast/expressions/conditional_expression/conditional_expression.cpp
        compile/ast/expressions/functional_expression/functional_expression.cpp
        compile/ast/expressions/unary_expression/unary_expression.cpp
        compile/ast/expressions/value_expression/value_expression.cpp
        compile/ast/expressions/variable_expression/variable_expression.cpp
        compile/ast/statements/assigment_statement/assigment_statement.cpp
        compile/ast/statements/block_statement/block_statement.cpp
        compile/ast/statements/break_statement/break_statement.cpp
        compile/ast/statements/continue_statement/continue_statement.cpp
        compile/ast/statements/for_statement/for_statement.cpp
        compile/ast/statements/function_define_statement/function_define_statement.cpp
        compile/ast/statements/function_statement/function_statement.cpp
        compile/ast/statements/if_statement/if_statement.cpp
        compile/ast/statements/print_statement/print_statement.cpp
        compile/ast/statements/return_statement/return_statement.cpp
        compile/lib/builtin/functions/functions.cpp
        compile/lib/builtin/variables/variables.cpp
        compile/lib/double_value/double_value.cpp
        compile/lib/string_value/string_value.cpp
        compile/lib/user_function/user_function.cpp
        compile/parser/parser.cpp
        compile/lib/int_value/int_value.cpp
        compile/lib/short_value/short_value.cpp
        compile/lib/long_value/long_value.cpp
        compile/lib/boolean_value/boolean_value.cpp
        compile/lib/array_value/array_value.cpp
        compile/lib/value.cpp
        compile/ast/statements/initialization_statement/initialization_statement.cpp
        compile/generator/generator.cpp
        runtime/jit/jit_compiler.cpp
        runtime/vm/vm.cpp
        runtime/vm/vm.h
        runtime/jit/jit_compiler.h
        compile/ast/statements/array_assignment_statement/array_assignment_statement.cpp
        compile/ast/statements/array_initialization_statement/array_initialization_statement.cpp
        compile/ast/expressions/array_access_expression/array_access_expression.cpp
        compile/ast/statements/array_assignment_statement/array_assignment_statement.h
        compile/ast/statements/array_initialization_statement/array_initialization_statement.h
        compile/ast/expressions/array_access_expression/array_access_expression.h
)

include_directories(/opt/homebrew/opt/llvm/include)
include_directories(/opt/homebrew/opt/bdw-gc/include)

link_directories(/opt/homebrew/opt/llvm/lib)
link_directories(/opt/homebrew/opt/bdw-gc/lib)

target_link_libraries(vinum /opt/homebrew/opt/llvm/lib/libLLVM-19.dylib)
find_library(GC_LIBRARY gc REQUIRED)
target_link_libraries(vinum ${GC_LIBRARY})