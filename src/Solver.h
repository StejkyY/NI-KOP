#pragma once
#include <random>
#include <set>
#include <numeric>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "Formula.h"

class Solver {
    private:
        Formula formula;
        int variablesCount;
        int stopIfNoImprovement;
        double startingTemperature;
        double finalTemperature;
        double coolingCoeff;
        int equilibriumConst;
        int clauseLength;


        double temperature;
        int totalIterations;

        std::vector<bool> configuration;

        std::vector<bool> bestConfiguration;

        bool isFrozen(double & t) {
            if (t > finalTemperature) {
                return false;
            } else {
                return true;
            }
        }

        bool equilibrium(int n) {
            return n > equilibriumConst;
        }

        void cool(double & t) {
            t *= coolingCoeff;
        }

        double weightSum (const std::vector<bool> & config) {
            double weight = 0;
            for (int i = 0; i < variablesCount; ++i) {
                weight += config[i] * formula.getWeights()[i];
            }
            return weight;
        }

        double cost(const std::vector<bool> & config) {
            if (formula.evaluate(config)) {
                return weightSum(config);
            }
            return formula.clausesCompleted(config) - (int)formula.getClauses().size();
        }

        std::vector<bool> tryNeighbourState(std::vector<bool> & currentConfiguration) {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> distSelection(0, 100);

            std::vector<bool> newConfiguration = currentConfiguration;

            if (distSelection(rng) % 10 != 0) {
                if (formula.evaluate(currentConfiguration)) {
                    std::uniform_int_distribution<std::mt19937::result_type> dist(0, variablesCount - 1);
                    newConfiguration[dist(rng)].flip();
                } else {
                    std::vector<Clause> clauses = formula.getClauses();
                    for (long unsigned int i = 0; i < clauses.size(); ++i) {
                        if (!clauses[i].evaluate(currentConfiguration)) {
                            std::uniform_int_distribution<std::mt19937::result_type> distFlip(0, clauseLength - 1);
                            newConfiguration[clauses[i].getVariablesNames()[distFlip(rng)] - 1].flip();
                            return newConfiguration;
                        }
                    }
                }
            } else {
                for (auto i = 0; i < variablesCount; ++i) {
                    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
                    newConfiguration[i] = dist(rng);
                }
            }

            return newConfiguration;
        }

    public:
        Solver (Formula & formula, int variablesCount) {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(0,1);

            this->formula = formula;
            this->variablesCount = variablesCount;
            for (int i = 0; i < variablesCount; ++i) {
                configuration.emplace_back(dist(rng));
            }
            bestConfiguration = configuration;
            clauseLength = 3;
        }

        void calculateStartingTemperature() {
            std::vector<bool> currentConfiguration = bestConfiguration;
            std::vector<bool> newConfiguration = bestConfiguration;
            std::vector<double> worseCosts;
            temperature = 1;
            double diff = 0;

            for (int i = 0; i < 1000; ++i) {
                newConfiguration = tryNeighbourState(currentConfiguration);
                diff = cost(newConfiguration) - cost(currentConfiguration);
                if (diff < 0) {
                    diff *= -1;
                    worseCosts.push_back(diff);
                }
                currentConfiguration = newConfiguration;
            }
            startingTemperature = -1 * (std::accumulate(worseCosts.begin(), worseCosts.end(), 0.0)/worseCosts.size()) / std::log(0.5);
        }

        void setStartingParameters ( double finalTemperature,
                                     double coolingCoeff, int equilibriumConst, int stopIfNoImprovement) {
            this->finalTemperature = finalTemperature;
            this->coolingCoeff = coolingCoeff;
            this->equilibriumConst = equilibriumConst;
            this->stopIfNoImprovement = stopIfNoImprovement;
        }

        void solve ( ) {
            temperature = startingTemperature;
            int equilibriumIterations = 0;
            int noImprovement = 0;
            totalIterations = 0;
            std::vector<bool> config;

            std::random_device dev;
            std::mt19937 rng(dev());


            while (!isFrozen(temperature)) {
                while (!equilibrium(equilibriumIterations)) {
                    config = tryNeighbourState(configuration);
                    if(cost(config) > cost (configuration)) {
                        if (cost(config) > cost(bestConfiguration)) {
                            bestConfiguration = config;
                        }
                        noImprovement = 0;
                        configuration = config;
                    }
                    else {
                        double delta = cost (configuration) - cost(config);
                        std::uniform_real_distribution<> dist(0.0,1.0);
                        if ( dist(rng) < exp(-delta/temperature)) {
                            configuration = config;
                            noImprovement = 0;
                        } else {
                            noImprovement++;
                            if(noImprovement == stopIfNoImprovement) {
                                return;
                            }
                        }
                    }
                    equilibriumIterations++;
                    totalIterations++;
                }

                cool (temperature);
                equilibriumIterations = 0;
            }

        }

        void printResult(std::string fileName, std::string outputFile) {
            std::fstream outFile (outputFile, std::ios::app);
            outFile << fileName << "," << totalIterations << "," <<
                formula.clausesCompleted(bestConfiguration) << "," <<
                    formula.getClauses().size() << "," << weightSum(bestConfiguration) << "," << std::endl;
        }
};
