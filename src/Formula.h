#pragma once
#include "Clause.h"
#include <vector>

class Formula {
private:
    std::vector<int> weights;
    std::vector<Clause> clauses;
    int weightsSum;

public:
    Formula () = default;

    Formula (std::vector<int> weights, std::vector<Clause> clauses) {
        this->weights = weights;
        this->clauses = clauses;
        weightsSum = 0;
        for (long unsigned int i = 0; i < weights.size(); ++i){
            weightsSum += weights[i];
        }
    }

    std::vector<Clause> getClauses () {
        return clauses;
    }

    std::vector<int> getWeights () {
        return weights;
    }

    int getWeightsSum () {
        return weightsSum;
    }

    bool evaluate (const std::vector<bool> &values) {
        for (long unsigned int i = 0; i < clauses.size(); ++i) {
            if(!clauses[i].evaluate(values)) return false;
        }
        return true;
    }

    int clausesCompleted (const std::vector<bool> &values) {
        int count = 0;
        for (long unsigned int i = 0; i < clauses.size(); ++i) {
            if(clauses[i].evaluate(values)) count++;
        }
        return count;
    }

};