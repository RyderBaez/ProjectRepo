// ReturnStatement.cpp:
#include "ReturnStatement.h"

ReturnStatement::ReturnStatement() {}
ReturnStatement::~ReturnStatement() {}

void ReturnStatement::execute(ProgramState* state, std::ostream& outf) {
    int returnpnt = (state->getreturnpnt());
    // std::cout << returnpnt << std::endl;
    state->jump(returnpnt);  // returns to the latests return pnt
}
