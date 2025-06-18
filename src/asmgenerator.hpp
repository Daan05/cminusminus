#ifndef ASMGENERATOR_HPP
#define ASMGENERATOR_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "common/irinstructions.hpp"
#include "irgenerator.hpp"

std::string generate_assembly(
    std::pair<
        std::vector<std::unique_ptr<IRInstr>>,
        std::unordered_map<std::string, IRFunction>>
        &ir
);

#endif
