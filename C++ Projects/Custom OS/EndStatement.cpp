// ReturnStatement.cpp:
#include "EndStatement.h"

EndStatement::EndStatement() {}
EndStatement::~EndStatement() {}

void EndStatement::execute(ProgramState* state, std::ostream& outf) {
    state->jump(state->getnumline());  // pushes to the end
}
