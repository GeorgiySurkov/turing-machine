#include <iostream>
#include <fstream>
#include "turingmachine.h"

void readTMParams(std::istream &is, size_t &tapeSize, step_t &maxSteps, std::string &init) {
    is >> tapeSize >> maxSteps >> init;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Missing arguments: program path, input path, output path";
        return 1;
    }

    // Open files specified in args
    std::ifstream programFile(argv[1]), tmParamsFile(argv[2]);
    if (programFile.fail()) {
        std::cerr << "Error while opening file '" << argv[1] << "'" << std::endl;
        return 1;
    }
    if (tmParamsFile.fail()) {
        std::cerr << "Error while opening file '" << argv[2] << "'" << std::endl;
        return 1;
    }

    // Read program from file
    Program program;
    try {
        program.readFromStream(programFile);
    } catch (const std::invalid_argument &err) {
        std::cerr << err.what() << std::endl;
        std::ofstream outputFile(argv[3]);
        outputFile << "Error";
        return 1;
    }

    // Read Turing machine parameters from file
    size_t tapeSize;
    step_t maxSteps;
    std::string init;
    readTMParams(tmParamsFile, tapeSize, maxSteps, init);

    TuringMachine tm(tapeSize, program);
    std::string result = tm.calc(init, maxSteps);

    // Write execution result to output file
    std::ofstream outputFile(argv[3]);
    if (outputFile.fail()) {
        std::cerr << "Error while opening file '" << argv[3] << "'" << std::endl;
        return 1;
    }
    outputFile << result;
    return 0;
}
