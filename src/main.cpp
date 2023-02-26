#include <string>
#include "Clause.h"
#include "Loader.h"
#include "Solver.h"

#define DEFAULT_COOLING_COEFF 0.995
#define DEFAULT_EQUILIBRIUM_CONST 250
#define DEFAULT_STOP_IF_NO_IMPROVEMENT 1000
#define DEFAULT_FINAL_TEMPERATURE 0.1


int main(int argc, char **argv) {
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    Loader loader = Loader();
    loader.LoadFile(inputFile);
    Solver solver = Solver(loader.getFormula(), loader.getVariablesCount());
    solver.setStartingParameters(DEFAULT_FINAL_TEMPERATURE, DEFAULT_COOLING_COEFF, DEFAULT_EQUILIBRIUM_CONST, DEFAULT_STOP_IF_NO_IMPROVEMENT);
    solver.calculateStartingTemperature();
    solver.solve();
    solver.printResult(inputFile, outputFile);
    return 0;
}
