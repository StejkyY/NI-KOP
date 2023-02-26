#pragma once
#include <vector>
#include <iostream>

class Clause {
    std::vector <std::pair<int, bool>> variables;
    public:

        Clause (std::vector<std::pair<int, bool>> variables) {
            this->variables = variables;
        }

        bool evaluate(const std::vector<bool> &values) {
            for (long unsigned int i = 0; i < variables.size(); ++i) {
                if (values[variables[i].first - 1] == variables[i].second) return true;
            }
            return false;
        }
        std::vector<int> getVariablesNames () {
            std::vector<int> varNames;
            for (long unsigned int i = 0; i < variables.size(); ++i) {
                varNames.push_back(variables[i].first);
            }
            return varNames;
        }
};
