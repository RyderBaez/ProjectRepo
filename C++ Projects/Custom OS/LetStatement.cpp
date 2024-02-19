// LetStatement.cpp:
#include "LetStatement.h"
using namespace std;

LetStatement::LetStatement(std::string variableName, std::string value)
        : m_variableName(variableName), m_value(value) {}
LetStatement::~LetStatement() {}

// The LetStatement version of execute() should make two changes to the
// state of the program:
//
//    * set the value of the appropriate variable
//    * increment the program counter
void LetStatement::execute(ProgramState* state, ostream& outf) {
    // TODO

    if (!(state->mapfind(m_variableName))) {  // adds if new

        state->mapadd(m_variableName, m_value);

    } else {

        state->mapset(m_variableName, m_value);  // puts in new
    }
    state->jump(state->getcurrentline() + 1);  // moves to next line of code
}
