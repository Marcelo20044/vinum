#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../../function.h"
#include <unordered_map>
#include <string>

class Functions {
public:
    static bool isExists(const std::string &key);

    static std::shared_ptr<Function> get(const std::string &key);

    static void set(const std::string &key, std::shared_ptr<Function> function);

    static void initialize();

private:
    static std::unordered_map<std::string, std::shared_ptr<Function> > functions;
};

#endif // FUNCTIONS_H
