#ifndef ASMGENERATOR_HPP
#define ASMGENERATOR_HPP

#include <string>
#include <vector>
#include <memory>

#include "common/irinstructions.hpp"

std::string generate_assembly(const std::vector<std::unique_ptr<IRInstr>> &ir);

#endif
