#include "JumpStatement.h"

JumpStatement::JumpStatement(int val) : value(val) {}
JumpStatement::~JumpStatement() {}

void JumpStatement::execute(ProgramState* state, std::ostream& outf) {
    if (value > state->getnumline()) {  // illegal jump
        outf << "Illegal jump instruction" << std::endl;
    }
    state->jump(value);
}
