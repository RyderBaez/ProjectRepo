#include "JumpSubStatement.h"

JumpSubStatement::JumpSubStatement(int val) : value(val) {}
JumpSubStatement::~JumpSubStatement() {}

void JumpSubStatement::execute(ProgramState* state, std::ostream& outf) {
    if (value > state->getnumline()) {  // illegal jump
        outf << "Illegal jump instruction" << std::endl;
    }
    state->jumpsetreturn(value);
}
