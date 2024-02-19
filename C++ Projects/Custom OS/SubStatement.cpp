// LetStatement.cpp:
#include "SubStatement.h"
using namespace std;

SubStatement::SubStatement(std::string variableName, std::string value)
        : m_variableName(variableName), m_value(value) {}

SubStatement::~SubStatement() {}
// The LetStatement version of execute() should make two changes to the
// state of the program:
//
//    * set the value of the appropriate variable
//    * increment the program counter
void SubStatement::execute(ProgramState* state, ostream& outf) {
    bool done = false;
    // TODO
    if (!(state->mapfind(m_variableName))) {  // checks if in map
        state->mapadd(m_variableName, std::string("0"));
    }
    for (size_t i = 0; i < m_value.size(); i++) {
        if ((m_value[i] - '0' > 9 || m_value[i] - '0' < 0) && !(i == 0 && m_value[i] == '-')) {
            int newval = state->mapget(m_variableName) - state->mapget(m_value);  // checks for string
            state->mapset(m_variableName, newval);
            done = true;
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
            return;
        }
    }
    if (!done) {
        int newval = state->mapget(m_variableName) - stoi(m_value);
        state->mapset(m_variableName, newval);  // if an int
    }
    state->jump(state->getcurrentline() + 1);  // moves to next line of code
}
