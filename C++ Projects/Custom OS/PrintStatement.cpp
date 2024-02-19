// PrintStatement.cpp:
#include "PrintStatement.h"

#include <iostream>
#include <sstream>
#include <string>
PrintStatement::PrintStatement(std::string variableName) : m_variableName(variableName) {}
PrintStatement::~PrintStatement() {}

void PrintStatement::execute(ProgramState* state, std::ostream& outf) {
    if (!(state->mapfind(m_variableName))) {  // finds if in map otherwise add it with 0
        state->mapadd(m_variableName, std::string("0"));
    }
    // std::cout << "ye" << std::endl;
    // std::cout << state->mapget(m_variableName) << std::endl;
    outf << state->mapget(m_variableName) << std::endl;  // print it
    state->jump(state->getcurrentline() + 1);            // moves to next line of code
}
