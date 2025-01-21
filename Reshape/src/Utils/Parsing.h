// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef PARSING_H
#define PARSING_H

#include "Forge/Forge.hpp"
#include <string>

namespace reshape {

class CommandLineParser {
public:
    static std::string ToLower(std::string str);
    static forge::GraphicsAPI ParseGraphicsAPI(const std::string& apiStr);
    static void PrintUsage();
    static forge::GraphicsAPI ParseCommandLine(int argc, char* argv[], bool& apiSpecified);
};

} // namespace reshape

#endif // PARSING_H
