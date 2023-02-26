#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Formula.h"

class Loader  {
private:
    Formula formula;
    int variablesCount;

public:
    Loader () {
    }

    Formula & getFormula() {
        return formula;
    }

    int getVariablesCount() {
        return variablesCount;
    }

    void LoadFile(std::string fileName) {
        std::vector<std::string> row;
        std::string line, word;

        std::fstream fin (fileName, std::fstream::in);
        int clausesCount = 0;
        bool readingInfo = true;
        std::vector<int> weights;
        std::vector<Clause> clauses;
        std::vector<std::pair<int, bool>> tmpClauseVariables;

        while (getline(fin, line)) {
            row.clear();
            // used for breaking words
            std::istringstream s(line);

            while (getline(s, word, ' ')) {
                row.push_back(word);
            }

            if (readingInfo) {
                switch (row[0].c_str()[0]) {
                    case 'c' :
                        if (weights.size() != 0) readingInfo = false;
                        break;
                    case 'p' :
                        variablesCount = std::atoi(row[2].c_str());
                        break;
                    case 'w' :
                        for (long unsigned int i = 1; i < row.size() - 1; ++i) weights.emplace_back(std::atoi(row[i].c_str()));
                        break;
                }
            } else {
                    for (long unsigned int i = 0; i < row.size() - 1; ++i) {
                        if (row[i].size() == 0) continue;
                        if ( row[i][0] == '-' ) tmpClauseVariables.emplace_back(std::atoi(row[i].substr(1, row[i].length() - 1).c_str()), false);
                        else tmpClauseVariables.emplace_back(std::atoi(row[i].c_str()), true);
                    }
                    Clause tmp = Clause(tmpClauseVariables);
                    tmpClauseVariables.clear();

                    clauses.push_back(tmp);
            }
        }
        formula = Formula(weights, clauses);
    }
};
