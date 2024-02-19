// PrintStatement.cpp:
#include "PrintAllStatement.h"

PrintAllStatement::PrintAllStatement() {}
PrintAllStatement::~PrintAllStatement() {}

void PrintAllStatement::execute(ProgramState* state, std::ostream& outf) {
    state->mapprintall(outf);                  // prints all
    state->jump(state->getcurrentline() + 1);  // moves to next line of code
}
